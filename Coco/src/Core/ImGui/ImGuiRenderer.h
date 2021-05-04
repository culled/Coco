#pragma once
#include "Core/Base.h"

namespace Coco
{
	class COCO_API ImGuiRenderer
	{
	public:
		virtual ~ImGuiRenderer() = default;

		static Scope<ImGuiRenderer> Create();

		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;
		virtual void OnNewFrame() = 0;
		virtual void OnEndFrame() = 0;
	};
}

