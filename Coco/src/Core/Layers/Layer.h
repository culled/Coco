#pragma once
#include "Core/Base.h"
#include "Core/Events/EventDispatcher.h"
#include "Core/Timing/Timestep.h"

namespace Coco
{
	class COCO_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttached() {}
		virtual void OnDetached() {}
		virtual void OnEvent(DispatchedEvent& e) {}
		virtual void Update(Timestep timestep) {}
		virtual void OnImGuiRender() {}
		const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}

