#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/Texture.h"
#include "Comet/Core/Buffer.h"

namespace Comet
{

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(TextureFormat format, uint32_t width, uint32_t height, TextureWrap wrap = TextureWrap::CLAMP_TO_EDGE);
		OpenGLTexture2D(const std::string& filepath, const bool SRGB = false, const TextureWrap wrap = TextureWrap::CLAMP_TO_BORDER);
		~OpenGLTexture2D() override;

		void bind(const uint32_t slot = 0) const override;

		void setData(void* data, uint32_t size) override;

		TextureFormat getTextureFormat() const override { return m_textureFormat; }
		uint32_t getWidth() const override { return m_width; }
		uint32_t getHeight() const override { return m_height; }
		uint32_t getMipMapLevels() const override { return m_mipMapLevels; }
		RendererID getRendererID() const override { return m_rendererID; }
		const std::string& getFilepath() const override { return m_filepath; }
		TextureWrap getTextureWrap() const override { return m_textureWrap; }
		virtual bool getSRGB() const override { return m_SRGB; }
		virtual bool getHDR() const override { return m_HDR; }

	private:
		RendererID m_rendererID = 0;
		TextureFormat m_textureFormat;
		TextureWrap m_textureWrap;
		uint32_t m_width, m_height;
		uint32_t m_mipMapLevels;
		std::string m_filepath;
		bool m_SRGB;
		bool m_HDR;
		
		Unique<Buffer> m_localData;
	};

	class OpenGLTextureCube : public TextureCube
	{
	public:
		OpenGLTextureCube(const TextureFormat textureFormat, const uint32_t width, const uint32_t height);
		OpenGLTextureCube(const std::string& filepath, const bool SRGB = false);
		~OpenGLTextureCube() override;

		void bind(const uint32_t slot = 0) const override;

		TextureFormat getTextureFormat() const override { return m_textureFormat; }
		uint32_t getWidth() const override { return m_width; }
		uint32_t getHeight() const override { return m_height; }
		uint32_t getMipMapLevels() const override { return m_mipMapLevels; }
		RendererID getRendererID() const override { return m_rendererID; }
		const std::string& getFilepath() const override { return m_filepath; }
		virtual bool getSRGB() const override { return m_SRGB; }
		virtual bool getHDR() const override { return m_HDR; }

	private:
		RendererID m_rendererID = 0;
		TextureFormat m_textureFormat;
		uint32_t m_width, m_height;
		uint32_t m_mipMapLevels;
		std::string m_filepath;
		bool m_SRGB;
		bool m_HDR;

		Unique<Buffer> m_localData;
	};

}