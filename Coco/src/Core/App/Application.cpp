#include "ccpch.h"
#include "Application.h"

#include "Log.h"
#include "Core/Graphics/GLContext.h"

namespace Coco
{
	Application::Application(const std::string& name)
	{
		Coco::Log::Init();
		LOG_CORE_INFO("Initialized core logger");
		LOG_INFO("Initialized app logger");

		GLContext::Init();

		m_MainWindow = Window::Create(name, 1280, 720);
		m_Running = true;

		LOG_CORE_INFO("Initialization complete");
	}

	Application::~Application()
	{
		LOG_CORE_INFO("Shut down");
	}

	void Application::Run()
	{
		LOG_CORE_INFO("Run");

		while (m_Running)
		{
			m_MainWindow->PollEvents();
		}
	}
}