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

	enum class TextureFilter
	{
		NEAREST, LINEAR
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

		bool operator==(const Texture& other) const { return getRendererID() == other.getRendererID(); }

		static uint32_t calculateMipMapLevelsNeeded(const uint32_t width, const uint32_t height);
		static uint32_t getBPP(const TextureFormat format);
	};

	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		static Reference<Texture2D> create(TextureFormat format, uint32_t width, uint32_t height, TextureFilter magFilter = TextureFilter::LINEAR, TextureFilter minFilter = TextureFilter::LINEAR, TextureWrap wrap = TextureWrap::CLAMP_TO_EDGE);
		static Reference<Texture2D> create(const std::string& filepath, const bool SRGB = false, TextureFilter magFilter = TextureFilter::LINEAR, TextureFilter minFilter = TextureFilter::LINEAR, const TextureWrap wrap = TextureWrap::CLAMP_TO_BORDER);

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

	class Texture2DSubTexture
	{
	public:
		Texture2DSubTexture(const Reference<Texture2D>& textureAtlas, uint32_t textureAtlasCellSize, const glm::vec2& textureAtlasIndex, const glm::vec2& scale = glm::vec2(1.0f));
		~Texture2DSubTexture() = default;

		Reference<Texture2D> getTextureAtlas() const { return m_textureAtlas; }
		uint32_t getCellSize() const { return m_textureAtlasCellSize; }
		const glm::mat4x2& getTextureAtlasIndex() const { return m_textureAtlasCoordinates; }

	private:
		Reference<Texture2D> m_textureAtlas;
		uint32_t m_textureAtlasCellSize;
		glm::mat4x2 m_textureAtlasCoordinates;
	};

}