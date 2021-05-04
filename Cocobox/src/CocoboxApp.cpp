#include "CocoboxApp.h"

//--------Entry Point--------
#include <Core/App/Entrypoint.h>
//--------Entry Point--------

namespace Coco
{
	CocoboxApp::CocoboxApp() : Application("Cocobox")
	{
	}

	CocoboxApp::~CocoboxApp()
	{
	}

	Scope<Application> CreateApplication()
	{
		return CreateScope<CocoboxApp>();
	}
}
