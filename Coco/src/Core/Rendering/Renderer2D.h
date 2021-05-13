#pragma once
#include "Core/Base.h"
#include "Renderer.h"
#include "Material.h"
#include "VertexArray.h"
#include "SubTextures.h"
#include "Textures.h"

#include <glm/glm.hpp>

namespace Coco
{
	class COCO_API Renderer2D
	{
	public:
		struct COCO_API BatchedQuadVertex
		{
			glm::vec3 Position;
			glm::vec2 TexCoord;
			float ID;
			glm::vec4 Color;
			float TexID;
		};

		struct COCO_API BatchedQuads
		{
			static const uint32_t MaxQuadsPerDrawcall = 10000;
			static const uint32_t MaxVerticiesPerDrawcall = MaxQuadsPerDrawcall * 4;
			static const uint32_t MaxIndiciesPerDrawcall = MaxQuadsPerDrawcall * 6;
			static const uint32_t MaxTextureSlots = 32; //TODO: render capabilities

			Ref<VertexBuffer> VertexBuffer = nullptr;
			Ref<VertexArray> VertexArray = nullptr;

			BatchedQuadVertex* QuadVertexBase = nullptr;
			BatchedQuadVertex* QuadVertexPtr = nullptr;

			uint32_t CurrentIndexCount = 0;

			std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
			uint32_t TextureSlotIndex = 1; //0 = white texture

			glm::vec4 QuadVertexPositions[4];
			glm::vec2 QuadVertexTexCoords[4];

			Ref<Shader> Shader;
		};

		struct COCO_API PrimativeRenderData
		{
			Ref<VertexArray> QuadVertexArray = nullptr;
		};

		static void Init();
		static void Shutdown();

		static void BeginBatch(const Ref<Shader>& shader);
		static void FlushBatch();

		static void SubmitImmediateQuad(const Ref<Material>& material, const glm::mat4& transform);
		static void SubmitBatchedSprite(const glm::mat4& transform, uint32_t id, const Ref<Texture2D>& texture = nullptr, const glm::vec4& color = glm::vec4(1.0f), const glm::vec2& tiling = glm::vec2(1.0f));
		static void SubmitBatchedSubSprite(const glm::mat4& transform, uint32_t id, const Ref<SubTexture2D>& texture = nullptr, const glm::vec4& color = glm::vec4(1.0f), const glm::vec2& tiling = glm::vec2(1.0f));

	private:
		static PrimativeRenderData s_PrimativeData;
		static BatchedQuads s_QuadBatch;

	private:
		static void BatchQuad(const glm::mat4& transform, uint32_t id, uint32_t texIndex, const glm::vec4& color, const glm::vec2& tiling);
	};
}