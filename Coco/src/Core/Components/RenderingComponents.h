#pragma once
#include "Core/Base.h"

#include "Core/Rendering/Camera.h"
#include "Core/Rendering/Material.h"
#include "Core/Rendering/VertexArray.h"
#include "glm/glm.hpp"

namespace Coco
{
	struct COCO_API SpriteRendererComponent
	{
		glm::vec4 Color = glm::vec4(1.0f);
		Ref<Material> Material;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const Ref<Coco::Material>& material, const glm::vec4& color = glm::vec4(1.0f)) : Material(material), Color(color) {}
	};

	struct COCO_API MeshDataComponent
	{
		Ref<VertexArray> VAO = nullptr;

		MeshDataComponent() = default;
		MeshDataComponent(const MeshDataComponent&) = default;
		MeshDataComponent(const Ref<VertexArray>& vao) : VAO(vao) {}
	};

	struct COCO_API CameraComponent
	{
		SceneCamera Camera;

		CameraComponent() = default;
		CameraComponent(const CameraComponent& other)
		{
			if (other.Camera.GetProjectionType() == SceneCamera::Projection::Orthographic)
			{
				Camera.SetOrthographic(other.Camera.GetOrthographicSize(), other.Camera.GetNearPlane(), other.Camera.GetFarPlane());
			}
			else
			{
				Camera.SetPerspective(other.Camera.GetPerspectiveVerticalFOV(), other.Camera.GetNearPlane(), other.Camera.GetFarPlane());
			}
		}

		CameraComponent(SceneCamera::Projection projectionType, float aspectRatio) :
			Camera(projectionType, aspectRatio) {}
	};
}