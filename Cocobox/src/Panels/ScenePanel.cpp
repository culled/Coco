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
		m_Framebuffer = Framebuffer::Create(Application::Get().GetMainWindow().GetWidth(), Application::Get().GetMainWindow().GetHeight());

		m_Controller = CreateScope<EditorCameraController>(CreateRef<SceneCamera>(SceneCamera::Projection::Orthographic, 1.0f));
		m_Controller->SetControlEnabled(true);
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

			if (aspect != m_Controller->GetCamera()->GetAspectRatio())
			{
				m_Controller->GetCamera()->SetAspectRatio(aspect);
			}
		}

		if (m_ViewportFocused)
		{
			m_Controller->OnUpdate(timestep);
		}

		m_Framebuffer->Bind();
		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		RenderCommand::Clear();
		Renderer::ResetStats();

		//m_Context->Update(timestep);
		m_Context->DrawForCamera(static_cast<Camera>(*m_Controller->GetCamera().get()), m_Controller->GetTransform());

		m_Framebuffer->Unbind();
	}

	void ScenePanel::OnImGuiRender()
	{
		if (!m_Context) return;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Scene");

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();

		m_ViewportSize = { viewportSize.x, viewportSize.y };

		ImGui::Image((void*)(uint64_t)m_Framebuffer->GetColorAttachmentID(), { viewportSize.x, viewportSize.y }, { 0.0f, 1.0f }, { 1.0f, 0.0f });

		m_ViewportFocused = ImGui::IsWindowFocused(0);
		m_ViewportHovered = ImGui::IsWindowHovered(0);

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
			ImGuizmo::SetOrthographic(true);
			ImGuizmo::SetDrawlist();

			double yOffset = ImGui::GetWindowSize().y - viewportSize.y;
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + yOffset, viewportSize.x, viewportSize.y);

			const glm::mat4& cameraProjection = m_Controller->GetCamera()->GetProjectionMatrix();
			glm::mat4 cameraView = glm::inverse(m_Controller->GetTransform());

			auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = transformComponent;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			if (ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr))
			{
				glm::vec3 finalRotation;

				Math::DecomposeTransform(transform, transformComponent.Position, finalRotation, transformComponent.Scale);

				glm::vec3 deltaRotation = finalRotation - transformComponent.Rotation;
				transformComponent.Rotation += deltaRotation;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void ScenePanel::OnEvent(DispatchedEvent& e)
	{
		EventDispatcher::Dispatch<KeyPressEventArgs>(e, this, &ScenePanel::OnKeyPressed);

		if (m_ViewportHovered && m_Context)
		{
			m_Controller->OnEvent(e);
		}
	}

	void ScenePanel::OnKeyPressed(KeyPressEventArgs* args)
	{
		if (m_ViewportHovered)
		{
			switch ((KeyCodes)args->Key)
			{
			case KeyCodes::Q:
				m_GizmoType = -1;
				break;
			case KeyCodes::E:
				m_GizmoType = ImGuizmo::TRANSLATE;
				break;
			case KeyCodes::R:
				m_GizmoType = ImGuizmo::ROTATE;
				break;
			case KeyCodes::T:
				m_GizmoType = ImGuizmo::SCALE;
				break;
			}
		}
	}
}
