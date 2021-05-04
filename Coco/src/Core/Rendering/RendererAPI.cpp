#include "ccpch.h"
#include "RendererAPI.h"

#include "Core/App/Log.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Coco
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::None;
	RendererAPI* RendererAPI::s_CurrentAPI = nullptr;

	void RendererAPI::Create()
	{
		//TODO: platform detection
		s_API = API::OpenGL;

		switch (s_API)
		{
		case API::None:
			ASSERT_CORE(false, "Headless rendering isn't supported yet");
			break;

		case API::OpenGL:
			LOG_CORE_INFO("Creating OpenGLRendererAPI");
			s_CurrentAPI = new OpenGLRendererAPI();
			break;
		}
	}

	void RendererAPI::Destroy()
	{
		if (s_CurrentAPI)
		{
			LOG_CORE_INFO("Destroying RendererAPI");
			delete s_CurrentAPI;
		}
	}
}