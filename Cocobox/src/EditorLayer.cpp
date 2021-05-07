#include "EditorLayer.h"
#include "imgui.h"

namespace Coco
{
	EditorLayer::EditorLayer() : Layer("Coco Editor") {}

	void EditorLayer::OnAttached()
	{
		m_ActiveScene = CreateRef<Scene>();

		m_SceneHierarchy.SetContext(m_ActiveScene);
		m_ScenePanel.SetContext(m_ActiveScene);

		m_EditorCameraEntity = m_ActiveScene->CreateEntity("Camera");
		m_EditorCameraEntity.AddComponent<CameraComponent>(SceneCamera::Projection::Orthographic, (float)Application::Get().GetMainWindow().GetWidth() / (float)Application::Get().GetMainWindow().GetHeight());

		MaterialLibrary::Create("Flat Color", m_Shaders.Load("assets/shaders/FlatColor.glsl"));

		m_SquareEntity = m_ActiveScene->CreateEntity("Square");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(MaterialLibrary::Get("Flat Color"), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		ImGuiIO& io = ImGui::GetIO();
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Regular.ttf", 16.0f);
	}

	void EditorLayer::OnDetached()
	{
	}

	void EditorLayer::Update(Timestep timestep)
	{
		if (Application::Get().GetMainWindow().GetVSync() != (m_Vsync ? 1 : 0))
		{
			Application::Get().GetMainWindow().SetVSync(m_Vsync ? 1 : 0);
		}

		m_FrameRate = (int)std::lround(1.0f / timestep);

		m_ScenePanel.OnUpdate(timestep);
	}

	void EditorLayer::OnImGuiRender()
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene", NULL, false))
				{

				}

				ImGui::Separator();

				if (ImGui::MenuItem("Open Scene", NULL, false))
				{

				}

				if (ImGui::MenuItem("Save Scene", NULL, false))
				{

				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", NULL, false)) Application::Get().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		m_SceneHierarchy.OnImGuiRender();
		m_Inspector.OnImGuiRender();
		m_ScenePanel.OnImGuiRender();

		ImGui::Begin("Stats");

		ImGui::Checkbox("Vsync", &m_Vsync);

		ImGui::Text("Render stats:");

		RenderStats stats = Renderer::GetStats();

		ImGui::Text("%dfps", m_FrameRate);
		ImGui::Text("Render Time: %.3fms", stats.GetDurationMilliseconds());
		ImGui::Text("Draw calls: %d", stats.DrawCalls);
		ImGui::Text("Verticies: %d", stats.VerticiesDrawn);
		ImGui::Text("Indicies: %d", stats.IndiciesDrawn);
		ImGui::End();
	}

	void EditorLayer::OnEvent(DispatchedEvent& e)
	{
		m_ScenePanel.OnEvent(e);
	}
}