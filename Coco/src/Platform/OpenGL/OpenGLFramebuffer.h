#pragma once
#include "Core/Base.h"

#include "Core/Rendering/Framebuffer.h"

#include "glad/glad.h"

namespace Coco
{
	struct COCO_API OpenGLFramebufferTexture
	{
		TextureFormat Format = TextureFormat::RGB24;
		TextureFilteringMode Filtering = TextureFilteringMode::Linear;
		uint32_t ID = 0;

		OpenGLFramebufferTexture() {}

		OpenGLFramebufferTexture(TextureFormat format, TextureFilteringMode filtering) :
			Format(format), Filtering(filtering) {}
	};

	class COCO_API OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(uint32_t width, uint32_t height, FramebufferCreationProperties creationProperties);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Invalidate() override;
		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentID() const override { return m_Attachments.at(GL_COLOR_ATTACHMENT0).ID; }
	private:
		uint32_t m_Id = 0;
		uint32_t m_Width, m_Height;
		bool m_ScreenTarget;

		std::unordered_map<uint32_t, OpenGLFramebufferTexture> m_Attachments;
	};
}

