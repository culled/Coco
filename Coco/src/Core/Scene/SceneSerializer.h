#pragma once
#include "Core/Base.h"

#include "Scene.h"
#include "Entity.h"
#include <yaml-cpp/yaml.h>

namespace Coco
{
	class COCO_API SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filePath);
		void SerializeBinary(const std::string& filePath);

		void Deserialize(const std::string& filePath);
		void DeserializeBinary(const std::string& filePath);
	private:
		static void SerializeEntity(Entity entity, YAML::Emitter& writer);

		Ref<Scene> m_Scene;
	};
}

