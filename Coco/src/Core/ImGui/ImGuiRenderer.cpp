#include "ccpch.h"
#include "ImGuiRenderer.h"

#include "Platform/OpenGL/OpenGLImGui.h"

namespace Coco
{
	Scope<ImGuiRenderer> ImGuiRenderer::Create()
	{
		//TODO: get renderer API
		return CreateScope<OpenGLImGui>();
	}
}
