#include "ccpch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Coco {
	static GLenum ShaderDataTypeToOpenGLType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float: return GL_FLOAT;
		case ShaderDataType::Float2: return GL_FLOAT;
		case ShaderDataType::Float3: return GL_FLOAT;
		case ShaderDataType::Float4: return GL_FLOAT;
		case ShaderDataType::Mat3: return GL_FLOAT;
		case ShaderDataType::Mat4: return GL_FLOAT;
		case ShaderDataType::Int: return GL_INT;
		case ShaderDataType::Int2: return GL_INT;
		case ShaderDataType::Int3: return GL_INT;
		case ShaderDataType::Int4: return GL_INT;
		case ShaderDataType::Bool: return GL_BOOL;
		}

		ASSERT_CORE(false, "Unknown shader data type");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_Id);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_Id);
	}

	void OpenGLVertexArray::Bind()
	{
		glBindVertexArray(m_Id);
	}

	void OpenGLVertexArray::Unbind()
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> buffer)
	{
		ASSERT_CORE(buffer->GetLayout().GetElements().size(), "VertexBuffer has no layout elements");

		glBindVertexArray(m_Id);
		buffer->Bind();

		uint32_t index = 0;
		const auto& layout = buffer->GetLayout();

		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)(uint64_t)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(buffer);
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> buffer)
	{
		glBindVertexArray(m_Id);
		buffer->Bind();

		m_IndexBuffer = buffer;
		glBindVertexArray(0);
	}

	size_t OpenGLVertexArray::GetVertexCount() const
	{
		size_t count = 0;

		for (auto& buffer : m_VertexBuffers)
		{
			count += buffer->GetVertexCount();
		}

		return count;
	}
}