#include "SandboxLayer.h"
#include <Coco.h>
#include "imgui.h"

SandboxLayer::SandboxLayer() : Layer("Sandbox")
{
}

void SandboxLayer::OnAttached()
{
	LOG_TRACE("Sandbox layer attached");

	using namespace Coco;

	m_Shader = Shader::CreateFromFile("assets/shaders/FlatColor.glsl");

	Ref<Material> mat = CreateRef<Material>(m_Shader);
	mat->SetVector4("u_Color", glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));

	m_ActiveScene = CreateRef<Scene>();

	m_Camera = m_ActiveScene->CreateEntity("Camera");
	m_Camera.AddComponent<CameraComponent>(SceneCamera::Projection::Orthographic, 1280.0f / 720.0f);

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

	Entity quadEntity = m_ActiveScene->CreateEntity("Quad");
	quadEntity.AddComponent<MeshDataComponent>(vao);
	quadEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
}

void SandboxLayer::OnDetached()
{
	LOG_TRACE("Sandbox layer detached");
}

void SandboxLayer::OnEvent(Coco::DispatchedEvent& e)
{
	LOG_TRACE("SandboxLayer::OnEvent: {0}", e.Args->ToString());
}

void SandboxLayer::Update(Coco::Timestep timestep)
{
	Coco::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.15f, 1.0f));
	Coco::RenderCommand::Clear();

	auto& app = Coco::Application::Get();
	uint32_t viewportWidth = app.GetMainWindow().GetWidth();
	uint32_t viewportHeight = app.GetMainWindow().GetHeight();
	Coco::RenderCommand::SetViewport(0, 0, viewportWidth, viewportHeight);

	m_Camera.GetComponent<Coco::CameraComponent>().Camera.SetAspectRatio((float)viewportWidth / (float)viewportHeight);

	m_ActiveScene->Update(timestep);
}

void SandboxLayer::OnImGuiRender()
{
}
