#include "ccpch.h"
#include "Renderer.h"

#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Coco
{
	SceneData Renderer::s_SceneData;
	RenderStats Renderer::s_RenderStats;
	Ref<Texture2D> Renderer::s_WhiteTexture = nullptr;
	//Renderer::BatchMap Renderer::s_BatchedData;

	void Renderer::Init()
	{
		s_SceneData.TransformBuffer = UniformBuffer::Create(sizeof(TransformData), 0);

		s_WhiteTexture = Texture2D::Create(1, 1, TextureFormat::RGBA32);
		uint32_t whiteTextureData = 0xffffffff;
		s_WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& cameraTransform)
	{
		glm::mat4 vp = camera.GetProjectionMatrix() * glm::inverse(cameraTransform);
		s_SceneData.TransformBuffer->SetData(glm::value_ptr(vp), sizeof(TransformData));

		s_RenderStats.StartTimer();

		BeginBatch();
	}

	void Renderer::EndScene()
	{
		s_RenderStats.EndTimer();
	}

	void Renderer::SubmitBatched(Ref<VertexArray> vao, Ref<Material> material, const glm::mat4& transform)
	{
		/*BatchMapIterator it = std::find_if(s_BatchedData.begin(), s_BatchedData.end(), [&](const Ref<Material>& mat) {
			return *mat.get() == *material.get();
			});

		BatchRenderData* data = nullptr;

		if (it == s_BatchedData.end())
		{
			//This material is different, so we need a new batch
			data = CreateBatch(material);
		}
		else
		{
			data = &(it->second);
		}


		for (const auto& buffer : vao->GetVertexBuffers())
		{
			buffer->CopyTo(data->VertexBuffer, data->BufferSize);
			data->BufferSize += buffer->GetSize();
		}*/
		/*for (int i = 0; i < vao->GetVertexCount(); i++)
		{
			s_BatchData.VertexBufferPtr->Position = transform * vao->Get;
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexIndex = 0;
			s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.QuadVertexBufferPtr->TexCoord = s_Data.QuadTexCoords[i];
			s_Data.QuadVertexBufferPtr++;
		}*/
	}

	void Renderer::SubmitImmediate(Ref<VertexArray> vao, Ref<Material> material, const glm::mat4& transform)
	{
		s_SceneData.TransformBuffer->SetData(glm::value_ptr(transform), sizeof(glm::mat4), sizeof(glm::mat4));
		material->Use();

		RenderCommand::DrawIndexed(vao);

		s_RenderStats.DrawCalls++;
		s_RenderStats.IndiciesDrawn += vao->GetIndexBuffer()->GetCount();
		s_RenderStats.VerticiesDrawn += vao->GetVertexCount();
	}

	void Renderer::ResetStats()
	{
		s_RenderStats = RenderStats();
	}

	void Renderer::BeginBatch()
	{
		//s_BatchedData.clear();
	}

	void Renderer::FlushBatch()
	{
	}

	/*BatchRenderData* Renderer::CreateBatch(const Ref<Material>& material)
	{
		BatchRenderData data;

		data.VertexBuffer = VertexBuffer::Create(BatchRenderData::MaxVerticiesPerDrawcall * sizeof(BatchVertex), BufferUpdateType::Dynamic);
		data.VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Pos"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float, "a_ID"}
			});

		uint32_t* batchIndicies = new uint32_t[BatchRenderData::MaxIndiciesPerDrawcall];
		Ref<IndexBuffer> batchIBO = IndexBuffer::Create(batchIndicies, BatchRenderData::MaxIndiciesPerDrawcall, BufferUpdateType::Dynamic);

		data.VertexArray = VertexArray::Create(data.VertexBuffer, batchIBO);
		delete[] batchIndicies;

		s_BatchedData[material] = data;
		return &s_BatchedData[material];
	}*/
}