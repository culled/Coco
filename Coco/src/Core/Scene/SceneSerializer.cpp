#include "ccpch.h"
#include "SceneSerializer.h"

#include "Core/App/Log.h"
#include "Core/Components/Components.h"
#include "Core/Components/RenderingComponents.h"
#include <yaml-cpp/yaml.h>
#include <fstream>

namespace YAML
{
	template <>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& value)
		{
			Node node;
			node.push_back(value.x);
			node.push_back(value.y);
			node.push_back(value.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& value)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			value.x = node[0].as<float>();
			value.y = node[1].as<float>();
			value.z = node[2].as<float>();

			return true;
		}
	};

	Emitter& operator<<(Emitter& writer, const glm::vec3& value)
	{
		writer << Flow;
		writer << BeginSeq << value.x << value.y << value.z << EndSeq;
		return writer;
	}

	template <>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& value)
		{
			Node node;
			node.push_back(value.x);
			node.push_back(value.y);
			node.push_back(value.z);
			node.push_back(value.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& value)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			value.x = node[0].as<float>();
			value.y = node[1].as<float>();
			value.z = node[2].as<float>();
			value.w = node[3].as<float>();

			return true;
		}
	};

	Emitter& operator<<(Emitter& writer, const glm::vec4& value)
	{
		writer << Flow;
		writer << BeginSeq << value.x << value.y << value.z << value.w << EndSeq;
		return writer;
	}
}

namespace Coco
{
	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) :
		m_Scene(scene)
	{
	}

	void SceneSerializer::Serialize(const std::string& filePath)
	{
		YAML::Emitter writer;
		writer << YAML::BeginMap;
		writer << YAML::Key << "Scene" << YAML::Value << "Untitled Scene";
		writer << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_Scene->m_Registry.each([&](auto entityID) {
			Entity entity(entityID, m_Scene.get());
			if (!entity)
				return;

			SerializeEntity(entity, writer);
			});
		writer << YAML::EndSeq;
		writer << YAML::EndMap;

		std::ofstream file(filePath);
		file << writer.c_str();
		file.close();
	}

	void SceneSerializer::SerializeBinary(const std::string& filePath)
	{
		ASSERT_CORE(false, "Not implemented yet");
	}

	void SceneSerializer::Deserialize(const std::string& filePath)
	{
		std::ifstream file(filePath);
		std::stringstream fileString;
		fileString << file.rdbuf();
		file.close();

		YAML::Node data = YAML::Load(fileString.str());
		if (!data["Scene"])
			return; //TODO: error

		std::string sceneName = data["Scene"].as<std::string>();

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["ID"].as<uint64_t>();

				std::string name, tags;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
				{
					name = tagComponent["Name"].as<std::string>();
					tags = tagComponent["Tags"].as<std::string>();
				}

				Entity deserializedEntity = m_Scene->CreateEntity(name);
				deserializedEntity.GetComponent<TagComponent>().Tags = tags;

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& transform = deserializedEntity.GetComponent<TransformComponent>();

					transform.Position = transformComponent["Position"].as<glm::vec3>();
					transform.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					transform.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& camera = deserializedEntity.AddComponent<CameraComponent>();

					camera.Camera.SetProjectionType((SceneCamera::Projection)cameraComponent["ProjectionType"].as<int>());

					camera.Camera.SetPerspectiveVerticalFOV(cameraComponent["PerspectiveVerticalFOV"].as<float>());

					camera.Camera.SetOrthographicSize(cameraComponent["OrthographicSize"].as<float>());

					camera.Camera.SetNearPlane(cameraComponent["NearPlane"].as<float>());
					camera.Camera.SetFarPlane(cameraComponent["FarPlane"].as<float>());
				}

				auto src = entity["SpriteRendererComponent"];
				if (src)
				{
					auto& spriteRenderer = deserializedEntity.AddComponent<SpriteRendererComponent>();

					spriteRenderer.Color = src["Color"].as<glm::vec4>();
				}
			}
		}
	}

	void SceneSerializer::DeserializeBinary(const std::string& filePath)
	{
		ASSERT_CORE(false, "Not implemented yet");
	}

	void SceneSerializer::SerializeEntity(Entity entity, YAML::Emitter& writer)
	{
		writer << YAML::BeginMap;
		writer << YAML::Key << "ID" << YAML::Value << (uint32_t)entity; //TODO: get entity uuid
		
		if (entity.HasComponent<TagComponent>())
		{
			auto& tagComponent = entity.GetComponent<TagComponent>();

			writer << YAML::Key << "TagComponent" << YAML::Value << YAML::BeginMap;
			writer << YAML::Key << "Name" << YAML::Value << tagComponent.Name;
			writer << YAML::Key << "Tags" << YAML::Value << tagComponent.Tags;
			writer << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			auto& transformComponent = entity.GetComponent<TransformComponent>();

			writer << YAML::Key << "TransformComponent" << YAML::Value << YAML::BeginMap;
			writer << YAML::Key << "Position" << YAML::Value << transformComponent.Position;
			writer << YAML::Key << "Rotation" << YAML::Value << transformComponent.Rotation;
			writer << YAML::Key << "Scale" << YAML::Value << transformComponent.Scale;
			writer << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			auto& cameraComponent = entity.GetComponent<CameraComponent>();

			writer << YAML::Key << "CameraComponent" << YAML::Value << YAML::BeginMap;

			writer << YAML::Key << "ProjectionType" << YAML::Value << (int)cameraComponent.Camera.GetProjectionType();

			writer << YAML::Key << "PerspectiveVerticalFOV" << YAML::Value << cameraComponent.Camera.GetPerspectiveVerticalFOV();

			writer << YAML::Key << "OrthographicSize" << YAML::Value << cameraComponent.Camera.GetOrthographicSize();

			writer << YAML::Key << "NearPlane" << YAML::Value << cameraComponent.Camera.GetNearPlane();
			writer << YAML::Key << "FarPlane" << YAML::Value << cameraComponent.Camera.GetFarPlane();

			writer << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			auto& src = entity.GetComponent<SpriteRendererComponent>();

			writer << YAML::Key << "SpriteRendererComponent" << YAML::Value << YAML::BeginMap;

			writer << YAML::Key << "Color" << YAML::Value << src.Color;

			writer << YAML::EndMap;
		}

		writer << YAML::EndMap;
	}
}