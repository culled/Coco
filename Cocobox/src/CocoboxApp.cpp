#include "CocoboxApp.h"

//--------Entry Point--------
#include <Core/App/Entrypoint.h>
//--------Entry Point--------

#include "EditorLayer.h"

namespace Coco
{
	CocoboxApp::CocoboxApp() : Application("Cocobox")
	{
		PushLayer(std::static_pointer_cast<Layer>(std::make_shared<EditorLayer>()));
	}

	CocoboxApp::~CocoboxApp()
	{
	}

	Scope<Application> CreateApplication()
	{
		return CreateScope<CocoboxApp>();
	}
}
