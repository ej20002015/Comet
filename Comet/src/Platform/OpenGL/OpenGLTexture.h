#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/Texture.h"
#include "Comet/Core/Buffer.h"

namespace Comet
{

class OpenGLTexture2D : public Texture2D
{
public:
	OpenGLTexture2D(const Format format, const uint32_t width, const uint32_t height, const Filter magFilter = Filter::LINEAR, const Filter minFilter = Filter::LINEAR, const Wrap wrap = Wrap::CLAMP_TO_EDGE);
	OpenGLTexture2D(const std::filesystem::path& filepath, const bool SRGB = false, const Filter magFilter = Filter::LINEAR, const Filter minFilter = Filter::LINEAR, const Wrap wrap = Wrap::CLAMP_TO_BORDER);
	~OpenGLTexture2D() override;

	void bind(const uint32_t slot = 0) const override;

	void setData(const void* const data, const uint32_t size) override;

	Format getTextureFormat() const override { return m_textureFormat; }
	uint32_t getWidth() const override { return m_width; }
	uint32_t getHeight() const override { return m_height; }
	uint32_t getMipMapLevels() const override { return m_mipMapLevels; }
	RendererID getRendererID() const override { return m_rendererID; }
	const std::filesystem::path& getFilepath() const override { return m_filepath; }
	Wrap getTextureWrap() const override { return m_textureWrap; }
	Filter getTextureMagFilter() const override { return m_textureMagFilter; }
	Filter getTextureMinFilter() const override { return m_textureMinFilter; }
	virtual bool getSRGB() const override { return m_isSRGB; }
	virtual bool getHDR() const override { return m_isHDR; }

private:
	RendererID m_rendererID = 0;
	Format m_textureFormat;
	Filter m_textureMagFilter, m_textureMinFilter;
	Wrap m_textureWrap;
	uint32_t m_width, m_height;
	uint32_t m_mipMapLevels;
	std::filesystem::path m_filepath;
	bool m_isSRGB;
	bool m_isHDR;
		
	Buffer m_localData;
};

class OpenGLTextureCube : public TextureCube
{
public:
	OpenGLTextureCube(const Format textureFormat, const uint32_t width, const uint32_t height);
	OpenGLTextureCube(const std::filesystem::path& filepath, const bool SRGB = false);
	~OpenGLTextureCube() override;

	void bind(const uint32_t slot = 0) const override;

	Format getTextureFormat() const override { return m_textureFormat; }
	uint32_t getWidth() const override { return m_width; }
	uint32_t getHeight() const override { return m_height; }
	uint32_t getMipMapLevels() const override { return m_mipMapLevels; }
	RendererID getRendererID() const override { return m_rendererID; }
	const std::filesystem::path& getFilepath() const override { return m_filepath; }
	virtual bool getSRGB() const override { return m_isSRGB; }
	virtual bool getHDR() const override { return m_isHDR; }

private:
	RendererID m_rendererID = 0;
	Format m_textureFormat;
	uint32_t m_width, m_height;
	uint32_t m_mipMapLevels;
	std::filesystem::path m_filepath;
	bool m_isSRGB;
	bool m_isHDR;

	Buffer m_localData;
};

}