#pragma once
#include "Core/Base.h"

#include "Core/Rendering/Framebuffer.h"

#include <glad/glad.h>

namespace Coco
{
	struct COCO_API OpenGLFramebufferTexture : public FramebufferTexture
	{
		uint32_t ID = 0;

		OpenGLFramebufferTexture() {}

		OpenGLFramebufferTexture(FramebufferTexture tex) :
			FramebufferTexture(tex.TextureFormat, tex.Filtering) {}
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

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual uint32_t GetColorAttachmentID(int index) const override { return m_ColorAttachments[index].ID; }
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearColorAttachment(uint32_t index, const void* value) override;

	private:
		uint32_t m_Id = 0;
		uint32_t m_Width, m_Height;
		bool m_ScreenTarget;
		uint32_t m_Samples;

		std::vector<OpenGLFramebufferTexture> m_ColorAttachments;
		std::vector<OpenGLFramebufferTexture> m_DepthAttachments;

	private:
		void DeleteTextures();
	};
}

