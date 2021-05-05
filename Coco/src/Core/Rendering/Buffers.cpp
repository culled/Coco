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
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT_CORE(false, "No render api");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(size);
		}

		ASSERT_CORE(false, "Invalid API");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* verticies, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT_CORE(false, "No render api");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(verticies, size);
		}

		ASSERT_CORE(false, "Invalid API");
		return nullptr;
	}
#pragma endregion

#pragma region IndexBuffer
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indicies, uint32_t count)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT_CORE(false, "No render api");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indicies, count);
		}

		ASSERT_CORE(false, "Invalid API");
		return nullptr;
	}
#pragma endregion
}