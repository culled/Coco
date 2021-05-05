#include "InspectorPanel.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "glm/gtc/type_ptr.hpp"
#include "SceneHierarchyPanel.h"

namespace Coco
{
	void InspectorPanel::OnImGuiRender()
	{
		ImGui::Begin("Inspector");

		if (SceneHierarchyPanel::s_SelectedEntity)
		{
			DrawEntityComponents(SceneHierarchyPanel::s_SelectedEntity);
		}

		ImGui::End();
	}

	bool InspectorPanel::DrawVec3Control(const char* label, glm::vec3& value, float speed, glm::vec3 resetValue, float columnWidth)
	{
		bool changed = false;

		ImGui::PushID(label);
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);

		ImGui::Text(label);

		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });

		//From ImGui
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize{ lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		if (ImGui::Button("X", buttonSize))
		{
			value.x = resetValue.x;
			changed = true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();

		if (ImGui::DragFloat("##X", &value.x, speed, 0.0f, 0.0f, "%.2f"))
		{
			changed = true;
		}

		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.6f, 0.25f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.7f, 0.35f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.6f, 0.25f, 1.0f));
		if (ImGui::Button("Y", buttonSize))
		{
			value.y = resetValue.y;
			changed = true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();

		if (ImGui::DragFloat("##Y", &value.y, speed, 0.0f, 0.0f, "%.2f"))
		{
			changed = true;
		}

		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		if (ImGui::Button("Z", buttonSize))
		{
			value.z = resetValue.z;
			changed = true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();

		if (ImGui::DragFloat("##Z", &value.z, speed, 0.0f, 0.0f, "%.2f"))
		{
			changed = true;
		}

		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		return changed;
	}

	void InspectorPanel::DrawEntityComponents(Entity entity)
	{
		DrawComponent<TagComponent>("Entity", entity, [](TagComponent& tagComponent) {
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tagComponent.Name.c_str());

			if (ImGui::InputText("Name", buffer, sizeof(buffer)))
			{
				tagComponent.Name = std::string(buffer);
			}

			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tagComponent.Tags.c_str());

			if (ImGui::InputText("Tags", buffer, sizeof(buffer)))
			{
				tagComponent.Tags = std::string(buffer);
			}
			});

		DrawComponent<TransformComponent>("Transform", entity, [](TransformComponent& transformComponent) {
			DrawVec3Control("Position", transformComponent.Position);

			glm::vec3 rot = { glm::degrees(transformComponent.Rotation.x), glm::degrees(transformComponent.Rotation.y), glm::degrees(transformComponent.Rotation.z) };
			if (DrawVec3Control("Rotation", rot))
			{
				transformComponent.Rotation = { glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z) };
			}

			DrawVec3Control("Scale", transformComponent.Scale, 0.1f, glm::vec3(1.0f));
			});

		DrawComponent<CameraComponent>("Camera", entity, [](CameraComponent& cameraComponent) {
			auto& camera = cameraComponent.Camera;

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

			if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];

					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::Projection)i);
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::Projection::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();
				float orthoNear = camera.GetNearPlane();
				float orthoFar = camera.GetFarPlane();
				if (ImGui::DragFloat("Size", &orthoSize, 0.1f))
				{
					camera.SetOrthographicSize(orthoSize);
				}

				if (ImGui::DragFloat("Near Clip", &orthoNear, 0.1f))
				{
					camera.SetNearPlane(orthoNear);
				}

				if (ImGui::DragFloat("Far Clip", &orthoFar, 0.1f))
				{
					camera.SetFarPlane(orthoFar);
				}
			}
			else
			{
				float fov = camera.GetPerspectiveVerticalFOV();
				float nearPlane = camera.GetNearPlane();
				float farPlane = camera.GetFarPlane();
				if (ImGui::DragFloat("Vertical FOV", &fov, 0.1f))
				{
					camera.SetPerspectiveVerticalFOV(fov);
				}

				if (ImGui::DragFloat("Near Clip", &nearPlane, 0.1f))
				{
					camera.SetNearPlane(nearPlane);
				}

				if (ImGui::DragFloat("Far Clip", &farPlane, 0.1f))
				{
					camera.SetFarPlane(farPlane);
				}
			}
			});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](SpriteRendererComponent& spriteRendererComponent) {
			ImGui::ColorEdit4("Color", glm::value_ptr(spriteRendererComponent.Color));
			});
	}
}
