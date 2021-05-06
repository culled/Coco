#pragma once
#include "Core/Base.h"
#include "Buffers.h"

namespace Coco
{
	class COCO_API VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer> buffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer> buffer) = 0;
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		virtual uint32_t GetVertexCount() const = 0;

		static Ref<VertexArray> Create();
		static Ref<VertexArray> Create(Ref<VertexBuffer>& vertexBuffer, Ref<IndexBuffer>& indexBuffer);
	};
}

