#pragma once
#include "Core/Base.h"

#include "entt.hpp"
#include "Core/Timing/Timestep.h"
#include "Core/App/Log.h"

namespace Coco
{
	class Scene;

	class COCO_API Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene) : m_EntityHandle(handle), m_Scene(scene) {}
		Entity(const Entity & other) = default;
		~Entity() = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			ASSERT_CORE(!HasComponent<T>(), "Entity already has component");

			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			ASSERT_CORE(HasComponent<T>(), "Entity does not have component");

			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			ASSERT_CORE(HasComponent<T>(), "Entity does not have component");
			return m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		operator entt::entity() const { return m_EntityHandle; }
		bool operator ==(const Entity other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
		bool operator !=(const Entity other) const { return !(*this == other); }

	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;
	};

	class COCO_API ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() = default;

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Entity.HasComponent<T>();
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep timestep) {}

	private:
		Entity m_Entity;
		friend class Scene;
	};
}