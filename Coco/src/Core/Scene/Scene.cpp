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
			auto cameraGroup = m_Registry.group<TransformComponent, CameraComponent>();

			for (auto camEntity : cameraGroup)
			{
				auto [transformComponent, camComponent] = cameraGroup.get(camEntity);
				DrawForCamera(camComponent.Camera, transformComponent);
			}
		}
	}

	void Scene::DrawForCamera(const Camera& camera, const glm::mat4& cameraTransform)
	{
		auto view = m_Registry.view<SpriteRendererComponent>();
		Renderer::BeginScene(camera, cameraTransform);
		Renderer2D::BeginBatch(ShaderLibrary::Get("FlatColorBatched"));

		for (auto entity : view)
		{
			auto [transform, rendererComponent] = m_Registry.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::SubmitBatchedQuad(transform, (uint32_t)entity, nullptr, rendererComponent.Color);
		}

		Renderer2D::FlushBatch();
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
