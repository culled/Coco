#pragma once
#include "Core/Base.h"
#include "Core/ImGui/ImGuiRenderer.h"

namespace Coco
{
	class COCO_API OpenGLImGui : public ImGuiRenderer
	{
	public:
		OpenGLImGui();
		virtual ~OpenGLImGui();

		virtual void Initialize() override;
		virtual void Shutdown() override;
		virtual void OnNewFrame() override;
		virtual void OnEndFrame() override;
	};
}

