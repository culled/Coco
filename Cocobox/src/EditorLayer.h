#pragma once
#include <Coco.h>
#include "glm/glm.hpp"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/ScenePanel.h"
#include "Controllers/EditorCamera.h"

namespace Coco
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttached() override;
		virtual void OnDetached() override;

		virtual void Update(Timestep timestep) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(DispatchedEvent& e) override;

	private:
		ShaderLibrary m_Shaders;

		Ref<Scene> m_ActiveScene = nullptr;
		//Entity m_EditorCameraEntity;
		//Entity m_SquareEntity;

		bool m_Vsync = true;
		int m_FrameRate = 0;

		SceneHierarchyPanel m_SceneHierarchy;
		InspectorPanel m_Inspector;
		ScenePanel m_ScenePanel;

		static const char* s_SceneFileFilter;
	};
}

