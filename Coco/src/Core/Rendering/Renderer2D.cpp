#include "ccpch.h"
#include "Renderer2D.h"

#include "Buffers.h"
#include "RenderCommand.h"

namespace Coco
{
	Renderer2D::PrimativeRenderData Renderer2D::s_PrimativeData;
	Renderer2D::BatchedQuads Renderer2D::s_QuadBatch;


	void Renderer2D::Init()
	{
		float verts[] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f
		};

		Ref<VertexBuffer> quadVBO = VertexBuffer::Create(verts, sizeof(verts));
		quadVBO->SetLayout({ 
			{ShaderDataType::Float3, "a_Pos"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Int, "a_ID"}
			});

		uint32_t indicies[] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> quadIBO = IndexBuffer::Create(indicies, 6);

		s_PrimativeData.QuadVertexArray = VertexArray::Create(quadVBO, quadIBO);

		s_QuadBatch.VertexBuffer = VertexBuffer::Create(BatchedQuads::MaxVerticiesPerDrawcall * sizeof(BatchedQuadVertex), BufferUpdateType::Dynamic);
		s_QuadBatch.VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Pos"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float, "a_ID"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float, "a_TexID"}
			});

		uint32_t* batchIndicies = new uint32_t[BatchedQuads::MaxIndiciesPerDrawcall];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < BatchedQuads::MaxIndiciesPerDrawcall; i += 6)
		{
			batchIndicies[i + 0] = offset + 0;
			batchIndicies[i + 1] = offset + 1;
			batchIndicies[i + 2] = offset + 2;

			batchIndicies[i + 3] = offset + 2;
			batchIndicies[i + 4] = offset + 3;
			batchIndicies[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> batchIBO = IndexBuffer::Create(batchIndicies, BatchedQuads::MaxIndiciesPerDrawcall, BufferUpdateType::Dynamic);

		s_QuadBatch.VertexArray = VertexArray::Create(s_QuadBatch.VertexBuffer, batchIBO);
		delete[] batchIndicies;

		s_QuadBatch.QuadVertexPositions[0] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
		s_QuadBatch.QuadVertexPositions[1] = glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f);
		s_QuadBatch.QuadVertexPositions[2] = glm::vec4( 0.5f,  0.5f, 0.0f, 1.0f);
		s_QuadBatch.QuadVertexPositions[3] = glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f);

		s_QuadBatch.QuadVertexTexCoords[0] = glm::vec2(0.0f, 0.0f);
		s_QuadBatch.QuadVertexTexCoords[1] = glm::vec2(1.0f, 0.0f);
		s_QuadBatch.QuadVertexTexCoords[2] = glm::vec2(1.0f, 1.0f);
		s_QuadBatch.QuadVertexTexCoords[3] = glm::vec2(0.0f, 1.0f);

		s_QuadBatch.QuadVertexBase = new BatchedQuadVertex[BatchedQuads::MaxVerticiesPerDrawcall];

		s_QuadBatch.TextureSlots[0] = Renderer::s_WhiteTexture;
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_QuadBatch.QuadVertexBase;
	}

	void Renderer2D::BeginBatch(const Ref<Shader>& shader)
	{
		s_QuadBatch.CurrentIndexCount = 0;
		s_QuadBatch.QuadVertexPtr = s_QuadBatch.QuadVertexBase;
		s_QuadBatch.Shader = shader;
	}

	void Renderer2D::FlushBatch()
	{
		if (s_QuadBatch.CurrentIndexCount == 0) return;

		uint32_t dataSize = (uint32_t)(s_QuadBatch.QuadVertexPtr - s_QuadBatch.QuadVertexBase) * sizeof(BatchedQuadVertex);
		s_QuadBatch.VertexBuffer->SetData(s_QuadBatch.QuadVertexBase, dataSize);

		for (int i = 0; i < s_QuadBatch.TextureSlotIndex; i++)
			s_QuadBatch.TextureSlots[i]->Bind(i);

		s_QuadBatch.Shader->Bind();
		RenderCommand::DrawIndexed(s_QuadBatch.VertexArray, s_QuadBatch.CurrentIndexCount);

		Renderer::s_RenderStats.DrawCalls++;
		Renderer::s_RenderStats.IndiciesDrawn += s_QuadBatch.CurrentIndexCount;
		Renderer::s_RenderStats.VerticiesDrawn += s_QuadBatch.CurrentIndexCount / 6 * 4;
	}

	void Renderer2D::SubmitImmediateQuad(const Ref<Material>& material, const glm::mat4& transform)
	{
		Renderer::SubmitImmediate(s_PrimativeData.QuadVertexArray, material, transform);
	}

	void Renderer2D::SubmitBatchedQuad(const glm::mat4& transform, uint32_t id, const Ref<Texture2D>& texture, const glm::vec4& color, const glm::vec2& tiling)
	{
		if (s_QuadBatch.CurrentIndexCount >= BatchedQuads::MaxIndiciesPerDrawcall)
		{
			FlushBatch();
			BeginBatch(s_QuadBatch.Shader);
		}

		int textureSlot = -1;

		if (texture)
		{
			for (int i = 0; i < s_QuadBatch.TextureSlotIndex; i++)
			{
				if (*texture == *s_QuadBatch.TextureSlots[i])
				{
					textureSlot = i;
					break;
				}
			}

			if (textureSlot == -1)
			{
				if (s_QuadBatch.TextureSlotIndex >= BatchedQuads::MaxTextureSlots)
				{
					FlushBatch();
					BeginBatch(s_QuadBatch.Shader);
				}

				textureSlot = s_QuadBatch.TextureSlotIndex;
				s_QuadBatch.TextureSlots[s_QuadBatch.TextureSlotIndex] = texture;
				s_QuadBatch.TextureSlotIndex++;
			}
		}
		else
		{
			textureSlot = 0;
		}
		
		for (int i = 0; i < 4; i++)
		{
			s_QuadBatch.QuadVertexPtr->Position = transform * s_QuadBatch.QuadVertexPositions[i];
			s_QuadBatch.QuadVertexPtr->TexCoord = s_QuadBatch.QuadVertexTexCoords[i] * tiling;
			s_QuadBatch.QuadVertexPtr->ID = id;
			s_QuadBatch.QuadVertexPtr->Color = color;
			s_QuadBatch.QuadVertexPtr->TexID = (float)textureSlot;
			s_QuadBatch.QuadVertexPtr++;
		}

		s_QuadBatch.CurrentIndexCount += 6;
	}
}