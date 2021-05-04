#include "ccpch.h"
#include "GLContext.h"

#include "Platform/OpenGL/OpenGLContext.h"
#include "Core/App/Log.h"

namespace Coco
{
	Ref<GLContext> GLContext::s_Context = nullptr;

	void GLContext::Init()
	{
		//TODO: get render api
		LOG_CORE_INFO("Initializing OpenGL context");

		s_Context = CreateRef<OpenGLContext>();
	}
}