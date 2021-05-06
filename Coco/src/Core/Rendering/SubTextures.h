#pragma once
#include "Core/Base.h"

#include "Textures.h"
#include "glm/glm.hpp"

namespace Coco
{
	class COCO_API SubTexture2D
	{
	public:
		static Ref<SubTexture2D> Create(const Ref<Texture2D>& texture, glm::vec2 position, glm::vec2 cellSize, glm::vec2 tileSize = glm::vec2(1.0f));

		SubTexture2D() = default;
		SubTexture2D(const Ref<Texture2D>& texture, glm::vec2 min, glm::vec2 max);

		Ref<Texture2D> GetTexture() const { return m_Texture; }
		glm::vec2 GetMinCoords() const { return m_MinCoords; }
		glm::vec2 GetMaxCoords() const { return m_MaxCoords; }

	private:
		Ref<Texture2D> m_Texture = nullptr;
		glm::vec2 m_MinCoords = glm::vec2(0.0f);
		glm::vec2 m_MaxCoords = glm::vec2(1.0f);
	};
}
