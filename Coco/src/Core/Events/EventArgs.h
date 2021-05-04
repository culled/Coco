#pragma once
#include "Core/Base.h"
#include <string>

namespace Coco
{
	struct COCO_API EventArgs
	{
		bool Handled = false;

		virtual std::string ToString() const { return "EventArgs"; }

		EventArgs() = default;
		virtual ~EventArgs() = default;
	};
	
	struct COCO_API ResizedEventArgs : public EventArgs
	{
		uint32_t Width = 0, Height = 0;

		virtual std::string ToString() const override { return "ResizedEventArgs"; }

		ResizedEventArgs() = default;
		ResizedEventArgs(uint32_t width, uint32_t height) :
			Width(width), Height(height) {}
		virtual ~ResizedEventArgs() = default;
	};

	struct COCO_API ClosingEventArgs : public EventArgs
	{
		bool Close = true;

		virtual std::string ToString() const override { return "ClosingEventArgs"; }

		ClosingEventArgs() = default;
		virtual ~ClosingEventArgs() = default;
	};

	struct COCO_API ClosedEventArgs : public EventArgs
	{
		virtual std::string ToString() const override { return "ClosedEventArgs"; }

		ClosedEventArgs() = default;
		virtual ~ClosedEventArgs() = default;
	};

	struct COCO_API KeyEventArgs : public EventArgs
	{
		int Key = -1, Modifiers = -1;

		virtual std::string ToString() const override { return "KeyEventArgs"; }

		KeyEventArgs() = default;
		KeyEventArgs(int key, int modifiers) :
			Key(key), Modifiers(modifiers) {}
		virtual ~KeyEventArgs() = default;
	};

	struct COCO_API KeyPressEventArgs : public KeyEventArgs
	{
		virtual std::string ToString() const override { return "KeyPressEventArgs"; }

		KeyPressEventArgs() :
			KeyEventArgs() {}
		KeyPressEventArgs(int key, int modifiers) :
			KeyEventArgs(key, modifiers) {}
		virtual ~KeyPressEventArgs() = default;
	};

	struct COCO_API KeyRepeatEventArgs : public KeyEventArgs
	{
		virtual std::string ToString() const override { return "KeyRepeatEventArgs"; }

		KeyRepeatEventArgs() :
			KeyEventArgs() {}
		KeyRepeatEventArgs(int key, int modifiers) :
			KeyEventArgs(key, modifiers) {}
		virtual ~KeyRepeatEventArgs() = default;
	};

	struct COCO_API KeyReleaseEventArgs : public KeyEventArgs
	{
		virtual std::string ToString() const override { return "KeyReleaseEventArgs"; }

		KeyReleaseEventArgs() :
			KeyEventArgs() {}
		KeyReleaseEventArgs(int key, int modifiers) :
			KeyEventArgs(key, modifiers) {}
		virtual ~KeyReleaseEventArgs() = default;
	};

	struct COCO_API KeyTypedEventArgs : public EventArgs
	{
		uint32_t KeyCode = 0;

		virtual std::string ToString() const override { return "KeyTypedEventArgs"; }

		KeyTypedEventArgs() = default;
		KeyTypedEventArgs(unsigned int keycode) :
			KeyCode(keycode) {}
		virtual ~KeyTypedEventArgs() = default;
	};

	struct COCO_API MouseButtonEventArgs : public EventArgs
	{
		int Button = -1, Modifiers = -1;

		virtual std::string ToString() const override { return "MouseButtonEventArgs"; }

		MouseButtonEventArgs() = default;
		MouseButtonEventArgs(int button, int modifiers) :
			Button(button), Modifiers(modifiers) {}
		virtual ~MouseButtonEventArgs() = default;
	};

	struct COCO_API MouseButtonPressEventArgs : public MouseButtonEventArgs
	{
		virtual std::string ToString() const override { return "MouseButtonPressEventArgs"; }

		MouseButtonPressEventArgs() :
			MouseButtonEventArgs() {}
		MouseButtonPressEventArgs(int key, int modifiers) :
			MouseButtonEventArgs(key, modifiers) {}
		virtual ~MouseButtonPressEventArgs() = default;
	};

	struct COCO_API MouseButtonReleaseEventArgs : public MouseButtonEventArgs
	{
		virtual std::string ToString() const override { return "MouseButtonReleaseEventArgs"; }

		MouseButtonReleaseEventArgs() :
			MouseButtonEventArgs() {}
		MouseButtonReleaseEventArgs(int key, int modifiers) :
			MouseButtonEventArgs(key, modifiers) {}
		virtual ~MouseButtonReleaseEventArgs() = default;
	};

	struct COCO_API ScrollEventArgs : public EventArgs
	{
		float xDelta = 0.0f, yDelta = 0.0f;

		virtual std::string ToString() const override { return "ScrollEventArgs"; }

		ScrollEventArgs() = default;
		ScrollEventArgs(float xDelta, float yDelta) :
			xDelta(xDelta), yDelta(yDelta) {}
		virtual ~ScrollEventArgs() = default;
	};

	struct COCO_API CursorPositionEventArgs : public EventArgs
	{
		float xPos = 0.0f, yPos = 0.0f;

		virtual std::string ToString() const override { return "CursorPositionEventArgs"; }

		CursorPositionEventArgs() = default;

		CursorPositionEventArgs(float xPos, float yPos) :
			xPos(xPos), yPos(yPos) {}
		virtual ~CursorPositionEventArgs() = default;
	};
}