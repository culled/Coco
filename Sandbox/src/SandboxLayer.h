#pragma once
#include <Coco.h>

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

private:
	Coco::Ref<Coco::Shader> m_Shader = nullptr;
	Coco::Ref<Coco::Scene> m_ActiveScene = nullptr;
	Coco::Entity m_Camera;
};

