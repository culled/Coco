#include "ccpch.h"
#include "SubTextures.h"

namespace Coco
{
  Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture, glm::vec2 position, glm::vec2 cellSize, glm::vec2 tileSize)
  {
    glm::vec2 min = { position.x * cellSize.x / texture->GetWidth(), position.y * cellSize.y / texture->GetHeight() };
    glm::vec2 max = { (position.x + tileSize.x) * cellSize.x / texture->GetWidth(), (position.y + tileSize.y) * cellSize.y / texture->GetHeight() };

    return CreateRef<SubTexture2D>(texture, min, max);
  }

  SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, glm::vec2 min, glm::vec2 max) :
    m_Texture(texture), m_MinCoords(min), m_MaxCoords(max)
  {
  }
}
