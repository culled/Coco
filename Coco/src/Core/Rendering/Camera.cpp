#include "ccpch.h"
#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Coco
{
	Camera::Camera() : Camera(glm::mat4(1.0f))
	{}

	Camera::Camera(glm::mat4 projectionMatrix)
		: m_ProjectionMatrix(projectionMatrix)
	{}

	void Camera::SetProjectionMatrix(const glm::mat4& projectionMatrix)
	{
		m_ProjectionMatrix = projectionMatrix;
	}

	glm::mat4 Camera::OrthographicProjection(float size, float aspectRatio, float nearPlane, float farPlane)
	{
		return glm::ortho((size / -2.0f) * aspectRatio, (size / 2.0f) * aspectRatio, size / -2.0f, size / 2.0f, nearPlane, farPlane);
	}

	glm::mat4 Camera::PerspectiveProjection(float verticalFOV, float aspectRatio, float nearPlane, float farPlane)
	{
		return glm::perspective(glm::radians(verticalFOV), aspectRatio, nearPlane, farPlane);
	}

	SceneCamera::SceneCamera(Projection projection, float aspectRatio) :
		m_Projection(projection), m_AspectRatio(aspectRatio)
	{
		UpdateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFOV, float nearPlane, float farPlane)
	{
		m_Projection = Projection::Perspective;
		m_VerticalFOV = verticalFOV;
		m_PerspectiveNearPlane = nearPlane;
		m_PerspectiveFarPlane = farPlane;
		UpdateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearPlane, float farPlane)
	{
		m_Projection = Projection::Orthographic;
		m_OrthoSize = size;
		m_OrthoNearPlane = nearPlane;
		m_OrthoFarPlane = farPlane;
		UpdateProjection();
	}

	void SceneCamera::UpdateProjection()
	{
		glm::mat4 projMat(1.0f);

		switch (m_Projection)
		{
		case Projection::Orthographic:
			projMat = Camera::OrthographicProjection(m_OrthoSize, m_AspectRatio, m_OrthoNearPlane, m_OrthoFarPlane);
			break;
		case Projection::Perspective:
			projMat = Camera::PerspectiveProjection(m_VerticalFOV, m_AspectRatio, m_PerspectiveNearPlane, m_PerspectiveFarPlane);
			break;
		}

		SetProjectionMatrix(projMat);
	}
}