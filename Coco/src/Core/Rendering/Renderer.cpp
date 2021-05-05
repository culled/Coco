#include "ccpch.h"
#include "Renderer.h"

#include "RenderCommand.h"

namespace Coco
{
	SceneData Renderer::s_SceneData;


	void Renderer::Init()
	{
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& cameraTransform)
	{
		s_SceneData.ViewProjectionMatrix = camera.GetProjectionMatrix() * glm::inverse(cameraTransform);
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::SubmitImmediate(Ref<VertexArray> vao, Ref<Material> material, const glm::mat4& transform)
	{
		material->Use();
		material->SetMatrix4("u_ViewProjectionMatrix", s_SceneData.ViewProjectionMatrix);
		material->SetMatrix4("u_ModelMatrix", transform);

		RenderCommand::DrawIndexed(vao);
	}
}