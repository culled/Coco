#pragma once
#include "Core/Layers/Layer.h"
#include "Core/Events/EventDispatcher.h"
#include "Core/Timing/Timestep.h"

class SandboxLayer : public Coco::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer() = default;

	virtual void OnAttached() override;
	virtual void OnDetached() override;
	virtual void OnEvent(Coco::DispatchedEvent& e) override;
	virtual void Update(Coco::Timestep timestep) override;
	virtual void OnImGuiRender() override;
};

