#pragma once
#include <Coco.h>

#include "../Controllers/EditorCameraController.h"
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
		glm::vec2 m_ViewportSize = glm::vec2(320.0f, 180.0f);
		Ref<Framebuffer> m_Framebuffer = nullptr;

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		Ref<Scene> m_Context = nullptr;
		Scope<EditorCameraController> m_Controller = nullptr;
	};
}

