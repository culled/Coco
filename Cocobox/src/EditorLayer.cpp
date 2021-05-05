#include "EditorLayer.h"
#include "imgui.h"

namespace Coco
{
	EditorLayer::EditorLayer() : Layer("Coco Editor") {}

	void EditorLayer::OnAttached()
	{
		/*m_Tex = Texture2D::Create("assets/textures/HDIcon.png");
		m_AtlasTex = Texture2D::Create("assets/textures/texture_atlas.png", TextureCreationParameters(TextureFilteringMode::Nearest, TextureWrapMode::Clamp));
		m_SubTex = SubTexture2D::CreateFromCoords(m_AtlasTex, glm::vec2(6.0f, 2.0f), glm::vec2(16.0f, 16.0f), glm::vec2(3.0f, 2.0f));

		m_Framebuffer = Framebuffer::Create(Application::Get().GetMainWindow().GetWidth(), Application::Get().GetMainWindow().GetHeight());

		m_ActiveScene = CreateRef<Scene>();

		m_SquareEntity = m_ActiveScene->CreateEntity("Square");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
		m_CameraEntity.AddComponent<CameraComponent>(SceneCamera::Projection::Orthographic, (float)Application::Get().GetMainWindow().GetWidth() / (float)Application::Get().GetMainWindow().GetHeight());
		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<Camera2DController>();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);*/

		m_Framebuffer = Framebuffer::Create(Application::Get().GetMainWindow().GetWidth(), Application::Get().GetMainWindow().GetHeight());

		m_ActiveScene = CreateRef<Scene>();

		m_SceneHierarchy.SetContext(m_ActiveScene);

		m_EditorCameraEntity = m_ActiveScene->CreateEntity("Camera");
		m_EditorCameraEntity.AddComponent<CameraComponent>(SceneCamera::Projection::Orthographic, (float)Application::Get().GetMainWindow().GetWidth() / (float)Application::Get().GetMainWindow().GetHeight());
		
		Ref<Material> mat = CreateRef<Material>(m_Shaders.Load("assets/shaders/FlatColor.glsl"));

		m_SquareEntity = m_ActiveScene->CreateEntity("Square");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(mat, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		float verts[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		Ref<VertexBuffer> quadVBO = VertexBuffer::Create(verts, sizeof(verts));
		quadVBO->SetLayout({ {ShaderDataType::Float3, "a_Pos"} });

		uint32_t indicies[] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> quadIBO = IndexBuffer::Create(indicies, 6);

		Ref<VertexArray> vao = VertexArray::Create(quadVBO, quadIBO);

		m_SquareEntity.AddComponent<MeshDataComponent>(vao);
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

		if (m_ViewportFocused)
		{
			//m_CameraController.OnUpdate(timestep);
		}

		m_Framebuffer->Bind();
		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		RenderCommand::Clear();
		//Renderer2D::ResetStats();

		m_ActiveScene->Update(timestep);

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit", NULL, false)) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		m_SceneHierarchy.OnImGuiRender();
		m_Inspector.OnImGuiRender();

		ImGui::Begin("Stats");

		ImGui::Checkbox("Vsync", &m_Vsync);

		ImGui::Text("Render stats:");

		//Renderer2D::Statistics stats = Renderer2D::GetStats();

		ImGui::Text("%dfps", m_FrameRate);
		//ImGui::Text("Draw calls: %d", stats.DrawCalls);
		//ImGui::Text("Quad count: %d", stats.QuadCount);
		//ImGui::Text("Verticies: %d", stats.GetTotalVertexCount());
		//ImGui::Text("Indicies: %d", stats.GetTotalIndexCount());
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();

		if ((m_ViewportSize.x != viewportSize.x || m_ViewportSize.y != viewportSize.y) && viewportSize.x > 0 && viewportSize.y > 0)
		{
			m_ViewportSize = { viewportSize.x, viewportSize.y };
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			auto& cam = m_EditorCameraEntity.GetComponent<CameraComponent>().Camera;
			float aspect = m_ViewportSize.x / m_ViewportSize.y;

			if (aspect != cam.GetAspectRatio())
			{
				cam.SetAspectRatio(aspect);
			}
		}

		ImGui::Image((void*)(uint64_t)m_Framebuffer->GetColorAttachmentID(), { viewportSize.x, viewportSize.y }, { 0.0f, 1.0f }, { 1.0f, 0.0f });

		m_ViewportFocused = ImGui::IsWindowFocused(0);
		m_ViewportHovered = ImGui::IsWindowHovered(0);

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorLayer::OnEvent(DispatchedEvent& e)
	{
	}
}