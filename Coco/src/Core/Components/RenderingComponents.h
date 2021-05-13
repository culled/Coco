#pragma once
#include "Core/Base.h"

#include "Core/Rendering/MeshData.h"
#include "Core/Rendering/Camera.h"
#include "Core/Rendering/Material.h"
#include "Core/Rendering/VertexArray.h"

#include <glm/glm.hpp>

namespace Coco
{
	struct COCO_API SpriteRendererComponent
	{
		glm::vec4 Color = glm::vec4(1.0f);

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
	};

	struct COCO_API MeshRendererComponent
	{
		Ref<MeshData> Data = nullptr;

		MeshRendererComponent() = default;
		MeshRendererComponent(const MeshRendererComponent&) = default;
		MeshRendererComponent(const Ref<MeshData>& data) : Data(data) {}
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