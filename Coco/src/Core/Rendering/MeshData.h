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
		MeshData(bool dynamic = false);
		MeshData(const glm::vec3* positions, uint32_t positionCount, const uint32_t* indicies, uint32_t indexCount, bool dynamic = false);
		virtual ~MeshData() = default;

		void SetVertexPositions(const glm::vec3* positions, uint32_t count);
		void SetIndicies(const uint32_t* indicies, uint32_t count);

		Ref<VertexArray> GetVAO() { return m_VAO; }
		const Ref<VertexArray>& GetVAO() const { return m_VAO; }

	private:
		std::vector<glm::vec3> m_VertexPositions;
		std::vector<uint32_t> m_Indicies;

		bool m_Dynamic = false;

		Ref<VertexArray> m_VAO;
		Ref<VertexBuffer> m_VBO;
		Ref<IndexBuffer> m_IBO;
	};
}

