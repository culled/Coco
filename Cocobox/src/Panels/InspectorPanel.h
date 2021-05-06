#pragma once
#include <Coco.h>

#include "glm/glm.hpp"

namespace Coco
{
	class InspectorPanel
	{
	public:
		void OnImGuiRender();

	private:
		static bool DrawVec3Control(const char* label, glm::vec3& value, float speed = 0.1f, glm::vec3 resetValue = glm::vec3(0.0f), float columnWidth = 100.0f);

		void DrawEntityComponents(Entity entity);

		template<typename T>
		void DrawComponent(const char* label, Entity entity, void(*DrawFunction)(T&))
		{
			if (!entity.HasComponent<T>()) return;

			bool open = ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen);

			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
			if (ImGui::Button("-", ImVec2{ 20, 20 }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;

			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}

				ImGui::EndPopup();
			}

			if (open)
			{
				DrawFunction(entity.GetComponent<T>());
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				entity.RemoveComponent<T>();
			}
		}
	};
}

