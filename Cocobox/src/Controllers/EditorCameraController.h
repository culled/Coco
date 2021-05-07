#pragma once
#include <Coco.h>

namespace Coco
{
	class EditorCameraController
	{
	public:
		EditorCameraController(const Ref<SceneCamera>& camera);
		virtual ~EditorCameraController() = default;

		void OnUpdate(Timestep timestep);
		void OnEvent(DispatchedEvent& e);

		void SetControlEnabled(bool enabled);

		Ref<SceneCamera> GetCamera() const { return m_Camera; }
		const glm::mat4& GetTransform() const { return m_Transform; }

	private:
		float m_ZoomLevel = 1.0f;
		float m_MinZoom = 0.1f;

		float m_MoveSpeed = 1.5f;
		float m_RotateSpeed = 90.0f;
		float m_ZoomSpeed = 0.4f;
		float m_SpeedMulti = 1.0f;

		bool m_Rotation = false;
		bool m_ControlEnabled = false;

		EventDispatcher m_Dispatcher;

		Ref<SceneCamera> m_Camera = nullptr;
		glm::mat4 m_Transform = glm::mat4(1.0f);

		void OnMouseScrolled(ScrollEventArgs* args);
		float CalculateZoomSpeed();
	};
}

