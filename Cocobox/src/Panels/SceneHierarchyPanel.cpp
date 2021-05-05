#include "SceneHierarchyPanel.h"

#include "imgui.h"

namespace Coco
{
	Entity SceneHierarchyPanel::s_SelectedEntity;

	SceneHierarchyPanel::SceneHierarchyPanel(Ref<Scene> scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(Ref<Scene> scene)
	{
		m_Context = scene;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		if (ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen))
		{
			m_Context->GetRegistry().each([&](const entt::entity entityID) {
				Entity entity(entityID, m_Context.get());

				DrawEntityNode(entity);
				});

			ImGui::TreePop();
		}

		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			s_SelectedEntity = {};
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& name = entity.GetComponent<TagComponent>().Name;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | (s_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0);

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());

		if (ImGui::IsItemClicked())
		{
			s_SelectedEntity = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}
}
