#include "ccpch.h"
#include "Textures.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLTextures.h"

namespace Coco
{
	Ref<Texture2D> Texture2D::Create(const std::string& path, TextureCreationParameters params)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT_CORE(false, "No render api");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path, params);
		}

		ASSERT_CORE(false, "Invalid API");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, TextureFormat format, TextureCreationParameters params)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT_CORE(false, "No render api");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height, format, params);
		}

		ASSERT_CORE(false, "Invalid API");
		return nullptr;
	}
}