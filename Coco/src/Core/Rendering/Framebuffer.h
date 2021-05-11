#pragma once
#include "Core/Base.h"

#include "Textures.h"

namespace Coco
{
	enum class FramebufferTextureFormat
	{
		None = 0,

		//Color
		RGBA32 = 1,
		RGB24,
		R32,

		//Depth / stencil
		Depth24Stencil8,

		//Defaults
		Depth = Depth24Stencil8
	};

	struct COCO_API FramebufferTexture
	{
		FramebufferTexture() = default;
		FramebufferTexture(FramebufferTextureFormat format, TextureFilteringMode filtering = TextureFilteringMode::Linear) : TextureFormat(format), Filtering(filtering) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		TextureFilteringMode Filtering = TextureFilteringMode::Linear;
		TextureWrapMode WrapMode = TextureWrapMode::Clamp;
	};

	struct COCO_API FramebufferCreationProperties
	{		
		FramebufferCreationProperties() = default;
		FramebufferCreationProperties(std::initializer_list< FramebufferTexture> attachments) : Attachments(attachments) {}

		uint32_t Samples = 1;
		bool ScreenTarget = false;

		std::vector<FramebufferTexture> Attachments;
	};

	class COCO_API Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Invalidate() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual uint32_t GetColorAttachmentID(int index = 0) const = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void ClearColorAttachment(uint32_t index, const void* value) = 0;

		static Ref<Framebuffer> Create(uint32_t width, uint32_t height, FramebufferCreationProperties creationProperties = FramebufferCreationProperties());
	};
}

