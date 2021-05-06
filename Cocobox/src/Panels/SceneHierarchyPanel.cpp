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

		//Right click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_Context->CreateEntity("Entity");
			}

			ImGui::EndPopup();
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

		bool entityDeleted = false;

		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if (ImGui::MenuItem("Delete"))
			{
				entityDeleted = true;
			}

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			if (s_SelectedEntity == entity)
			{
				s_SelectedEntity = {};
			}

			m_Context->DestroyEntity(entity);
		}
	}
}
