#include "ccpch.h"
#include "ImGuiLayer.h"

namespace Coco
{
	ImGuiLayer::ImGuiLayer() : Layer("ImGui")
	{
	}

	void ImGuiLayer::OnAttached()
	{
		m_Renderer = ImGuiRenderer::Create();
	}

	void ImGuiLayer::OnDetached()
	{
		m_Renderer.reset(nullptr);
	}

	void ImGuiLayer::Begin()
	{
		m_Renderer->OnNewFrame();
	}

	void ImGuiLayer::End()
	{
		m_Renderer->OnEndFrame();
	}
}