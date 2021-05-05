#include "ccpch.h"
#include "OpenGLFramebuffer.h"

#include "OpenGLTextures.h"
#include "Core/App/Log.h"

namespace Coco
{
	static const uint32_t s_MaxFramebufferSize = 8192;

	OpenGLFramebuffer::OpenGLFramebuffer(uint32_t width, uint32_t height, FramebufferCreationProperties creationProperties) :
		m_Width(width), m_Height(height), m_ScreenTarget(creationProperties.ScreenTarget)
	{
		m_Attachments[GL_COLOR_ATTACHMENT0] = OpenGLFramebufferTexture(creationProperties.Format, creationProperties.Filtering);
		m_Attachments[GL_DEPTH_STENCIL_ATTACHMENT] = OpenGLFramebufferTexture(creationProperties.Format, creationProperties.Filtering);

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		if (m_Id)
		{
			glDeleteFramebuffers(1, &m_Id);
			glDeleteTextures(1, &m_Attachments.at(GL_COLOR_ATTACHMENT0).ID);
			glDeleteTextures(1, &m_Attachments.at(GL_DEPTH_STENCIL_ATTACHMENT).ID);
		}
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
		glViewport(0, 0, m_Width, m_Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (m_Id)
		{
			glDeleteFramebuffers(1, &m_Id);
			glDeleteTextures(1, &m_Attachments.at(GL_COLOR_ATTACHMENT0).ID);
			glDeleteTextures(1, &m_Attachments.at(GL_DEPTH_STENCIL_ATTACHMENT).ID);
		}

		glCreateFramebuffers(1, &m_Id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_Id);

		OpenGLFramebufferTexture& tex = m_Attachments.at(GL_COLOR_ATTACHMENT0);

		glCreateTextures(GL_TEXTURE_2D, 1, &tex.ID);
		glBindTexture(GL_TEXTURE_2D, tex.ID);

		glTexImage2D(GL_TEXTURE_2D, 0, OpenGLTexture2D::GetInternalFormat(tex.Format), m_Width, m_Height, 0,
			OpenGLTexture2D::GetDataFormat(tex.Format), GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, OpenGLTexture2D::GetFilteringMode(tex.Filtering));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, OpenGLTexture2D::GetFilteringMode(tex.Filtering));

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.ID, 0);

		OpenGLFramebufferTexture& dpTex = m_Attachments.at(GL_DEPTH_STENCIL_ATTACHMENT);
		glCreateTextures(GL_TEXTURE_2D, 1, &dpTex.ID);
		glBindTexture(GL_TEXTURE_2D, dpTex.ID);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Width, m_Height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, dpTex.ID, 0);

		ASSERT_CORE(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			LOG_CORE_WARN("Invalid framebuffer resize: {0}, {1}", width, height);
			return;
		}

		m_Width = width;
		m_Height = height;

		Invalidate();
	}
}