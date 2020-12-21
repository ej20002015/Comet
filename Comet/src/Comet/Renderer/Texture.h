#pragma once
#include "CometPCH.h"

#include "RendererAPI.h"

namespace Comet
{

	enum class TextureFormat
	{
		RGB, RGBA, FLOAT16
	};

	enum class TextureWrap
	{
		CLAMP_TO_BORDER, CLAMP_TO_EDGE, REPEAT
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void bind(uint32_t slot = 0) const = 0;

		virtual TextureFormat getTextureFormat() const = 0;
		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
		virtual uint32_t getMipMapLevels() const = 0;
		virtual RendererID getRendererID() const = 0;
		virtual bool getSRGB() const = 0;
		virtual bool getHDR() const = 0;

		static uint32_t calculateMipMapLevelsNeeded(const uint32_t width, const uint32_t height);
		static uint32_t getBPP(const TextureFormat format);
	};

	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		static Reference<Texture2D> create(TextureFormat format, uint32_t width, uint32_t height, TextureWrap wrap = TextureWrap::CLAMP_TO_EDGE);
		static Reference<Texture2D> create(const std::string& filepath, const bool SRGB = false, const TextureWrap wrap = TextureWrap::CLAMP_TO_BORDER);

		virtual void setData(void* data, uint32_t size) = 0;

		virtual const std::string& getFilepath() const = 0;
		virtual TextureWrap getTextureWrap() const = 0;
	};

	class TextureCube : public Texture
	{
	public:
		virtual ~TextureCube() = default;

		static Reference<TextureCube> create(const TextureFormat textureFormat, const uint32_t width, const uint32_t height);
		static Reference<TextureCube> create(const std::string& filepath, const bool SRGB = false);

		virtual const std::string& getFilepath() const = 0;
	};

}