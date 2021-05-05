#include "ccpch.h"
#include "VertexArray.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Coco
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ASSERT_CORE(false, "No render api");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexArray>();
		}

		ASSERT_CORE(false, "Invalid API");
		return nullptr;
	}
	Ref<VertexArray> VertexArray::Create(Ref<VertexBuffer>& vertexBuffer, Ref<IndexBuffer>& indexBuffer)
	{
		Ref<VertexArray> vao = Create();
		vao->AddVertexBuffer(vertexBuffer);
		vao->SetIndexBuffer(indexBuffer);
		return vao;
	}
}