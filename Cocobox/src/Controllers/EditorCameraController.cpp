#include "EditorCameraController.h"

namespace Coco
{
	EditorCameraController::EditorCameraController(const Ref<SceneCamera>& camera) :
		m_Camera(camera)
	{
		camera->SetOrthographic(m_ZoomLevel, 0.0f, 2.0f);
		m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	void EditorCameraController::OnUpdate(Timestep timestep)
	{
		if (!m_ControlEnabled || !m_Camera) return;

		if (Input::IsKeyPressed(KeyCodes::D))
		{
			m_Transform = glm::translate(m_Transform, glm::vec3(m_MoveSpeed * timestep * m_SpeedMulti, 0.0f, 0.0f));
		}
		else if (Input::IsKeyPressed(KeyCodes::A))
		{
			m_Transform = glm::translate(m_Transform, glm::vec3(-m_MoveSpeed * timestep * m_SpeedMulti, 0.0f, 0.0f));
		}

		if (Input::IsKeyPressed(KeyCodes::W))
		{
			m_Transform = glm::translate(m_Transform, glm::vec3(0.0f, m_MoveSpeed * timestep * m_SpeedMulti, 0.0f));
		}
		else if (Input::IsKeyPressed(KeyCodes::S))
		{
			m_Transform = glm::translate(m_Transform, glm::vec3(0.0f, -m_MoveSpeed * timestep * m_SpeedMulti, 0.0f));
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

		if (m_Camera->GetProjectionType() == SceneCamera::Projection::Orthographic)
		{
			m_Camera->SetOrthographicSize(m_ZoomLevel);
		}
	}
}