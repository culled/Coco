#include "ccpch.h"
#include "GLContext.h"

#include "Platform/OpenGL/OpenGLContext.h"
#include "Core/App/Log.h"
#include "Core/Rendering/RendererAPI.h"

namespace Coco
{
	Ref<GLContext> GLContext::s_Context = nullptr;

	void GLContext::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			break;
		case RendererAPI::API::OpenGL:
			LOG_CORE_INFO("Creating OpenGL context");
			s_Context = CreateRef<OpenGLContext>();
			break;
		}
	}
}