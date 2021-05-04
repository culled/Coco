#include "ccpch.h"
#include "ImGuiRenderer.h"

#include "Core/Rendering/RendererAPI.h"
#include "Platform/OpenGL/OpenGLImGui.h"
#include "Core/App/Log.h"

namespace Coco
{
	Scope<ImGuiRenderer> ImGuiRenderer::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT_CORE(false, "Headless rendering isn't supported yet");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateScope<OpenGLImGui>();
		}

		ASSERT_CORE(false, "Invalid API");
		return nullptr;
	}
}
