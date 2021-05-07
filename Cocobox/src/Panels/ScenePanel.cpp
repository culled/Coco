#include "ScenePanel.h"
#include "imgui.h"

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

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void ScenePanel::OnEvent(DispatchedEvent& e)
	{
		if (m_ViewportHovered && m_Context)
		{
			m_Controller->OnEvent(e);
		}
	}
}
