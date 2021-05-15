#pragma once
#include "Core/Base.h"
#include "Core/Rendering/Buffers.h"

namespace Coco
{
#pragma region VertexBuffer
	class COCO_API OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(size_t size, BufferUpdateType type = BufferUpdateType::Static);
		OpenGLVertexBuffer(float* verticies, size_t size, BufferUpdateType type = BufferUpdateType::Static);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void CopyTo(const Ref<VertexBuffer>& destination, uint32_t offset) override;

		virtual void Resize(const float* verticies, size_t size) override;
		virtual void SetData(const void* data, size_t size, uint32_t offset = 0) override;

		inline virtual void SetLayout(const VertexBufferLayout& layout) override { m_Layout = layout; }
		inline virtual const VertexBufferLayout& GetLayout() override { return m_Layout; }

		virtual size_t GetVertexCount() const override { return m_Size / m_Layout.GetStride(); }

		virtual uint32_t GetID() const override { return m_Id; }
		virtual size_t GetSize() const override { return m_Size; }

	private:
		uint32_t m_Id;
		VertexBufferLayout m_Layout;
		size_t m_Size;
		BufferUpdateType m_Type;
	};
#pragma endregion

#pragma region IndexBuffer
	class COCO_API OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indicies, size_t count, BufferUpdateType type = BufferUpdateType::Static);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		inline virtual size_t GetCount() const override { return m_Count; }

		virtual void SetData(const uint32_t* indicies, size_t count) override;
		virtual void Resize(const uint32_t* indicies, size_t count) override;

	private:
		uint32_t m_Id;
		size_t m_Count;
		BufferUpdateType m_Type;
	};
#pragma endregion

#pragma region UniformBuffer
	class COCO_API OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(size_t size, uint32_t binding, BufferUpdateType type = BufferUpdateType::Static);
		virtual ~OpenGLUniformBuffer();

		virtual void SetData(const void* data, size_t size, uint32_t offset = 0) override;
		virtual uint32_t GetID() const override { return m_Id; }
	private:
		uint32_t m_Id;
	};
#pragma endregion
}