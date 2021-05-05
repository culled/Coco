#pragma once
#include "Core/Base.h"
#include <string>

#include "Core/Scene/Entity.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace Coco
{
	struct COCO_API TagComponent
	{
		std::string Name;
		std::string Tags;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& name, const std::string& tags = "") : Name(name), Tags(tags) {}
	};

	struct COCO_API TransformComponent
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& position, const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f)) : 
			Position(position), Rotation(rotation), Scale(scale) {}

		operator glm::mat4()
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), Position) *
				glm::mat4_cast(glm::quat(Rotation)) *
				glm::scale(glm::mat4(1.0f), Scale);

			return transform;
		}
		operator const glm::mat4() const
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), Position) *
				glm::mat4_cast(glm::quat(Rotation)) *
				glm::scale(glm::mat4(1.0f), Scale);

			return transform;
		}
	};

	struct COCO_API NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void(*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() {return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) {delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}