#include "ccpch.h"
#include "Application.h"

#include "Log.h"

namespace Coco
{
	Application::Application(const std::string& name)
	{
		Coco::Log::Init();
		LOG_CORE_INFO("Initialized core logger");
		LOG_INFO("Initialized app logger");

		LOG_CORE_INFO("Initialization complete");
	}

	Application::~Application()
	{
		LOG_CORE_INFO("Shut down");
	}

	void Application::Run()
	{
		LOG_CORE_INFO("Run");
	}
}