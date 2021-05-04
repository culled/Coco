#include "SandboxApp.h"

//--------Entry Point--------
#include <Core/App/Entrypoint.h>
//--------Entry Point--------

#include "SandboxLayer.h"
#include <Core/App/Log.h>

SandboxApp::SandboxApp() : Application("Sandbox")
{
	LOG_INFO("Starting...");

	PushLayer(std::static_pointer_cast<Coco::Layer>(Coco::CreateRef<SandboxLayer>()));
}

SandboxApp::~SandboxApp()
{
	LOG_INFO("Shutting down...");
}

Coco::Scope<Coco::Application> Coco::CreateApplication()
{
	return Coco::CreateScope<SandboxApp>();
}
