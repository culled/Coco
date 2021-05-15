#pragma once
#include "Core/Base.h"

#include <string>

namespace Coco
{
	class COCO_API Asset
	{
	public:
		Asset(const std::string& id) :
			m_ID(id) {}

		virtual ~Asset() = default;

	protected:
		std::string m_ID;
	};

	template<typename T>
	class COCO_API AssetResource : public Asset
	{
	public:
		AssetResource(const std::string& id, const std::string& filePath, const Ref<T>& resource) :
			Asset(id), m_FilePath(filePath), m_Resource(resource) {}

		virtual ~AssetResource() = default;

		Ref<T> GetResource() { return m_Resource; }
		const Ref<T>& GetResource() const { return m_Resource; }

	private:
		std::string m_FilePath;
		Ref<T> m_Resource;
	};
}