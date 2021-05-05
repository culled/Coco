#pragma once
#include <Coco.h>

namespace Coco
{
	class EditorCameraController : public ScriptableEntity
	{
	public:
		virtual void OnCreate() override;
		virtual void OnUpdate(Timestep timestep) override;
		void OnEvent(DispatchedEvent& e);

		void SetControlEnabled(bool enabled);

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

		void OnMouseScrolled(ScrollEventArgs* args);
		float CalculateZoomSpeed();
	};
}

