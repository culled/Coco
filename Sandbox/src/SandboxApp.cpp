#include "SandboxApp.h"

//--------Entry Point--------
#include <Core/App/Entrypoint.h>
//--------Entry Point--------

#include <Core/App/Log.h>

SandboxApp::SandboxApp() : Application("Sandbox")
{
	LOG_INFO("Starting...");
}

SandboxApp::~SandboxApp()
{
	LOG_INFO("Shutting down...");
}

Coco::Scope<Coco::Application> Coco::CreateApplication()
{
	return Coco::CreateScope<SandboxApp>();
}
