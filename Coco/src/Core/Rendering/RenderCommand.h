#pragma once
#include "Core/Base.h"

#include "RendererAPI.h"

namespace Coco
{
	class COCO_API RenderCommand
	{
	public:
		inline static void Init()
		{
			RendererAPI::s_CurrentAPI->Init();
		}

		inline static void Shutdown()
		{
			RendererAPI::s_CurrentAPI->Shutdown();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			RendererAPI::s_CurrentAPI->SetViewport(x, y, width, height);
		}

		inline static void Clear()
		{
			RendererAPI::s_CurrentAPI->Clear();
		}

		inline static void SetClearColor(const glm::vec4 color)
		{
			RendererAPI::s_CurrentAPI->SetClearColor(color);
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, size_t indexCount = 0)
		{
			RendererAPI::s_CurrentAPI->DrawIndexed(vertexArray, indexCount);
		}
	};
}