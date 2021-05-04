#pragma once
#include "Core/Base.h"

#include "Core/App/Window.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Coco
{
	class COCO_API GLFWWindow : public Window
	{
	public:
		GLFWWindow(const std::string& title, uint32_t width, uint32_t height);
		virtual ~GLFWWindow();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void SetTitle(const std::string& title) override;
		virtual const std::string& GetTitle() const override { return m_Title; }

		virtual void* GetNativeWindow() const override { return m_NativeWindow; }

		virtual void SetVSync(int vblanks) override;
		virtual int GetVSync() const override { return m_VSync; }

		virtual void PollEvents() override;
		virtual void SwapBuffers() override;

		virtual void MakeRenderTarget() override;

	private:
		GLFWwindow* m_NativeWindow = nullptr;

		uint32_t m_Width = 0, m_Height = 0;
		std::string m_Title;
		int m_VSync = 0;

		static bool s_GLFWInitialized;

	private:
		void InitGLFW();
		void SetupEventCallbacks();
	};
}

