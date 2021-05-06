#pragma once
#include "Core/Base.h"

#include "glm/glm.hpp"

namespace Coco
{
	class COCO_API Camera
	{
	public:
		Camera();
		Camera(glm::mat4 projectionMatrix);

		virtual ~Camera() = default;

		virtual void SetProjectionMatrix(const glm::mat4& projectionMatrix);
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

		static glm::mat4 OrthographicProjection(float size, float aspectRatio, float nearPlane, float farPlane);
		static glm::mat4 PerspectiveProjection(float verticalFOV, float aspectRatio, float nearPlane, float farPlane);

	private:
		glm::mat4 m_ProjectionMatrix;
	};

	class COCO_API SceneCamera : public Camera
	{
	public:
		enum class Projection { Perspective = 0, Orthographic = 1 };

		SceneCamera() : SceneCamera(Projection::Perspective, 1.0f) {}

		SceneCamera(Projection projection, float aspectRatio);
		virtual ~SceneCamera() = default;

		void SetPerspectiveVerticalFOV(float verticalFOV) { m_VerticalFOV = verticalFOV; UpdateProjection(); }
		float GetPerspectiveVerticalFOV() const { return m_VerticalFOV; }

		void SetOrthographicSize(float size) { m_OrthoSize = size; UpdateProjection(); }
		float GetOrthographicSize() const { return m_OrthoSize; }

		void SetNearPlane(float nearPlane)
		{
			m_Projection == Projection::Orthographic ? m_OrthoNearPlane = nearPlane : m_PerspectiveNearPlane = nearPlane;
			UpdateProjection();
		}
		float GetNearPlane() const { return m_Projection == Projection::Orthographic ? m_OrthoNearPlane : m_PerspectiveNearPlane; }

		void SetFarPlane(float farPlane)
		{
			m_Projection == Projection::Orthographic ? m_OrthoFarPlane = farPlane : m_PerspectiveFarPlane = farPlane;
			UpdateProjection();
		}
		float GetFarPlane() const { return m_Projection == Projection::Orthographic ? m_OrthoFarPlane : m_PerspectiveFarPlane; }

		void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; UpdateProjection(); }
		float GetAspectRatio() const { return m_AspectRatio; }

		void SetProjectionType(Projection projection) { m_Projection = projection; UpdateProjection(); }
		Projection GetProjectionType() const { return m_Projection; }

		void SetPerspective(float verticalFOV, float nearPlane, float farPlane);
		void SetOrthographic(float size, float nearPlane, float farPlane);
	private:
		void UpdateProjection();

		float m_VerticalFOV = 75.0f, m_PerspectiveNearPlane = 0.1f, m_PerspectiveFarPlane = 100.0f;
		float m_OrthoSize = 5.0f, m_OrthoNearPlane = -1.0f, m_OrthoFarPlane = 1.0f;
		float m_AspectRatio = 1.0f;

		Projection m_Projection;
	};
}