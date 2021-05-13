#include "ScenePanel.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "SceneHierarchyPanel.h"
#include "Core/Utils/Math.h"
#include "Core/Events/EventDispatcher.h"

#include <glm/gtc/type_ptr.hpp>

namespace Coco
{
	ScenePanel::ScenePanel()
	{
		m_Framebuffer = Framebuffer::Create(Application::Get().GetMainWindow().GetWidth(), Application::Get().GetMainWindow().GetHeight(), 
			{ FramebufferTextureFormat::RGB24, FramebufferTextureFormat::R32, FramebufferTextureFormat::Depth });

		m_EditorCamera = CreateScope<EditorCamera>(5.0f);
		m_EditorCamera->SetControlEnabled(true);
	}

	ScenePanel::ScenePanel(const Ref<Scene>& scene) : ScenePanel()
	{
		SetContext(scene);
	}

	void ScenePanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
	}

	void ScenePanel::OnUpdate(Timestep timestep)
	{
		if (!m_Context) return;

		if ((m_ViewportSize.x != m_Framebuffer->GetWidth() || m_ViewportSize.y != m_Framebuffer->GetHeight()) &&
			m_ViewportSize.x > 0 && m_ViewportSize.y > 0)
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			float aspect = m_ViewportSize.x / m_ViewportSize.y;

			if (aspect != m_EditorCamera->GetCamera()->GetAspectRatio())
			{
				m_EditorCamera->GetCamera()->SetAspectRatio(aspect);
			}
		}

		m_EditorCamera->SetControlEnabled(m_ViewportFocused);
		m_EditorCamera->SetMouseHovering(m_ViewportHovered);
		m_EditorCamera->OnUpdate(timestep);

		Renderer::ResetStats();

		m_Framebuffer->Bind();
		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		RenderCommand::Clear();

		int clearValue = -1;
		m_Framebuffer->ClearColorAttachment(1, &clearValue);

		//m_Context->Update(timestep);
		m_Context->DrawForCamera(static_cast<Camera>(*m_EditorCamera->GetCamera()), m_EditorCamera->GetTransform());

		if (m_ViewportHovered)
		{
			auto [mx, my] = ImGui::GetMousePos();
			int mouseX = ((int)mx - m_ViewportBounds[0].x);
			int mouseY = ((int)(m_ViewportSize.y - (my - m_ViewportBounds[0].y)));

			int pixel = m_Framebuffer->ReadPixel(1, mouseX, mouseY);

			m_HoveredEntity = pixel == -1 ? Entity() : Entity((entt::entity)pixel, m_Context.get());
		}

		m_Framebuffer->Unbind();
	}

	void ScenePanel::OnImGuiRender()
	{
		if (!m_Context) return;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Scene");

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		double yOffset = ImGui::GetWindowSize().y - viewportSize.y;

		m_ViewportSize = { viewportSize.x, viewportSize.y };
		m_ViewportBounds[0] = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + yOffset };
		m_ViewportBounds[1] = { m_ViewportBounds[0].x + viewportSize.x, m_ViewportBounds[0].y + viewportSize.y };

		ImGui::Image((void*)(uint64_t)m_Framebuffer->GetColorAttachmentID(), { viewportSize.x, viewportSize.y }, { 0.0f, 1.0f }, { 1.0f, 0.0f });

		m_ViewportFocused = ImGui::IsWindowFocused(0);
		m_ViewportHovered = ImGui::IsWindowHovered(0);
		m_CanMousePick = m_ViewportHovered;

		//Snapping
		bool snap = Input::IsKeyPressed(KeyCodes::Left_Control);
		float snapValue = 0.5f;

		if (m_GizmoType == (int)ImGuizmo::OPERATION::ROTATE)
		{
			snapValue = 45.0f;
		}

		//ImGuizmo
		Entity selectedEntity = SceneHierarchyPanel::GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(m_EditorCamera->GetOrthographic());
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, viewportSize.x, viewportSize.y);

			const glm::mat4& cameraProjection = m_EditorCamera->GetCamera()->GetProjectionMatrix();
			glm::mat4 cameraView = glm::inverse(m_EditorCamera->GetTransform());

			auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = transformComponent;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::AllowAxisFlip(false);

			if (ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr))
			{
				glm::vec3 finalRotation;

				Math::DecomposeTransform(transform, transformComponent.Position, finalRotation, transformComponent.Scale);

				glm::vec3 deltaRotation = finalRotation - transformComponent.Rotation;
				transformComponent.Rotation += deltaRotation;
			}

			if (ImGuizmo::IsOver())
				m_CanMousePick = false;
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void ScenePanel::OnEvent(DispatchedEvent& e)
	{
		EventDispatcher::Dispatch<KeyPressEventArgs>(e, this, &ScenePanel::OnKeyPressed);
		EventDispatcher::Dispatch<MouseButtonReleaseEventArgs>(e, this, &ScenePanel::OnMouseButtonReleased);
		m_EditorCamera->OnEvent(e);
	}

	void ScenePanel::OnKeyPressed(KeyPressEventArgs* args)
	{
		if (m_ViewportHovered)
		{
			switch ((KeyCodes)args->Key)
			{
			case KeyCodes::Grave_Accent:
				m_GizmoType = -1;
				break;
			case KeyCodes::D1:
				m_GizmoType = ImGuizmo::TRANSLATE;
				break;
			case KeyCodes::D2:
				m_GizmoType = ImGuizmo::ROTATE;
				break;
			case KeyCodes::D3:
				m_GizmoType = ImGuizmo::SCALE;
				break;
			}
		}
	}

	void ScenePanel::OnMouseButtonReleased(MouseButtonReleaseEventArgs* args)
	{
		if (args->Button == (int)MouseButtons::Button_1 && m_CanMousePick)
		{
			SceneHierarchyPanel::SetSelectedEntity(m_HoveredEntity);
		}
	}
}
