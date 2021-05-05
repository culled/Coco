#pragma once
#include "Core/Base.h"
#include "KeyCodes.h"
#include "MouseButtonCodes.h"

namespace Coco {
	class COCO_API Input
	{
	public:
		static bool IsKeyPressed(KeyCodes keycode);

		static bool IsMouseButtonPressed(MouseButtons button);
		static float GetMousePositionX();
		static float GetMousePositionY();
		static std::pair<float, float> GetMousePosition();
	};
}