#pragma once
#include <string>
#include <chrono>

#include "Core/Base.h"
#include "Window.h"
#include "Core/Events/EventArgs.h"
#include "Core/Layers/LayerStack.h"

#if COCO_IMGUI
#include "Core/ImGui/ImGuiLayer.h"
#endif

namespace Coco
{
	class COCO_API Application
	{
	public:
		/*@brief Initializes the core application
		* 		@param name - The name of the application
		*/
		Application(const std::string& name = "Coco App");

		/*@brief Cleans up the core application*/
		virtual ~Application();

		/*@brief Runs the core logic of the application*/
		virtual void Run();

		virtual void OnEvent(DispatchedEvent& e);

		void PushLayer(Ref<Layer>& layer);
		void PushOverlay(Ref<Layer>& overlay);
		void PopLayer(Ref<Layer>& layer);
		void PopOverlay(Ref<Layer>& overlay);

		void Close();

		Window& GetMainWindow() { return *m_MainWindow; }
		static Application& Get() { return *s_Instance; }

	protected:
		void OnResized(ResizedEventArgs* args);
		void OnClosing(ClosingEventArgs* args);
		void OnClosed(ClosedEventArgs* args);
	private:
		Scope<Window> m_MainWindow = nullptr;
		bool m_Running = false;
		bool m_Minimized = false;
		bool m_RunInBackground = false;
		float m_LastFrameTime = 0.0f;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;

		LayerStack m_LayerStack;

#if COCO_IMGUI
		Ref<ImGuiLayer> m_ImGuiLayer = nullptr;
#endif

		static Application* s_Instance;
	};

	//Will be defined in clients
	Scope<Application> CreateApplication();
}

