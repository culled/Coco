#include "SandboxLayer.h"
#include "Core/App/Log.h"

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
	LOG_TRACE("SandboxLayer::Update: {0}ms", timestep.GetMilliseconds());
}

void SandboxLayer::OnImGuiRender()
{
	LOG_TRACE("SandboxLayer::OnImGuiRender");
}
