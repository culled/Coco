#pragma once
#include "Core/Base.h"
#include <string>
#include "Core/Events/EventDispatcher.h"

namespace Coco
{
	class COCO_API Window
	{
	public:
		/*@brief Creates a window with the given arguments
		* 		@param title - The title of the window
		* 		@param width - The initial width of the window
		* 		@param height - The initial height of the window
		*/
		static Scope<Window> Create(const std::string& title, const uint32_t& width, const uint32_t& height);

		virtual ~Window() = default;

		/*@brief Gets the current width of the window
		* 		@returns The width of the window
		*/
		virtual uint32_t GetWidth() const = 0;

		/*@brief Gets the current height of the window
		* 		@returns The height of the window
		*/
		virtual uint32_t GetHeight() const = 0;

		/*@brief Sets the title of the window
		* 		@param title - The new title of the window
		*/
		virtual void SetTitle(const std::string& title) = 0;

		/*@brief Gets the title of the window
		* 		@returns The title of the window
		*/
		virtual const std::string& GetTitle() const = 0;

		/*@brief Gets the native implementation of the window (this is platform specific)
		* 		@returns The native implementation of the window
		*/
		virtual void* GetNativeWindow() const = 0;

		/*@brief Sets the number of V-blanks before the window refreshes
		* 		0 = No vsync
		* 		1 = Vsync to the monitor's refresh rate
		* 		2 = Vsync to 1/2 of the monitor's refresh rate
		* 
		* 		@params vblanks - The number of V-blanks before the window refreshes
		*/
		virtual void SetVSync(int vblanks) = 0;

		/*@brief Gets the vsync setting of the window
		* 		@returns The number of V-blanks the window waits before refreshing
		*/
		virtual int GetVSync() const = 0;

		/*@brief Updates any pending events for the window*/
		virtual void PollEvents() = 0;

		/*@brief Swaps the buffers*/
		virtual void SwapBuffers() = 0;

		virtual void MakeRenderTarget() = 0;

		static Window* GetCurrentRenderTarget();

		virtual EventDispatcher& GetEventDispatcher() = 0;
	};
}