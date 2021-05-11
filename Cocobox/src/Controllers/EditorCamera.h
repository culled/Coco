#pragma once
#include <Coco.h>

namespace Coco
{
	class EditorCamera
	{
	public:
		EditorCamera(float startingZoom, bool orthographic = false);
		virtual ~EditorCamera() = default;

		void OnUpdate(Timestep timestep);
		void OnEvent(DispatchedEvent& e);

		void SetControlEnabled(bool enabled);

		SceneCamera* GetCamera() const { return m_Camera.get(); }
		glm::mat4 GetTransform() const;

	private:
		float m_ZoomLevel = 1.0f;
		float m_MinZoom = 0.1f;
		float m_PerspectiveFOV = 75.0f;

		float m_MoveSpeed = 1.5f;
		float m_RotateSpeed = 90.0f;
		float m_ZoomSpeed = 0.1f;
		float m_SpeedMulti = 1.0f;
		glm::vec3 m_MousePosition = glm::vec3(0.0f);

		bool m_Orthographic = false;
		bool m_ControlEnabled = false;

		EventDispatcher m_Dispatcher;

		Scope<SceneCamera> m_Camera = nullptr;

		glm::vec3 m_Origin = glm::vec3(0.0f);
		float m_Yaw = 0.0f, m_Pitch = 0.0f;
		float m_MinPitch = glm::radians(-90.0f);
		float m_MaxPitch = glm::radians(90.0f);

		glm::vec3 m_LookDirection = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_RightDirection = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 m_UpDirection = glm::vec3(0.0f, 1.0f, 0.0f);

		float CalculateZoomSpeed();
		void OnMouseScrolled(ScrollEventArgs* args);
		void OnCursorMoved(CursorPositionEventArgs* args);
		glm::quat GetOrientation();
		void RecalculateDirections();
	};
}

