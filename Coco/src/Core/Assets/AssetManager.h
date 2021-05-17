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
		AssetFactory() = default;
		AssetFactory(const std::function<void*(std::string)>& func) :
			m_Func(func)
		{}

		template <typename T>
		T* Create(const std::string& filePath)
		{
			return static_cast<T*>(m_Func(filePath));
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

			size_t type = typeid(T).hash_code();

			if (m_AssetFactories.find(type) == m_AssetFactories.end())
			{
				ASSERT_CORE(false, "A factory was not given for the type");
				return nullptr;
			}

			T* asset = m_AssetFactories[type].Create<T>(filePath);
			m_Assets[filePath] = CreateScope<AssetResource<T>>(filePath, filePath, asset);
			return static_cast<AssetResource<T>*>(m_Assets[filePath].get())->GetResource();
		}

		void ReleaseAll();

		template<typename T, typename ...Args>
		void RegisterFactory(const std::function<T*(std::string)>& func)
		{
			size_t type = typeid(T).hash_code();

			if (m_AssetFactories.find(type) != m_AssetFactories.end())
			{
				ASSERT_CORE(false, "Factory has already been registered for the given type");
				return;
			}

			m_AssetFactories[type] = AssetFactory(func);
		}

		template<typename T>
		void Reload(const std::string& filePath)
		{
			size_t type = typeid(T).hash_code();
			AssetMap::iterator it = m_Assets.find(filePath);

			if (it != m_Assets.end())
			{
				AssetResource<T>* asset = static_cast<AssetResource<T>*>(it->second.get());
				asset->SetData(m_AssetFactories[type].Create<T>(filePath));
			}
		}

	private:
		AssetMap m_Assets;
		std::unordered_map <size_t, AssetFactory> m_AssetFactories;
	};
}