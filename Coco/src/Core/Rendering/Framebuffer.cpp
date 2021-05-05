#include "ccpch.h"
#include "Framebuffer.h"

#include "RendererAPI.h"
#include "Core/App/Log.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Coco
{
	Ref<Framebuffer> Coco::Framebuffer::Create(uint32_t width, uint32_t height, FramebufferCreationProperties creationProperties)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT_CORE(false, "No render api");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFramebuffer>(width, height, creationProperties);
		}

		ASSERT_CORE(false, "Invalid API");
		return nullptr;
	}
}
