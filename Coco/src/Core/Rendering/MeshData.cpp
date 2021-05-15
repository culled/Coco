#include "ccpch.h"
#include "MeshData.h"

namespace Coco
{
	MeshData::MeshData(bool dynamic) :
		m_Dynamic(dynamic)
	{
		m_VBO = VertexBuffer::Create(0, m_Dynamic ? BufferUpdateType::Dynamic : BufferUpdateType::Static);
		m_VBO->SetLayout({ 
			{ShaderDataType::Float3, "a_Pos"},
			{ShaderDataType::Float2, "a_TexCoords"}
			});

		m_IBO = IndexBuffer::Create(nullptr, 0, m_Dynamic ? BufferUpdateType::Dynamic : BufferUpdateType::Static);
		m_VAO = VertexArray::Create(m_VBO, m_IBO);
	}

	MeshData::MeshData(const MeshVertexData* vertices, size_t vertexCount, const uint32_t* indicies, size_t indexCount, bool dynamic) :
		MeshData(dynamic)
	{
		SetVertices(vertices, vertexCount);
		SetIndicies(indicies, indexCount);
		Apply();
	}

	void MeshData::SetVertices(const MeshVertexData* vertices, size_t count)
	{
		m_Vertices.resize(count);
		std::copy(vertices, vertices + count, m_Vertices.begin());
	}

	void MeshData::SetIndicies(const uint32_t* indicies, size_t count)
	{
		m_Indicies.resize(count);
		std::copy(indicies, indicies + count, m_Indicies.begin());
	}

	void MeshData::Apply()
	{
		m_VBO->Resize((float*)m_Vertices.data(), m_Vertices.size() * sizeof(MeshVertexData));
		m_IBO->Resize(m_Indicies.data(), m_Indicies.size());

		//For some reason, the IBO gets unbound from the VAO once it changes.
		//This is a quick way to rebind it.
		m_VAO->Bind();
		m_IBO->Bind();
		m_VAO->Unbind();
		m_IBO->Unbind();
	}
}