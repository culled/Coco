#include "ccpch.h"
#include "OpenGLBuffers.h"

#include "glad/glad.h"

namespace Coco
{
	static GLenum GetUpdateType(BufferUpdateType type)
	{
		switch (type)
		{
		case BufferUpdateType::Static: return GL_STATIC_DRAW;
		case BufferUpdateType::Dynamic: return GL_DYNAMIC_DRAW;
		case BufferUpdateType::Stream: return GL_STREAM_DRAW;
		}

		ASSERT_CORE(false, "Invalid update type");
	}

#pragma region VertexBuffer
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, BufferUpdateType type) :
		m_Size(size), m_Type(type)
	{
		glCreateBuffers(1, &m_Id);
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GetUpdateType(type));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* verticies, uint32_t size, BufferUpdateType type) :
		m_Size(size), m_Type(type)
	{
		glCreateBuffers(1, &m_Id);
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ARRAY_BUFFER, size, verticies, GetUpdateType(type));
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

	void OpenGLVertexBuffer::CopyTo(const Ref<VertexBuffer>& destination, uint32_t offset)
	{
		glBindBuffer(GL_COPY_READ_BUFFER, m_Id);
		glBindBuffer(GL_COPY_WRITE_BUFFER, destination->GetID());
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, offset, m_Size);
		glBindBuffer(GL_COPY_READ_BUFFER, 0);
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	}

	void OpenGLVertexBuffer::Resize(const float* verticies, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ARRAY_BUFFER, size, verticies, GetUpdateType(m_Type));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		m_Size = size;
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
#pragma endregion

#pragma region IndexBuffer
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indicies, uint32_t count, BufferUpdateType type)
		: m_Count(count), m_Type(type)
	{
		glCreateBuffers(1, &m_Id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indicies, GetUpdateType(type));
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

	void OpenGLIndexBuffer::SetData(const uint32_t* indicies, uint32_t count)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(uint32_t), indicies);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void OpenGLIndexBuffer::Resize(const uint32_t* indicies, uint32_t count)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indicies, GetUpdateType(m_Type));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_Count = count;
	}
#pragma endregion

#pragma region UniformBuffer
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding, BufferUpdateType type)
	{
		glCreateBuffers(1, &m_Id);
		glNamedBufferData(m_Id, size, nullptr, GetUpdateType(type));
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_Id);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_Id);
	}

	void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(m_Id, offset, size, data);
	}
#pragma endregion
}