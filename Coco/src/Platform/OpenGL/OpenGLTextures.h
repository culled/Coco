#pragma once

#include "Core/Base.h"
#include "Core/Rendering/Textures.h"
#include "glad/glad.h"

namespace Coco
{
	class COCO_API OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path, TextureCreationParameters params);
		OpenGLTexture2D(uint32_t width, uint32_t height, TextureFormat format, TextureCreationParameters params);
		virtual ~OpenGLTexture2D();

		virtual TextureFilteringMode GetFilteringMode() const override { return m_FilteringMode; }
		virtual TextureWrapMode GetWrapMode() const { return m_WrapMode; }
		virtual TextureFormat GetFormat() const { return m_Format; }

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual uint32_t GetID() const { return m_Id; }
		virtual bool operator ==(const Texture2D& other) const override;

		static GLenum GetFilteringMode(TextureFilteringMode filtering);
		static GLenum GetWrapMode(TextureWrapMode wrapMode);
		static GLenum GetInternalFormat(TextureFormat format);
		static GLenum GetDataFormat(TextureFormat format);
		static GLenum GetDataType(TextureFormat format);

	private:
		std::string m_Path;
		uint32_t m_Width, m_Height, m_Id;
		TextureFilteringMode m_FilteringMode;
		TextureWrapMode m_WrapMode;
		TextureFormat m_Format;
	};
}
