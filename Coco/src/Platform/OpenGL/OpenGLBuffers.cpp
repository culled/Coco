#include "ccpch.h"
#include "OpenGLBuffers.h"

#include "glad/glad.h"

namespace Coco
{
#pragma region VertexBuffer
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) :
		m_Size(size)
	{
		glCreateBuffers(1, &m_Id);
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* verticies, uint32_t size) :
		m_Size(size)
	{
		glCreateBuffers(1, &m_Id);
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_Id);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}
#pragma endregion

#pragma region IndexBuffer
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indicies, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_Id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indicies, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_Id);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
#pragma endregion
}