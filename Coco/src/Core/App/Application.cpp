#include "ccpch.h"
#include "Application.h"

#include "Log.h"
#include "Core/Graphics/GLContext.h"

#include "Core/Events/EventArgs.h"
#include "Core/Timing/Timestep.h"
#include "Core/Rendering/RendererAPI.h"
#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/Renderer2D.h"

namespace Coco
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		ASSERT_CORE(!s_Instance, "Appliction already exists");

		s_Instance = this;

		Coco::Log::Init();
		LOG_CORE_INFO("Initialized core logger");
		LOG_INFO("Initialized app logger");

		RendererAPI::Create();
		GLContext::Create();

		m_MainWindow = Window::Create(name, 1280, 720);
		m_Running = true;

		m_MainWindow->GetEventDispatcher().Connect([&](DispatchedEvent& e)
			{
				OnEvent(e);
			});

		m_StartTime = std::chrono::high_resolution_clock::now();

		RendererAPI::GetCurrent().Init();
		Renderer::Init();
		Renderer2D::Init();

#if COCO_IMGUI
		m_ImGuiLayer = CreateRef<ImGuiLayer>();
		m_LayerStack.PushOverlay(m_ImGuiLayer);
#endif

		LOG_CORE_INFO("Initialization complete");
	}

	Application::~Application()
	{
		LOG_CORE_INFO("Shut down");
		Renderer2D::Shutdown();
		Renderer::Shutdown();
		RendererAPI::Destroy();
	}

	void Application::Run()
	{
		LOG_CORE_INFO("Run");

		while (m_Running)
		{
			m_MainWindow->PollEvents();

			auto currentTimepoint = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> diff = currentTimepoint - m_StartTime;
			float time = diff.count();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (m_RunInBackground || !m_Minimized)
			{
				for (Ref<Layer>& layer : m_LayerStack)
				{
					layer->Update(timestep);
				}
			}

#ifdef COCO_IMGUI
			m_ImGuiLayer->Begin();
			for (Ref<Layer> layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();
#endif

			m_MainWindow->SwapBuffers();
		}
	}

	void Application::OnEvent(DispatchedEvent& e)
	{
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); it++)
		{
			(*it)->OnEvent(e);

			if (e.Args->Handled)
				break;
		}

		EventDispatcher::Dispatch<ResizedEventArgs>(e, this, &Application::OnResized);
		EventDispatcher::Dispatch<ClosingEventArgs>(e, this, &Application::OnClosing);
		EventDispatcher::Dispatch<ClosedEventArgs>(e, this, &Application::OnClosed);
	}

	void Application::PushLayer(Ref<Layer>& layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Ref<Layer>& overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::PopLayer(Ref<Layer>& layer)
	{
		m_LayerStack.PopLayer(layer);
	}

	void Application::PopOverlay(Ref<Layer>& overlay)
	{
		m_LayerStack.PopOverlay(overlay);
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

	void Application::OnResized(ResizedEventArgs* args)
	{
		m_Minimized = args->Height == 0 || args->Width == 0;

		if (!m_Minimized)
		{
			//Renderer::OnWindowResized(args->newWidth, args->newHeight);
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