#include "ccpch.h"
#include "OpenGLFramebuffer.h"

#include "OpenGLTextures.h"
#include "Core/App/Log.h"

namespace Coco
{
	static const uint32_t s_MaxFramebufferSize = 8192;

	static bool IsDepthFormat(const FramebufferTexture& tex)
	{
		switch (tex.TextureFormat)
		{
		case FramebufferTextureFormat::Depth24Stencil8:
			return true;
		}

		return false;
	}

	static GLenum GetInternalFormat(const FramebufferTexture& tex)
	{
		switch (tex.TextureFormat)
		{
		case FramebufferTextureFormat::RGB24:
			return GL_RGB8;
		case FramebufferTextureFormat::RGBA32:
			return GL_RGBA8;
		case FramebufferTextureFormat::R32:
			return GL_R32I;
		case FramebufferTextureFormat::Depth24Stencil8:
			return GL_DEPTH24_STENCIL8;
		}

		ASSERT_CORE(false, "Invalid format");
		return GL_FALSE;
	}

	static GLenum GetDataFormat(const FramebufferTexture& tex)
	{
		switch (tex.TextureFormat)
		{
		case FramebufferTextureFormat::RGB24:
			return GL_RGB;
		case FramebufferTextureFormat::RGBA32:
			return GL_RGBA;
		case FramebufferTextureFormat::R32:
			return GL_RED_INTEGER;
		case FramebufferTextureFormat::Depth24Stencil8:
			return GL_DEPTH24_STENCIL8;
		}

		ASSERT_CORE(false, "Invalid format");
		return GL_FALSE;
	}

	static GLenum GetDataType(const FramebufferTexture& tex)
	{
		switch (tex.TextureFormat)
		{
		case FramebufferTextureFormat::RGB24:
			return GL_UNSIGNED_BYTE;
		case FramebufferTextureFormat::RGBA32:
			return GL_UNSIGNED_BYTE;
		case FramebufferTextureFormat::R32:
			return GL_INT;
		case FramebufferTextureFormat::Depth24Stencil8:
			return GL_DEPTH24_STENCIL8;
		}

		ASSERT_CORE(false, "Invalid format");
		return GL_FALSE;
	}

	static GLenum GetTextureTarget(bool multisample)
	{
		return multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static GLenum GetDepthAttachmentType(const FramebufferTexture& tex)
	{
		switch (tex.TextureFormat)
		{
		case FramebufferTextureFormat::Depth24Stencil8:
			return GL_DEPTH_STENCIL_ATTACHMENT;
		}

		ASSERT_CORE(false, "Invalid format");
		return GL_FALSE;
	}

	OpenGLFramebuffer::OpenGLFramebuffer(uint32_t width, uint32_t height, FramebufferCreationProperties creationProperties) :
		m_Width(width), m_Height(height), m_ScreenTarget(creationProperties.ScreenTarget), m_Samples(creationProperties.Samples)
	{
		for (auto format : creationProperties.Attachments)
		{
			if (IsDepthFormat(format.TextureFormat))
			{
				m_DepthAttachments.emplace_back(format);
			}
			else
			{
				m_ColorAttachments.emplace_back(format);
			}
		}

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		if (m_Id)
		{
			glDeleteFramebuffers(1, &m_Id);

			DeleteTextures();
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
			DeleteTextures();
		}

		glCreateFramebuffers(1, &m_Id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_Id);

		uint32_t index = 0;
		std::vector<GLenum> buffers;

		for (auto& tex : m_ColorAttachments)
		{
			GLenum textureTarget = GetTextureTarget(m_Samples > 1);

			glCreateTextures(textureTarget, 1, &tex.ID);
			glBindTexture(textureTarget, tex.ID);

			if (m_Samples > 1)
			{
				glTexImage2DMultisample(textureTarget, m_Samples, GetInternalFormat(tex.TextureFormat), m_Width, m_Height, GL_FALSE);
			}
			else
			{
				glTexImage2D(textureTarget, 0, GetInternalFormat(tex.TextureFormat), m_Width, m_Height, 0,
					GetDataFormat(tex.TextureFormat), GetDataType(tex.TextureFormat), nullptr);
			}

			glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, OpenGLTexture2D::GetFilteringMode(tex.Filtering));
			glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, OpenGLTexture2D::GetFilteringMode(tex.Filtering));
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, OpenGLTexture2D::GetWrapMode(tex.WrapMode));
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, OpenGLTexture2D::GetWrapMode(tex.WrapMode));
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, OpenGLTexture2D::GetWrapMode(tex.WrapMode));

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, textureTarget, tex.ID, 0);
			buffers.push_back(GL_COLOR_ATTACHMENT0 + index);

			index++;
		}

		for (auto& tex : m_DepthAttachments)
		{
			GLenum textureTarget = GetTextureTarget(m_Samples > 1);

			glCreateTextures(textureTarget, 1, &tex.ID);
			glBindTexture(textureTarget, tex.ID);

			if (m_Samples > 1)
			{
				glTexImage2DMultisample(textureTarget, m_Samples, GetInternalFormat(tex.TextureFormat), m_Width, m_Height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(textureTarget, 1, GetInternalFormat(tex.TextureFormat), m_Width, m_Height);
			}

			glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, OpenGLTexture2D::GetFilteringMode(tex.Filtering));
			glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, OpenGLTexture2D::GetFilteringMode(tex.Filtering));
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, OpenGLTexture2D::GetWrapMode(tex.WrapMode));
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, OpenGLTexture2D::GetWrapMode(tex.WrapMode));
			glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, OpenGLTexture2D::GetWrapMode(tex.WrapMode));

			glFramebufferTexture2D(GL_FRAMEBUFFER, GetDepthAttachmentType(tex), textureTarget, tex.ID, 0);
		}

		if (!buffers.empty())
		{
			glDrawBuffers((GLsizei)buffers.size(), buffers.data());
		}
		else
		{
			//No color pass
			glDrawBuffer(GL_NONE);
		}

		auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		//LOG_CORE_INFO("Framebuffer status: {0}", fboStatus);
		ASSERT_CORE(fboStatus == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");
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

	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		ASSERT_CORE(attachmentIndex < m_ColorAttachments.size(), "Attachment index must be within attachments");

		const auto& tex = m_ColorAttachments[attachmentIndex];

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GetDataFormat(tex), GetDataType(tex), &pixelData);

		return pixelData;
	}

	void OpenGLFramebuffer::ClearColorAttachment(uint32_t index, const void* value)
	{
		ASSERT_CORE(index < m_ColorAttachments.size(), "Attachment index must be within attachments");

		const auto& tex = m_ColorAttachments[index];

		glClearTexImage(tex.ID, 0, GetDataFormat(tex), GetDataType(tex), value);
	}

	void OpenGLFramebuffer::DeleteTextures()
	{
		for (const auto& tex : m_ColorAttachments)
		{
			glDeleteTextures(1, &tex.ID);
		}

		for (const auto& tex : m_DepthAttachments)
		{
			glDeleteTextures(1, &tex.ID);
		}
	}
}