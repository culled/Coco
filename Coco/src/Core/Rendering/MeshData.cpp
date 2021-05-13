#include "ccpch.h"
#include "MeshData.h"

namespace Coco
{
	MeshData::MeshData(bool dynamic) :
		m_Dynamic(dynamic)
	{
		m_VBO = VertexBuffer::Create(0, m_Dynamic ? BufferUpdateType::Dynamic : BufferUpdateType::Static);
		m_VBO->SetLayout({ {ShaderDataType::Float3, "a_Pos"} });

		m_IBO = IndexBuffer::Create(nullptr, 0, m_Dynamic ? BufferUpdateType::Dynamic : BufferUpdateType::Static);
		m_VAO = VertexArray::Create(m_VBO, m_IBO);
	}

	MeshData::MeshData(const glm::vec3* positions, uint32_t positionCount, const uint32_t* indicies, uint32_t indexCount, bool dynamic) :
		MeshData(dynamic)
	{
		SetVertexPositions(positions, positionCount);
		SetIndicies(indicies, indexCount);
	}

	void MeshData::SetVertexPositions(const glm::vec3* positions, uint32_t count)
	{
		m_VertexPositions.resize(count);
		std::copy(positions, positions + count, m_VertexPositions.begin());

		m_VBO->Resize((float*)m_VertexPositions.data(), count * sizeof(glm::vec3));
	}

	void MeshData::SetIndicies(const uint32_t* indicies, uint32_t count)
	{
		m_Indicies.resize(count);
		std::copy(indicies, indicies + count, m_Indicies.begin());

		m_IBO->Resize(indicies, count);

		//For some reason, the IBO gets unbound from the VAO once it changes.
		//This is a quick way to rebind it.
		m_VAO->Bind();
		m_IBO->Bind();
		m_VAO->Unbind();
		m_IBO->Unbind();
	}
}