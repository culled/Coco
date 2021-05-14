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
	BatchRenderData Renderer::s_BatchData;

	void Renderer::Init()
	{
		MaterialLibrary::Create("Error", ShaderLibrary::Load("assets/shaders/Error.glsl"));

		s_SceneData.SceneDataBuffer = UniformBuffer::Create(sizeof(ShaderSceneData), 0);
		s_SceneData.ModelDataBuffer = UniformBuffer::Create(sizeof(ShaderModelData), 1);

		s_WhiteTexture = Texture2D::Create(1, 1, TextureFormat::RGBA32);
		uint32_t whiteTextureData = 0xffffffff;
		s_WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_BatchData.VertexBuffer = VertexBuffer::Create(BatchRenderData::MaxVerticiesPerDrawcall * sizeof(BatchedVertex), BufferUpdateType::Dynamic);
		s_BatchData.VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Pos"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float, "a_ID"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float, "a_TexID"}
			});

		uint32_t* batchIndicies = new uint32_t[BatchRenderData::MaxIndiciesPerDrawcall];

		s_BatchData.IndexBuffer = IndexBuffer::Create(batchIndicies, BatchRenderData::MaxIndiciesPerDrawcall, BufferUpdateType::Dynamic);

		s_BatchData.VertexArray = VertexArray::Create(s_BatchData.VertexBuffer, s_BatchData.IndexBuffer);
		delete[] batchIndicies;
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& cameraTransform)
	{
		s_SceneData.SceneData.ViewProjectionMatrix = camera.GetProjectionMatrix() * glm::inverse(cameraTransform);
		s_SceneData.SceneDataBuffer->SetData(&s_SceneData.SceneData, sizeof(ShaderSceneData));

		s_RenderStats.StartTimer();
	}

	void Renderer::EndScene()
	{
		s_RenderStats.EndTimer();
	}

	void Renderer::SubmitImmediate(Ref<VertexArray> vao, Ref<Material> material, const glm::mat4& transform, int32_t ID)
	{
		s_SceneData.ModelData.ModelMatrix = transform;
		s_SceneData.ModelData.ID = (float)ID;
		s_SceneData.ModelDataBuffer->SetData(&s_SceneData.ModelData, sizeof(ShaderModelData));

		if (!material)
		{
			MaterialLibrary::Get("Error")->Bind();
		}
		else
		{
			material->Bind();
		}

		vao->Bind();
		RenderCommand::DrawIndexed(vao);
		vao->Unbind();

		s_RenderStats.DrawCalls++;
		s_RenderStats.IndiciesDrawn += vao->GetIndexBuffer()->GetCount();
		s_RenderStats.VerticiesDrawn += vao->GetVertexCount();
	}

	void Renderer::SubmitMeshImmediate(const Ref<MeshData>& meshData, const Ref<Material>& material, const glm::mat4& transform, int32_t ID)
	{
		s_SceneData.ModelData.ModelMatrix = transform;
		s_SceneData.ModelData.ID = (float)ID;
		s_SceneData.ModelDataBuffer->SetData(&s_SceneData.ModelData, sizeof(ShaderModelData));

		if (!material)
		{
			MaterialLibrary::Get("Error")->Bind();
		}
		else
		{
			material->Bind();
		}

		meshData->GetVAO()->Bind();
		RenderCommand::DrawIndexed(meshData->GetVAO());
		meshData->GetVAO()->Unbind();

		s_RenderStats.DrawCalls++;
		s_RenderStats.IndiciesDrawn += meshData->GetVAO()->GetIndexBuffer()->GetCount();
		s_RenderStats.VerticiesDrawn += meshData->GetVAO()->GetVertexCount();
	}

	void Renderer::SubmitMeshBatched(const Ref<MeshData>& meshData, const glm::mat4& transform, int32_t ID)
	{
		if (s_BatchData.CurrentVertexCount + meshData->GetVertexCount() >= BatchRenderData::MaxVerticiesPerDrawcall ||
			s_BatchData.CurrentIndexCount + meshData->GetIndexCount() >= BatchRenderData::MaxIndiciesPerDrawcall)
		{
			FlushBatch();
			BeginBatch(s_BatchData.Material);
		}

		for (uint32_t i = 0; i < meshData->GetVertexCount(); i++)
		{
			const auto& vertex = meshData->GetVertex(i);
			s_BatchData.VertexPtr->Position = transform * glm::vec4(vertex.Position, 1.0f);
			s_BatchData.VertexPtr->TexCoord = vertex.TexCoords;
			s_BatchData.VertexPtr->ID = ID;
			s_BatchData.VertexPtr->Color = glm::vec4(0.8f, 0.1f, 0.2f, 1.0f);
			s_BatchData.VertexPtr->TexID = 0.0f;
			s_BatchData.VertexPtr++;
		}

		s_BatchData.CurrentVertexCount += meshData->GetVertexCount();

		for (uint32_t i = 0; i < meshData->GetIndexCount(); i++)
		{
			s_BatchData.Indicies[i + s_BatchData.CurrentIndexCount] = meshData->GetIndex(i);
		}

		s_BatchData.CurrentIndexCount += meshData->GetIndexCount();
	}

	void Renderer::ResetStats()
	{
		s_RenderStats = RenderStats();
	}

	void Renderer::BeginBatch(const Ref<Material>& material)
	{
		s_BatchData.CurrentIndexCount = 0;
		s_BatchData.CurrentVertexCount = 0;
		s_BatchData.VertexPtr = s_BatchData.VertexBase.data();
		s_BatchData.Material = material;
	}

	void Renderer::FlushBatch()
	{
		if (s_BatchData.CurrentIndexCount == 0) return;

		uint32_t dataSize = s_BatchData.CurrentVertexCount * sizeof(BatchedVertex);
		s_BatchData.VertexBuffer->SetData(s_BatchData.VertexBase.data(), dataSize);
		s_BatchData.IndexBuffer->SetData(s_BatchData.Indicies.data(), s_BatchData.CurrentIndexCount);

		//for (int i = 0; i < s_QuadBatch.TextureSlotIndex; i++)
		//	s_QuadBatch.TextureSlots[i]->Bind(i);

		s_BatchData.Material->Bind();
		s_BatchData.VertexArray->Bind();

		//For some reason, the IBO gets unbound from the VAO once it changes.
		//This is a quick way to rebind it.
		s_BatchData.IndexBuffer->Bind();
		RenderCommand::DrawIndexed(s_BatchData.VertexArray, s_BatchData.CurrentIndexCount);
		s_BatchData.VertexArray->Unbind();

		s_RenderStats.DrawCalls++;
		s_RenderStats.IndiciesDrawn += s_BatchData.CurrentIndexCount;
		s_RenderStats.VerticiesDrawn += s_BatchData.CurrentVertexCount;
	}
}