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

	class Texture2DAtlas
	{
	public:
		Texture2DAtlas(Reference<Texture2D> texture, uint32_t cellSize) : m_texture(texture), m_cellSize(cellSize) {}
		~Texture2DAtlas() = default;

		static Reference<Texture2DAtlas> create(Reference<Texture2D> texture, uint32_t cellSize);
		static Reference<Texture2DAtlas> create(const std::string& filepath, uint32_t cellSize, const bool SRGB = false, TextureFilter magFilter = TextureFilter::LINEAR, TextureFilter minFilter = TextureFilter::LINEAR, const TextureWrap wrap = TextureWrap::CLAMP_TO_BORDER);

		Reference<Texture2D> getTexture() const { return m_texture; }

		uint32_t getCellSize() const { return m_cellSize; }

	private:
		Reference<Texture2D> m_texture;
		uint32_t m_cellSize;

		friend class Texture2DSubTexture;
	};

	class Texture2DSubTexture
	{
	public:
		Texture2DSubTexture(Reference<Texture2DAtlas> textureAtlas, const glm::vec2& offset, const glm::vec2& scale = glm::vec2(1.0f));
		~Texture2DSubTexture() = default;

		static Reference<Texture2DSubTexture> create(Reference<Texture2DAtlas> textureAtlas, const glm::vec2& offset, const glm::vec2& scale = glm::vec2(1.0f));

		Reference<Texture2D> getTexture() const { return m_textureAtlas->m_texture; }
		const glm::mat4x2& getTextureCoordinates() const { return m_textureCoordinates; }

		Reference<Texture2DAtlas> getTextureAtlas() const { return m_textureAtlas; }

	private:
		Reference<Texture2DAtlas> m_textureAtlas;
		glm::mat4x2 m_textureCoordinates;
	};

}