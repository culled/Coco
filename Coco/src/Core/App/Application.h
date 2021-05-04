#pragma once

#include "Core/Base.h"
#include "Window.h"
#include <string>

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

	private:
		Scope<Window> m_MainWindow = nullptr;
		bool m_Running = false;
	};

	//Will be defined in clients
	Scope<Application> CreateApplication();
}

