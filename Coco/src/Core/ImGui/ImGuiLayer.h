#pragma once
#include "Core/Layers/Layer.h"
#include "ImGuiRenderer.h"

namespace Coco
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer() = default;

		virtual void OnDetached() override;
		virtual void OnAttached() override;

		void Begin();
		void End();
	private:
		Scope<ImGuiRenderer> m_Renderer = nullptr;
	};
}

