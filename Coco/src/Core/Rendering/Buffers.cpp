#include "ccpch.h"
#include "Buffers.h"

#include "Core/Rendering/RendererAPI.h"
#include "Platform/OpenGL/OpenGLBuffers.h"

namespace Coco
{
#pragma region VertexBufferLayout
	void VertexBufferLayout::CalculateOffsetsAndStride()
	{
		uint32_t offset = 0;
		m_Stride = 0;
		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}
#pragma endregion

#pragma region VertexBuffer
	Ref<VertexBuffer> VertexBuffer::Create(size_t size, BufferUpdateType type)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT_CORE(false, "No render api");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(size, type);
		}

		ASSERT_CORE(false, "Invalid API");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* verticies, size_t size, BufferUpdateType type)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT_CORE(false, "No render api");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(verticies, size, type);
		}

		ASSERT_CORE(false, "Invalid API");
		return nullptr;
	}
#pragma endregion

#pragma region IndexBuffer
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indicies, size_t count, BufferUpdateType type)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT_CORE(false, "No render api");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indicies, count, type);
		}

		ASSERT_CORE(false, "Invalid API");
		return nullptr;
	}
#pragma endregion

	Ref<UniformBuffer> UniformBuffer::Create(size_t size, uint32_t binding, BufferUpdateType type)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT_CORE(false, "No render api");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLUniformBuffer>(size, binding, type);
		}

		ASSERT_CORE(false, "Invalid API");
		return nullptr;
	}
}