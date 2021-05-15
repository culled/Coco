#pragma once
#include "Core/Base.h"
#include "Asset.h"

#include <unordered_map>
#include <functional>

namespace Coco
{
	class COCO_API AssetFactory
	{
	public:
		template <typename T>
		AssetFactory(const std::function<Ref<T>>& func) :
			m_Func(func)
		{}

		template <typename T>
		Ref<T> Create(const std::string& filePath)
		{
			return static_cast<Ref<T>>(m_Func(filePath));
		}

	private:
		std::function<void*(std::string)> m_Func;
	};

	class COCO_API AssetManager
	{
	public:
		using AssetMap = std::unordered_map <std::string, Scope<Asset>>;

		template<typename T>
		Ref<T> Load(const std::string& filePath)
		{
			AssetMap::iterator it = m_Assets.find(filePath);

			if (it != m_Assets.end())
			{
				return static_cast<AssetResource<T>*>(it->second.get())->GetResource();
			}

			size_t type = type_info(T).hash_code();

			if (m_AssetFactories.find(type) == m_AssetFactories.end())
			{
				ASSERT_CORE(false, "A factory was not given for the type");
				return nullptr;
			}

			m_Assets[filePath] = CreateScope<AssetResource<T>>(filePath, filePath, m_AssetFactories[type].Create<T>(filePath));
			return static_cast<AssetResource<T>*>(m_Assets[filePath]->second.get())->GetResource();
		}

		void ReleaseAll()
		{
			AssetMap::iterator it = m_Assets.begin();

			while (it != m_Assets.end())
			{
				it = m_Assets.erase(it);
			}
		}

		template<typename T, typename ...Args>
		void RegisterFactory(const std::function<Ref<T>(std::string)>& func)
		{
			size_t type = type_info(T).hash_code();

			if (m_AssetFactories.find(type) != m_AssetFactories.end())
			{
				ASSERT_CORE(false, "Factory has already been registered for the given type");
				return;
			}

			m_AssetFactories.try_emplace(type, AssetFactory(func));
		}

	private:
		AssetMap m_Assets;
		std::unordered_map <size_t, AssetFactory> m_AssetFactories;
	};
}