#pragma once
#include <Coco.h>

#include "../Controllers/EditorCamera.h"
#include "glm/glm.hpp"

namespace Coco
{
	class COCO_API ScenePanel
	{
	public:
		ScenePanel();
		ScenePanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnUpdate(Timestep timestep);
		void OnImGuiRender();
		void OnEvent(DispatchedEvent& e);

	private:
		void OnKeyPressed(KeyPressEventArgs* args);

		glm::vec2 m_ViewportSize = glm::vec2(320.0f, 180.0f);
		Ref<Framebuffer> m_Framebuffer = nullptr;

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		Ref<Scene> m_Context = nullptr;
		Scope<EditorCamera> m_EditorCamera = nullptr;

		int m_GizmoType = -1;
	};
}

