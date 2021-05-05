#pragma once
#include <Coco.h>

namespace Coco
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(Ref<Scene> scene);

		void SetContext(Ref<Scene> scene);

		void OnImGuiRender();

	private:
		Ref<Scene> m_Context = nullptr;
		static Entity s_SelectedEntity;

		void DrawEntityNode(Entity entity);

		friend class InspectorPanel;
	};
}

