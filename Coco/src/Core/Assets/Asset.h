#pragma once
#include "Core/Base.h"

#include <string>

namespace Coco
{
	class COCO_API Asset
	{
	public:
		Asset(const std::string& id, const std::string& filePath) :
			m_ID(id), m_FilePath(filePath) {}

		virtual ~Asset() = default;

	protected:
		std::string m_ID;
		std::string m_FilePath;
	};

	template<typename T>
	class COCO_API AssetResource : public Asset
	{
	public:
		AssetResource(const std::string& id, const std::string& filePath, T* resource) :
			Asset(id, filePath)
		{
			m_Resource.reset(resource);
		}

		virtual ~AssetResource() = default;

		Ref<T> GetResource() { return m_Resource; }
		const Ref<T>& GetResource() const { return m_Resource; }

		void SetData(T* resource)
		{
			m_Resource.reset(resource);
		}

	private:
		
		Ref<T> m_Resource = nullptr;
	};
}