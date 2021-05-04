#include "ccpch.h"
#include "Application.h"

#include "Log.h"
#include "Core/Graphics/GLContext.h"

#include "Core/Events/EventArgs.h"

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

		m_MainWindow->GetEventDispatcher().Connect([&](DispatchedEvent& e)
			{
				OnEvent(e);
			});

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

	void Application::OnEvent(DispatchedEvent& e)
	{
		EventDispatcher::Dispatch<ClosingEventArgs>(e, this, &Application::OnClosing);
		EventDispatcher::Dispatch<ClosedEventArgs>(e, this, &Application::OnClosed);
	}

	void Application::Close()
	{
		ClosingEventArgs closingArgs;
		OnEvent(DispatchedEvent(&closingArgs));

		if (closingArgs.Close)
		{
			ClosedEventArgs closedArgs;
			OnEvent(DispatchedEvent(&closedArgs));
		}
	}

	void Application::OnClosing(ClosingEventArgs* args)
	{
		LOG_CORE_TRACE("Closing");
	}

	void Application::OnClosed(ClosedEventArgs* args)
	{
		LOG_CORE_TRACE("Closed");

		m_Running = false;
	}
}