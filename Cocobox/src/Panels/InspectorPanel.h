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
		static bool DrawVec3Control(const char* label, glm::vec3& value, float speed = 0.1f, glm::vec3 resetValue = glm::vec3(0.0f), float labelColumnWidth = 75.0f);

		void DrawEntityComponents(Entity entity);
		static void DrawMaterialProperties(const Ref<Material>& material);

		template<typename T>
		void DrawComponent(const char* label, Entity entity, void(*DrawFunction)(T&))
		{
			if (!entity.HasComponent<T>()) return;

			float widthAvailable = GImGui->CurrentWindow->WorkRect.GetWidth();
			bool open = ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | 
				ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap |
			ImGuiTreeNodeFlags_FramePadding);

			if (!std::is_same<T, TransformComponent>() && !std::is_same<T, TagComponent>())
			{
				float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

				ImGui::SameLine(widthAvailable - lineHeight * 0.5f);
				if (ImGui::Button("-", ImVec2{ lineHeight, lineHeight }))
				{
					ImGui::OpenPopup("ComponentSettings");
				}
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
				T& component = entity.GetComponent<T>();
				DrawFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				entity.RemoveComponent<T>();
			}
		}
	};
}

