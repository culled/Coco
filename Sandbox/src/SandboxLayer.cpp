#include "SandboxLayer.h"
#include "Core/App/Log.h"
#include "imgui.h"
#include "Core/Rendering/RenderCommand.h"

SandboxLayer::SandboxLayer() : Layer("Sandbox")
{
}

void SandboxLayer::OnAttached()
{
	LOG_TRACE("Sandbox layer attached");
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
}

static bool s_DemoOpen = true;
void SandboxLayer::OnImGuiRender()
{
	ImGui::ShowDemoWindow(&s_DemoOpen);
}
