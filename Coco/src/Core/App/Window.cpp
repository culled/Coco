#include "ccpch.h"
#include "Window.h"

#include "Platform/OpenGL/GLFWWindow.h"

namespace Coco
{
	Scope<Window> Window::Create(const std::string& title, const uint32_t& width, const uint32_t& height)
	{
#ifdef COCO_PLATFORM_WINDOWS
		return CreateScope<GLFWWindow>(title, width, height);
#endif
	}
}