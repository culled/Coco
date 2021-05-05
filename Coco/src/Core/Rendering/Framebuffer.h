#pragma once
#include "Core/Base.h"

#include "Textures.h"

namespace Coco
{
	enum class FramebufferType : uint32_t
	{
		None = 0,
		Color = 1,
		Depth = 2,
		Stencil = 4
	};

	constexpr enum FramebufferType operator|(const FramebufferType left, const FramebufferType right)
	{
		return (enum FramebufferType)(uint32_t(left) | uint32_t(right));
	}

	constexpr enum FramebufferType operator&(const FramebufferType left, const FramebufferType right)
	{
		return (enum FramebufferType)(uint32_t(left) & uint32_t(right));
	}

	struct COCO_API FramebufferCreationProperties
	{
		TextureFormat Format = TextureFormat::RGB24;
		TextureFilteringMode Filtering = TextureFilteringMode::Linear;
		uint32_t Samples = 1;
		FramebufferType Attachments = FramebufferType::Color;

		bool ScreenTarget = false;
	};

	class COCO_API Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Invalidate() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentID() const = 0;

		static Ref<Framebuffer> Create(uint32_t width, uint32_t height, FramebufferCreationProperties creationProperties = FramebufferCreationProperties());
	};
}

