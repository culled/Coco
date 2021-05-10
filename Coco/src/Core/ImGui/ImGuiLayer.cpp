#include "ccpch.h"
#include "ImGuiLayer.h"
#include "ImGuizmo.h"

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
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		m_Renderer->OnEndFrame();
	}
}