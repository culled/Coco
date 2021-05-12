#include "ccpch.h"
#include "Renderer.h"

#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Coco
{
	SceneData Renderer::s_SceneData;
	RenderStats Renderer::s_RenderStats;
	PrimativeRenderData Renderer::s_PrimativeData;
	BatchRenderData Renderer::s_BatchData;

	void Renderer::Init()
	{
		float verts[] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
		};

		Ref<VertexBuffer> quadVBO = VertexBuffer::Create(verts, sizeof(verts));
		quadVBO->SetLayout({ {ShaderDataType::Float3, "a_Pos"},
			{ShaderDataType::Float4, "a_Color"} });

		uint32_t indicies[] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> quadIBO = IndexBuffer::Create(indicies, 6);

		s_PrimativeData.QuadVertexArray = VertexArray::Create(quadVBO, quadIBO);

		s_SceneData.CameraBuffer = UniformBuffer::Create(sizeof(SceneData::CameraData), 0);
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& cameraTransform)
	{
		glm::mat4 vp = camera.GetProjectionMatrix() * glm::inverse(cameraTransform);
		s_SceneData.CameraBuffer->SetData(glm::value_ptr(vp), sizeof(SceneData::CameraData));

		s_RenderStats.StartTimer();

		BeginBatch();
	}

	void Renderer::EndScene()
	{
		s_RenderStats.EndTimer();
	}

	void Renderer::SubmitBatched(Ref<VertexArray> vao, Ref<Material> material, const glm::mat4& transform)
	{
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
		//material->SetMatrix4("u_Model.ModelMatrix", transform);
		material->Use();

		RenderCommand::DrawIndexed(vao);

		s_RenderStats.DrawCalls++;
		s_RenderStats.IndiciesDrawn += vao->GetIndexBuffer()->GetCount();
		s_RenderStats.VerticiesDrawn += vao->GetVertexCount();
	}

	void Renderer::SubmitImmediateQuad(Ref<Material> material, const glm::mat4& transform)
	{
		s_SceneData.CameraBuffer->SetData(glm::value_ptr(transform), sizeof(glm::mat4), sizeof(glm::mat4));
		/*SceneData::TransformData data;
		data.ModelMatrix = transform;
		data.ID = 0;
		data.Color = glm::vec4(0.8f, 0.1f, 0.8f, 1.0f);

		s_SceneData.TransformBuffer->SetData(&data, sizeof(SceneData::TransformData));
		material->GetShader()->BindBuffer(s_SceneData.TransformBuffer, 1);*/
		material->Use();

		RenderCommand::DrawIndexed(s_PrimativeData.QuadVertexArray);

		s_RenderStats.DrawCalls++;
		s_RenderStats.IndiciesDrawn += s_PrimativeData.QuadVertexArray->GetIndexBuffer()->GetCount();
		s_RenderStats.VerticiesDrawn += s_PrimativeData.QuadVertexArray->GetVertexCount();
	}

	void Renderer::ResetStats()
	{
		s_RenderStats = RenderStats();
	}

	void Renderer::BeginBatch()
	{
		s_BatchData.IndexCount = 0;
		s_BatchData.VertexBufferPtr = s_BatchData.VertexBufferBase;
		s_BatchData.TextureSlotIndex = 1;
	}

	void Renderer::FlushBatch()
	{
	}
}