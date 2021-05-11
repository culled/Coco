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

		static void SetSelectedEntity(Entity entity);
		static Entity GetSelectedEntity() { return s_SelectedEntity; }

	private:
		Ref<Scene> m_Context = nullptr;
		static Entity s_SelectedEntity;

		void DrawEntityNode(Entity entity);

		friend class InspectorPanel;
	};
}

