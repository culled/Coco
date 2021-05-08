#include "ccpch.h"
#include "Core/Utils/PlatformUtils.h"

#include "Core/App/Application.h"
#include "Core/App/Log.h"

#include <windows.h>
#include <commdlg.h>

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Coco
{
	std::string FileDialog::OpenFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };

		ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetMainWindow().GetNativeWindow());

		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);

		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}

		return std::string();
	}

	std::string FileDialog::SaveFile(const char* filter)
	{
		OPENFILENAMEA sfn;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&sfn, sizeof(OPENFILENAMEA));

		sfn.lStructSize = sizeof(OPENFILENAMEA);
		sfn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetMainWindow().GetNativeWindow());

		sfn.lpstrFile = szFile;
		sfn.nMaxFile = sizeof(szFile);

		sfn.lpstrFilter = filter;
		sfn.nFilterIndex = 1;

		sfn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA(&sfn) == TRUE)
		{
			return sfn.lpstrFile;
		}

		return std::string();
	}
}