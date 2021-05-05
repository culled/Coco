#include "ccpch.h"
#include "GLFWWindow.h"

#include "OpenGLContext.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Core/App/Log.h"

namespace Coco
{
	static void GLFWErrorCallback(int error, const char* description)
	{
		LOG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	bool GLFWWindow::s_GLFWInitialized = false;

	GLFWWindow::GLFWWindow(const std::string& title, uint32_t width, uint32_t height)
		: m_Title(title), m_Width(width), m_Height(height)
	{
		InitGLFW();
	}

	GLFWWindow::~GLFWWindow()
	{
		if (m_NativeWindow != NULL)
		{
			glfwDestroyWindow(m_NativeWindow);
			LOG_CORE_INFO("Destroyed window {0}", m_Title);
		}
	}

	void GLFWWindow::SetTitle(const std::string& title)
	{
		m_Title = title;
		glfwSetWindowTitle(m_NativeWindow, m_Title.c_str());
	}

	void GLFWWindow::SetVSync(int vblanks)
	{
		glfwSwapInterval(vblanks);
		m_VSync = vblanks;
	}

	void GLFWWindow::PollEvents()
	{
		glfwPollEvents();
	}

	void GLFWWindow::SwapBuffers()
	{
		glfwSwapBuffers(m_NativeWindow);
	}

	void GLFWWindow::MakeRenderTarget()
	{
		glfwMakeContextCurrent(m_NativeWindow);
	}

	void GLFWWindow::InitGLFW()
	{
		if (!s_GLFWInitialized)
		{
			int status = glfwInit();

			ASSERT_CORE(status, "Could not initialize GLFW");

			s_GLFWInitialized = true;

			glfwSetErrorCallback(GLFWErrorCallback);

			LOG_CORE_INFO("GLFW Initialized");
		}

		LOG_CORE_INFO("Creating window {0} ({1}, {2})", m_Title, m_Width, m_Height);

		const char* title = m_Title.c_str();
		m_NativeWindow = glfwCreateWindow(m_Width, m_Height, title, NULL, NULL);
		glfwSetWindowUserPointer(m_NativeWindow, this);
		glfwMakeContextCurrent(m_NativeWindow);

		if (!OpenGLContext::Loaded())
		{
			Ref<OpenGLContext> context = std::dynamic_pointer_cast<OpenGLContext>(GLContext::GetCurrentContext());
			ASSERT_CORE(context, "No OpenGL context");

			context->Load((GLADloadproc)glfwGetProcAddress);
		}

		SetVSync(1);
		SetupEventCallbacks();
	}

	void GLFWWindow::SetupEventCallbacks()
	{
		glfwSetWindowSizeCallback(m_NativeWindow, [](GLFWwindow* glWindow, int newWidth, int newHeight)
			{
				GLFWWindow* window = (GLFWWindow*)glfwGetWindowUserPointer(glWindow);
				window->m_Width = newWidth;
				window->m_Height = newHeight;

				window->m_EventDispatcher.Invoke<ResizedEventArgs>(newWidth, newHeight);
			});

		glfwSetWindowCloseCallback(m_NativeWindow, [](GLFWwindow* glWindow)
			{
				GLFWWindow* window = (GLFWWindow*)glfwGetWindowUserPointer(glWindow);
				ClosingEventArgs args = window->m_EventDispatcher.Invoke<ClosingEventArgs>();

				LOG_CORE_INFO("Should close: {0}", args.Close);

				if (!args.Close)
				{
					glfwSetWindowShouldClose(glWindow, 0);
				}
				else
				{
					glfwSetWindowShouldClose(glWindow, 1);
					window->m_EventDispatcher.Invoke<ClosedEventArgs>();
				}
			});

		glfwSetKeyCallback(m_NativeWindow, [](GLFWwindow* glWindow, int key, int scancode, int action, int mods)
			{
				GLFWWindow* window = (GLFWWindow*)glfwGetWindowUserPointer(glWindow);

				switch (action)
				{
				case GLFW_PRESS:
					window->m_EventDispatcher.Invoke<KeyPressEventArgs>(key, mods);
					break;
				case GLFW_RELEASE:
					window->m_EventDispatcher.Invoke<KeyReleaseEventArgs>(key, mods);
					break;
				case GLFW_REPEAT:
					window->m_EventDispatcher.Invoke<KeyRepeatEventArgs>(key, mods);
					break;
				}
			});

		glfwSetCharCallback(m_NativeWindow, [](GLFWwindow* glWindow, unsigned int keycode) {
			GLFWWindow* window = (GLFWWindow*)glfwGetWindowUserPointer(glWindow);

			window->m_EventDispatcher.Invoke<KeyTypedEventArgs>(keycode);
			});

		glfwSetMouseButtonCallback(m_NativeWindow, [](GLFWwindow* glWindow, int button, int action, int mods) {
			GLFWWindow* window = (GLFWWindow*)glfwGetWindowUserPointer(glWindow);

			switch (action)
			{
			case GLFW_PRESS:
				window->m_EventDispatcher.Invoke<MouseButtonPressEventArgs>(button, mods);
				break;
			case GLFW_RELEASE:
				window->m_EventDispatcher.Invoke<MouseButtonReleaseEventArgs>(button, mods);
				break;
			}
			});

		glfwSetScrollCallback(m_NativeWindow, [](GLFWwindow* glWindow, double xOffset, double yOffset) {
			GLFWWindow* window = (GLFWWindow*)glfwGetWindowUserPointer(glWindow);
			window->m_EventDispatcher.Invoke<ScrollEventArgs>((float)xOffset, (float)yOffset);
			});

		glfwSetCursorPosCallback(m_NativeWindow, [](GLFWwindow* glWindow, double xPos, double yPos) {

			GLFWWindow* window = (GLFWWindow*)glfwGetWindowUserPointer(glWindow);
			window->m_EventDispatcher.Invoke<CursorPositionEventArgs>((float)xPos, (float)yPos);
			});
	}

	Window* Window::GetCurrentRenderTarget()
	{
		GLFWWindow* windowImpl = (GLFWWindow*)glfwGetWindowUserPointer(glfwGetCurrentContext());
		return windowImpl;
	}
}
