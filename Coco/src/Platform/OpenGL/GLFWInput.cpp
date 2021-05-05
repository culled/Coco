#include "ccpch.h"
#include "Core/Input/Input.h"

#include "Core/App/Application.h"
#include "GLFW/glfw3.h"

namespace Coco
{
	bool Input::IsKeyPressed(KeyCodes keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetMainWindow().GetNativeWindow());

		int state = glfwGetKey(window, static_cast<int>(keycode));

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(MouseButtons button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetMainWindow().GetNativeWindow());

		int state = glfwGetMouseButton(window, static_cast<int>(button));

		return state == GLFW_PRESS;
	}

	float Input::GetMousePositionX()
	{
		auto [x, y] = GetMousePosition();

		return (float)x;
	}

	float Input::GetMousePositionY()
	{
		auto [x, y] = GetMousePosition();

		return (float)y;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetMainWindow().GetNativeWindow());
		double xPos, yPos;

		glfwGetCursorPos(window, &xPos, &yPos);

		return std::pair<float, float>((float)xPos, (float)yPos);
	}
}