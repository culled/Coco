#pragma once

#include "Application.h"
#include "Core/Base.h"

#ifdef COCO_PLATFORM_WINDOWS

//Will be defined by the client
extern Coco::Scope<Coco::Application> Coco::CreateApplication();

//Actual entrypoint for the app. Runs the app-specific run() method
int main(int argc, char** argv)
{
#if COCO_DEBUG_MEMORY
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_CHECK_CRT_DF;
	_CrtSetDbgFlag(tmpFlag);
#endif

	//PROFILE_BEGIN("Startup", "CocoProfile-Startup.json");
	auto app = Coco::CreateApplication();
	//PROFILE_END();

	//PROFILE_BEGIN("Runtime", "CocoProfile-Runtime.json");
	app->Run();
	//PROFILE_END();

	//PROFILE_BEGIN("Shutdown", "CocoProfile-Shutdown.json");
	app.reset(nullptr);
	//PROFILE_END();
}

#endif