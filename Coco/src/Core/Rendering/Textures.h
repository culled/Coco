#pragma once
#include "Core/Base.h"
#include <string>

namespace Coco
{
	enum class TextureWrapMode
	{
		None = 0,
		Clamp,
		Repeat
	};

	enum class TextureFilteringMode
	{
		None = 0,
		Linear,
		Nearest
	};

	enum class TextureFormat
	{
		None = 0,
		RGB24,
		RGBA32
	};

	struct COCO_API TextureCreationParameters
	{
		TextureFilteringMode Filtering = TextureFilteringMode::Linear;
		TextureWrapMode WrapMode = TextureWrapMode::Repeat;

		TextureCreationParameters() {}

		TextureCreationParameters(TextureFilteringMode filtering, TextureWrapMode wrapMode) :
			Filtering(filtering), WrapMode(wrapMode) {}
	};

	class COCO_API Texture
	{
	public:
		virtual ~Texture() = default;
		virtual void SetData(void* data, uint32_t size) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual TextureFilteringMode GetFilteringMode() const = 0;
		virtual TextureWrapMode GetWrapMode() const = 0;
		virtual TextureFormat GetFormat() const = 0;
		virtual uint32_t GetID() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class COCO_API Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		virtual uint32_t GetHeight() const = 0;

		virtual bool operator ==(const Texture2D& other) const = 0;

		static Ref<Texture2D> Create(const std::string& path, TextureCreationParameters params = TextureCreationParameters());
		static Texture2D* CreateAsset(const std::string& path);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, TextureFormat format, TextureCreationParameters params = TextureCreationParameters());
	};
}
