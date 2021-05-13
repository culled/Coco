#include "EditorLayer.h"
#include "imgui.h"
#include "Core/Scene/SceneSerializer.h"
#include "Core/Utils/PlatformUtils.h"
#include "Core/Input/KeyCodes.h"
#include "Core/Components/RenderingComponents.h"

#include <OBJ_Loader.h>

namespace Coco
{
	const char* EditorLayer::s_SceneFileFilter = "Cocobox Scene (*.cocoscene)\0*.cocoscene\0";

	EditorLayer::EditorLayer() : Layer("Coco Editor") {}

	void EditorLayer::OnAttached()
	{
		m_ActiveScene = CreateRef<Scene>();

		m_SceneHierarchy.SetContext(m_ActiveScene);
		m_ScenePanel.SetContext(m_ActiveScene);

		Ref<Material> mat = MaterialLibrary::Create("Flat Color", m_Shaders.Load("assets/shaders/FlatColor.glsl"));

		ShaderLibrary::Load("assets/shaders/FlatColorBatched.glsl");

		mat->SetLayout(1, { {"ID", ShaderDataType::Int},
												{"Color", ShaderDataType::Float4} });

		ImGuiIO& io = ImGui::GetIO();
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Regular.ttf", 16.0f);


		objl::Loader Loader;

		if (Loader.LoadFile("assets/models/Ferrari.obj"))
		{
			for (size_t i = 0; i < Loader.LoadedMeshes.size(); i++)
			{
				objl::Mesh mesh = Loader.LoadedMeshes[i];

				Entity meshEntity = m_ActiveScene->CreateEntity(mesh.MeshName);

				glm::vec3* positions = new glm::vec3[mesh.Vertices.size()];

				for (size_t p = 0; p < mesh.Vertices.size(); p++)
				{
					positions[p] = glm::vec3(mesh.Vertices[p].Position.X * 0.01f, mesh.Vertices[p].Position.Y * 0.01f, mesh.Vertices[p].Position.Z * 0.01f);
				}

				Ref<MeshData> meshData = CreateRef<MeshData>(positions, mesh.Vertices.size(), mesh.Indices.data(), mesh.Indices.size());
				meshEntity.AddComponent<MeshRendererComponent>(meshData);

				delete[] positions;
			}
		}
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
				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
				{
					m_ActiveScene = CreateRef<Scene>();

					m_SceneHierarchy.SetContext(m_ActiveScene);
					m_ScenePanel.SetContext(m_ActiveScene);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))
				{
					std::string filePath = FileDialog::OpenFile(s_SceneFileFilter);

					if (!filePath.empty())
					{
						m_ActiveScene = CreateRef<Scene>();

						m_SceneHierarchy.SetContext(m_ActiveScene);
						m_ScenePanel.SetContext(m_ActiveScene);

						SceneSerializer serializer(m_ActiveScene);
						serializer.Deserialize(filePath);
					}
				}

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
				{
				}

				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
				{
					std::string filePath = FileDialog::SaveFile(s_SceneFileFilter);

					if (!filePath.empty())
					{
						SceneSerializer serializer(m_ActiveScene);
						serializer.Serialize(filePath);
					}
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "Ctrl+W")) Application::Get().Close();

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