#include "ccpch.h"
#include "OpenGLTextures.h"

#include "Core/App/Log.h"
#include "stb_image.h"

namespace Coco
{
	GLenum OpenGLTexture2D::GetFilteringMode(TextureFilteringMode filtering)
	{
		switch (filtering)
		{
		case TextureFilteringMode::Linear: return GL_LINEAR;
		case TextureFilteringMode::Nearest: return GL_NEAREST;
		}

		ASSERT_CORE(false, "Invalid filtering mode");
		return GL_FALSE;
	}

	GLenum OpenGLTexture2D::GetWrapMode(TextureWrapMode wrapMode)
	{
		switch (wrapMode)
		{
		case TextureWrapMode::Clamp: return GL_CLAMP_TO_EDGE;
		case TextureWrapMode::Repeat: return GL_REPEAT;
		}

		ASSERT_CORE(false, "Invalid wrap mode");
		return GL_FALSE;
	}

	GLenum OpenGLTexture2D::GetInternalFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RGB24: return GL_RGB8;
		case TextureFormat::RGBA32: return GL_RGBA8;
		}

		ASSERT_CORE(false, "Invalid format");
		return GL_FALSE;
	}

	GLenum OpenGLTexture2D::GetDataFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RGB24: return GL_RGB;
		case TextureFormat::RGBA32: return GL_RGBA;
		}

		ASSERT_CORE(false, "Invalid format");
		return GL_FALSE;
	}

	GLenum OpenGLTexture2D::GetDataType(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RGB24: return GL_UNSIGNED_BYTE;
		case TextureFormat::RGBA32: return GL_UNSIGNED_BYTE;
		}

		ASSERT_CORE(false, "Invalid format");
		return GL_FALSE;
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, TextureCreationParameters params)
		:m_Path(path), m_FilteringMode(params.Filtering), m_WrapMode(params.WrapMode), m_Format(TextureFormat::RGBA32)
	{
		int width = 0, height = 0, channels = 0;

		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		ASSERT_CORE(data, "Failed to load image");

		m_Width = width;
		m_Height = height;

		if (channels == 4)
		{
			m_Format = TextureFormat::RGBA32;
		}
		else if (channels == 3)
		{
			m_Format = TextureFormat::RGB24;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_Id);
		glTextureStorage2D(m_Id, 1, GetInternalFormat(m_Format), m_Width, m_Height);

		glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, GetFilteringMode(m_FilteringMode));
		glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, GetFilteringMode(m_FilteringMode));
		glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, GetWrapMode(m_WrapMode));
		glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, GetWrapMode(m_WrapMode));

		glTextureSubImage2D(m_Id, 0, 0, 0, m_Width, m_Height, GetDataFormat(m_Format), GetDataType(m_Format), data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, TextureFormat format, TextureCreationParameters params)
		: m_Width(width), m_Height(height), m_FilteringMode(params.Filtering), m_WrapMode(params.WrapMode),
		m_Format(format)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_Id);
		glTextureStorage2D(m_Id, 1, GetInternalFormat(m_Format), m_Width, m_Height);

		glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, GetFilteringMode(m_FilteringMode));
		glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, GetFilteringMode(m_FilteringMode));
		glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, GetWrapMode(m_WrapMode));
		glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, GetWrapMode(m_WrapMode));
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_Id);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpc = m_Format == TextureFormat::RGBA32 ? 4 : 3;

		ASSERT_CORE(size == m_Width * m_Height * bpc, "Size does not match the image's data size");
		glTextureSubImage2D(m_Id, 0, 0, 0, m_Width, m_Height, GetDataFormat(m_Format), GetDataType(m_Format), data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_Id);
	}

	bool OpenGLTexture2D::operator==(const Texture2D& other) const
	{
		return m_Id == other.GetID();
	}
}