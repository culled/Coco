#pragma once
#include "Core/Base.h"

#include "Core/Timing/Timestep.h"
#include "Entity.h"
#include "Core/Rendering/Camera.h"

namespace Coco
{
	class COCO_API Scene
	{
	public:
		friend class Entity;

		virtual ~Scene() = default;

		void Update(Timestep timestep);

		void DrawForCamera(const Camera& camera, const glm::mat4& cameraTransform);

		Entity CreateEntity(const std::string& name);

		const entt::registry& GetRegistry() { return m_Registry; }

	private:
		entt::registry m_Registry;
	};
}

