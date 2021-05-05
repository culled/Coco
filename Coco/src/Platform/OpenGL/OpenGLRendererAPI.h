#pragma once
#include "Core/Base.h"
#include "Core/Rendering/RendererAPI.h"

namespace Coco
{
  class COCO_API OpenGLRendererAPI : public RendererAPI
  {
  public:
		virtual ~OpenGLRendererAPI();

		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void Clear() override;
		virtual void SetClearColor(const glm::vec4 color) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) override;
  };
}

