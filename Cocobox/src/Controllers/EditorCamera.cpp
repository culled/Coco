#include "EditorCamera.h"

namespace Coco
{
	EditorCamera::EditorCamera(float startingZoom, bool orthographic) : 
		m_ZoomLevel(startingZoom), m_Orthographic(orthographic)
	{
		m_Camera = CreateScope<SceneCamera>();

		if (m_Orthographic)
		{
			m_Camera->SetOrthographic(m_ZoomLevel, 0.1f, 100.0f);
		}
		else
		{
			m_Camera->SetPerspective(m_PerspectiveFOV, 0.1f, 100.0f);
		}

		RecalculateDirections();
	}

	void EditorCamera::OnUpdate(Timestep timestep)
	{
		if (!m_ControlEnabled) return;

		if (Input::IsKeyPressed(KeyCodes::D))
		{
			m_Origin += m_RightDirection * (m_MoveSpeed * timestep * m_SpeedMulti);
		}
		else if (Input::IsKeyPressed(KeyCodes::A))
		{
			m_Origin -= m_RightDirection * (m_MoveSpeed * timestep * m_SpeedMulti);
		}

		if (Input::IsKeyPressed(KeyCodes::W))
		{
			if (m_Orthographic)
			{
				m_Origin += m_UpDirection * (m_MoveSpeed * timestep * m_SpeedMulti);
			}
			else
			{
				m_Origin += m_LookDirection * (m_MoveSpeed * timestep * m_SpeedMulti);
			}
		}
		else if (Input::IsKeyPressed(KeyCodes::S))
		{
			if (m_Orthographic)
			{
				m_Origin -= m_UpDirection * (m_MoveSpeed * timestep * m_SpeedMulti);
			}
			else
			{
				m_Origin -= m_LookDirection * (m_MoveSpeed * timestep * m_SpeedMulti);
			}
		}

		if (Input::IsKeyPressed(KeyCodes::Space))
		{
			if (!m_Orthographic)
			{
				m_Origin += m_UpDirection * (m_MoveSpeed * timestep * m_SpeedMulti);
			}
		}
		else if (Input::IsKeyPressed(KeyCodes::Left_Shift))
		{
			if (!m_Orthographic)
			{
				m_Origin -= m_UpDirection * (m_MoveSpeed * timestep * m_SpeedMulti);
			}
		}
	}

	void EditorCamera::SetMouseHovering(bool hovering)
	{
		m_MouseHovering = hovering;

		if (!hovering) m_WasHovering = false;
	}

	void EditorCamera::OnEvent(DispatchedEvent& e)
	{
		EventDispatcher::Dispatch<ScrollEventArgs>(e, this, &EditorCamera::OnMouseScrolled);
		EventDispatcher::Dispatch<CursorPositionEventArgs>(e, this, &EditorCamera::OnCursorMoved);
	}

	void EditorCamera::SetControlEnabled(bool enabled)
	{
		m_ControlEnabled = enabled;
	}

	glm::mat4 EditorCamera::GetTransform() const
	{
		glm::vec3 camPos = m_Origin - (m_LookDirection * m_ZoomLevel);
		glm::mat4 camTransform = glm::lookAt(camPos, m_Origin, m_UpDirection);

		return glm::inverse(camTransform);
	}

	float EditorCamera::CalculateZoomSpeed()
	{
		//return std::sqrt(m_ZoomLevel);
		return m_ZoomLevel;
	}

	void EditorCamera::OnMouseScrolled(ScrollEventArgs* args)
	{
		m_ZoomLevel -= args->yDelta * m_ZoomSpeed * m_ZoomLevel;

		m_ZoomLevel = std::max(m_ZoomLevel, m_MinZoom);
		m_SpeedMulti = CalculateZoomSpeed();

		if (m_Camera->GetProjectionType() == SceneCamera::Projection::Orthographic)
		{
			m_Camera->SetOrthographicSize(m_ZoomLevel);
		}
	}

	void EditorCamera::OnCursorMoved(CursorPositionEventArgs* args)
	{
		glm::vec3 pos = glm::vec3(args->xPos, args->yPos, 0.0f);

		glm::vec3 delta = glm::vec3(args->xPos - m_MousePosition.x, args->yPos - m_MousePosition.y, 0.0f);

		if (!m_WasHovering)
		{
			delta = glm::vec3(0.0f);
			m_WasHovering = true;
		}

		if (Input::IsMouseButtonPressed(MouseButtons::Button_3))
		{
			m_Origin += m_RightDirection * (-delta.x * 0.002f * m_SpeedMulti) + m_UpDirection * (delta.y * 0.002f * m_SpeedMulti);
		}
		else if (Input::IsMouseButtonPressed(MouseButtons::Button_2))
		{
			m_Yaw -= delta.x * 0.01f;
			m_Pitch += delta.y * 0.01f;

			if (m_Pitch < m_MinPitch) m_Pitch = m_MinPitch;
			if (m_Pitch > m_MaxPitch) m_Pitch = m_MaxPitch;

			RecalculateDirections();
		}

		m_MousePosition = pos;
	}

	glm::quat EditorCamera::GetOrientation()
	{
		return glm::quat(glm::vec3(m_Pitch, m_Yaw, 0.0f));
	}

	void EditorCamera::RecalculateDirections()
	{
		glm::quat orientation = GetOrientation();

		m_LookDirection = glm::rotate(orientation, glm::vec3(0.0f, 0.0f, 1.0f));
		m_UpDirection = glm::rotate(orientation, glm::vec3(0.0f, 1.0f, 0.0f));
		m_RightDirection = glm::rotate(orientation, glm::vec3(-1.0f, 0.0f, 0.0f));
	}
}