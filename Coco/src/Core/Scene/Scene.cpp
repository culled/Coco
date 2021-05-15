#include "ccpch.h"
#include "Scene.h"

#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/Renderer2D.h"
#include "Core/Components/Components.h"
#include "Core/Components/RenderingComponents.h"

namespace Coco
{
	void Scene::Update(Timestep timestep)
	{
		//Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();

					ASSERT_CORE(nsc.Instance, "Instantiation didn't work");

					nsc.Instance->m_Entity = Entity(entity, this);

					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(timestep);
				});
		}

		//Update cameras
		{
			auto cameraView = m_Registry.view<TransformComponent, CameraComponent>();

			for (auto camEntity : cameraView)
			{
				auto [transformComponent, camComponent] = cameraView.get(camEntity);
				DrawForCamera(camComponent.Camera, transformComponent);
			}
		}
	}

	void Scene::DrawForCamera(const Camera& camera, const glm::mat4& cameraTransform)
	{
		auto spriteView = m_Registry.view<TransformComponent, SpriteRendererComponent>();
		Renderer::BeginScene(camera, cameraTransform);

		for (auto entity : spriteView)
		{
			auto [transform, rendererComponent] = spriteView.get(entity);

			Renderer2D::SubmitImmediateSprite(transform, (uint32_t)entity, rendererComponent.RenderMaterial);
		}

		auto meshView = m_Registry.view<TransformComponent, MeshRendererComponent>();
		for (auto entity : meshView)
		{
			auto [transform, rendererComponent] = meshView.get(entity);

			if(rendererComponent.Data)
				Renderer::SubmitMeshImmediate(rendererComponent.Data, rendererComponent.RenderMaterial, transform, (uint32_t)entity);
		}

		Renderer::EndScene();
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(name);
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}
}
