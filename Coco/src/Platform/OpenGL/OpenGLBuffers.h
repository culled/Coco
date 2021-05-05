#pragma once
#include "Core/Base.h"
#include "Core/Rendering/Buffers.h"

namespace Coco
{
#pragma region VertexBuffer
	class COCO_API OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* verticies, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		inline virtual void SetLayout(const VertexBufferLayout& layout) override { m_Layout = layout; }
		inline virtual const VertexBufferLayout& GetLayout() override { return m_Layout; }
	private:
		uint32_t m_Id;
		VertexBufferLayout m_Layout;
	};
#pragma endregion

#pragma region IndexBuffer
	class COCO_API OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indicies, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		inline virtual uint32_t GetCount() const override { return m_Count; }

	private:
		uint32_t m_Id;
		uint32_t m_Count;
	};
#pragma endregion
}