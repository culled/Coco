#include "ccpch.h"
#include "Renderer.h"

#include "RenderCommand.h"

namespace Coco
{
	SceneData Renderer::s_SceneData;
	RenderStats Renderer::s_RenderStats;
	PrimativeRenderData Renderer::s_PrimativeData;

	void Renderer::Init()
	{
		float verts[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		Ref<VertexBuffer> quadVBO = VertexBuffer::Create(verts, sizeof(verts));
		quadVBO->SetLayout({ {ShaderDataType::Float3, "a_Pos"} });

		uint32_t indicies[] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> quadIBO = IndexBuffer::Create(indicies, 6);

		s_PrimativeData.QuadVertexArray = VertexArray::Create(quadVBO, quadIBO);
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& cameraTransform)
	{
		s_SceneData.ViewProjectionMatrix = camera.GetProjectionMatrix() * glm::inverse(cameraTransform);
		s_RenderStats.StartTimer();
	}

	void Renderer::EndScene()
	{
		s_RenderStats.EndTimer();
	}

	void Renderer::SubmitImmediate(Ref<VertexArray> vao, Ref<Material> material, const glm::mat4& transform)
	{
		material->SetMatrix4("u_ViewProjectionMatrix", s_SceneData.ViewProjectionMatrix);
		material->SetMatrix4("u_ModelMatrix", transform);
		material->Use();

		RenderCommand::DrawIndexed(vao);

		s_RenderStats.DrawCalls++;
		s_RenderStats.IndiciesDrawn += vao->GetIndexBuffer()->GetCount();
		s_RenderStats.VerticiesDrawn += vao->GetVertexCount();
	}

	void Renderer::SubmitImmediateQuad(Ref<Material> material, const glm::mat4& transform)
	{
		material->SetMatrix4("u_ViewProjectionMatrix", s_SceneData.ViewProjectionMatrix);
		material->SetMatrix4("u_ModelMatrix", transform);
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
}