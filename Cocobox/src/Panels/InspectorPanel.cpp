#include "InspectorPanel.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "glm/gtc/type_ptr.hpp"
#include "SceneHierarchyPanel.h"

#include <any>

namespace Coco
{
	void InspectorPanel::OnImGuiRender()
	{
		ImGui::Begin("Inspector");

		Entity selectedEntity = SceneHierarchyPanel::s_SelectedEntity;

		if (selectedEntity)
		{
			DrawEntityComponents(selectedEntity);

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
			ImGui::Separator();

			float x = ImGui::GetCursorPosX();
			ImVec2 textSize = ImGui::CalcTextSize("Add Component");
			ImGui::SetCursorPosX((GImGui->CurrentWindow->WorkRect.GetWidth() - textSize.x) * 0.5f);

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("AddComponent");
			}

			ImGui::SetCursorPosX(x);

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (!selectedEntity.HasComponent<CameraComponent>() && ImGui::MenuItem("Camera"))
				{
					selectedEntity.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (!selectedEntity.HasComponent<MeshRendererComponent>() && ImGui::MenuItem("Mesh Renderer"))
				{
					selectedEntity.AddComponent<MeshRendererComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (!selectedEntity.HasComponent<SpriteRendererComponent>() && ImGui::MenuItem("Sprite Renderer"))
				{
					selectedEntity.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}

	bool InspectorPanel::DrawVec3Control(const char* label, glm::vec3& value, float speed, glm::vec3 resetValue, float labelColumnWidth)
	{
		bool changed = false;

		ImGui::PushID(label);

		bool smallVersion = ImGui::GetWindowWidth() - labelColumnWidth < 215;
			
		if (!smallVersion)
		{
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, labelColumnWidth);
		}

		ImGui::Text(label);

		if (!smallVersion)
		{
			ImGui::NextColumn();
		}

		//From ImGui
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize{ lineHeight + 3.0f, lineHeight };

		ImGui::PushMultiItemsWidths(3, ImGui::GetColumnWidth() - buttonSize.x * 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });

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
		auto& tagComponent = entity.GetComponent<TagComponent>();

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

		DrawComponent<MeshRendererComponent>("Mesh Renderer", entity, [](MeshRendererComponent& mrc) {
			if(mrc.RenderMaterial)
			{
				DrawMaterialProperties(mrc.RenderMaterial);
			}
			});
	}

	void InspectorPanel::DrawMaterialProperties(const Ref<Material>& material)
	{
		ImGui::Text("Material: %s", material->GetName().c_str());

		for (auto& prop : material->GetProperties())
		{
			ImGui::Text(prop.Name.c_str());
			std::string sId = "##" + prop.Name;

			switch (prop.Type)
			{
			case ShaderDataType::Float4:
				glm::vec4 v = std::any_cast<glm::vec4>(prop.Value);		
				ImGui::ColorEdit4(sId.c_str(), glm::value_ptr(v));
				prop.Value = v;
				break;
			case ShaderDataType::Sampler2D:
				bool selectTexture = false;
				if (prop.Value.has_value())
				{
					Ref<Texture2D> tex = std::any_cast<Ref<Texture2D>>(prop.Value);

					if (tex)
					{
						if (ImGui::ImageButton((ImTextureID)tex->GetID(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0)))
						{
							selectTexture = true;
						}
					}
					else
					{
						if (ImGui::Button(sId.c_str(), ImVec2(64, 64)))
						{
							selectTexture = true;
						}
					}
				}
				else
				{
					if (ImGui::Button(sId.c_str(), ImVec2(64, 64)))
					{
						selectTexture = true;
					}
				}

				if(selectTexture)
					//TODO
				break;
			}
		}
	}
}
