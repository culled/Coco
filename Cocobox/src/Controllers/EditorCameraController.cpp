#include "EditorCameraController.h"

namespace Coco
{
	void EditorCameraController::OnCreate()
	{
		ASSERT_CORE(HasComponent<CameraComponent>() && HasComponent<TransformComponent>(), "Entity does not have a transform or orthographic camera");
		LOG_INFO("Created Camera2DController");

		auto& cam = GetComponent<CameraComponent>().Camera;
		cam.SetOrthographicSize(m_ZoomLevel);
	}

	void EditorCameraController::OnUpdate(Timestep timestep)
	{
		if (!m_ControlEnabled) return;

		auto& transform = GetComponent<TransformComponent>();

		if (Input::IsKeyPressed(KeyCodes::D))
		{
			transform.Position += glm::vec3(m_MoveSpeed * timestep * m_SpeedMulti, 0.0f, 0.0f);
		}
		else if (Input::IsKeyPressed(KeyCodes::A))
		{
			transform.Position += glm::vec3(-m_MoveSpeed * timestep * m_SpeedMulti, 0.0f, 0.0f);
		}

		if (Input::IsKeyPressed(KeyCodes::W))
		{
			transform.Position += glm::vec3(0.0f, m_MoveSpeed * timestep * m_SpeedMulti, 0.0f);
		}
		else if (Input::IsKeyPressed(KeyCodes::S))
		{
			transform.Position += glm::vec3(0.0f, -m_MoveSpeed * timestep * m_SpeedMulti, 0.0f);
		}

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(KeyCodes::E))
			{
				transform.Rotation += glm::vec3(0.0f, 0.0f, m_RotateSpeed * timestep * m_SpeedMulti);
			}
			else if (Input::IsKeyPressed(KeyCodes::Q))
			{
				transform.Rotation += glm::vec3(0.0f, 0.0f, -m_RotateSpeed * timestep * m_SpeedMulti);
			}
		}
	}

	void EditorCameraController::OnEvent(DispatchedEvent& e)
	{
		EventDispatcher::Dispatch<ScrollEventArgs>(e, this, &EditorCameraController::OnMouseScrolled);
	}

	void EditorCameraController::SetControlEnabled(bool enabled)
	{
		m_ControlEnabled = enabled;
	}

	float EditorCameraController::CalculateZoomSpeed()
	{
		return std::sqrt(m_ZoomLevel);
	}

	void EditorCameraController::OnMouseScrolled(ScrollEventArgs* args)
	{
		m_ZoomLevel -= args->yDelta * m_ZoomSpeed;

		m_ZoomLevel = std::max(m_ZoomLevel, m_MinZoom);
		m_SpeedMulti = CalculateZoomSpeed();

		auto& cam = GetComponent<CameraComponent>().Camera;
		auto& camTransform = GetComponent<TransformComponent>();

		if (cam.GetProjectionType() == SceneCamera::Projection::Orthographic)
		{
			cam.SetOrthographicSize(m_ZoomLevel);
			camTransform.Position.z = 0.0f;
		}
		else
		{
			camTransform.Position.z = m_ZoomLevel;
		}
	}
}