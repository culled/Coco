#pragma once
#include "Core/Base.h"
#include "VertexArray.h"

#include "glm/glm.hpp"

namespace Coco
{
	class COCO_API RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL
		};

		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4 color) = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>&vertexArray, size_t indexCount = 0) = 0;

		static void Create();
		static void Destroy();

		inline static API GetAPI() { return s_API; }
		inline static RendererAPI& GetCurrent() { return *s_CurrentAPI; }
	private:
		static API s_API;
		static RendererAPI* s_CurrentAPI;

		friend class RenderCommand;
	};
}

