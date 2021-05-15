#pragma once
#include "Core/Base.h"
#include "VertexArray.h"
#include "Buffers.h"

#include <glm/glm.hpp>

namespace Coco
{
	class COCO_API MeshData
	{
	public:
		struct COCO_API MeshVertexData
		{
			glm::vec3 Position;
			glm::vec2 TexCoords;
		};

		MeshData(bool dynamic = false);
		MeshData(const MeshVertexData* vertices, size_t vertexCount, const uint32_t* indicies, size_t indexCount, bool dynamic = false);
		virtual ~MeshData() = default;

		void SetVertices(const MeshVertexData* vertices, size_t count);
		size_t GetVertexCount() { return m_Vertices.size(); }
		const MeshVertexData& GetVertex(size_t index) const { return m_Vertices[index]; }

		void SetIndicies(const uint32_t* indicies, size_t count);
		size_t GetIndexCount() { return m_Indicies.size(); }
		uint32_t GetIndex(size_t index) { return m_Indicies[index]; }

		Ref<VertexArray> GetVAO() { return m_VAO; }
		const Ref<VertexArray>& GetVAO() const { return m_VAO; }

		void Apply();

	private:
		std::vector<MeshVertexData> m_Vertices;
		std::vector<uint32_t> m_Indicies;

		bool m_Dynamic = false;

		Ref<VertexArray> m_VAO;
		Ref<VertexBuffer> m_VBO;
		Ref<IndexBuffer> m_IBO;
	};
}
