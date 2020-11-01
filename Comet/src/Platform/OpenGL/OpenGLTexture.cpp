#include "CometPCH.h"
#include "OpenGLTexture.h"

#include "glad/glad.h"

#include "stb_image.h"

namespace Comet
{

	static GLenum getGLTextureFormat(const TextureFormat textureFormat)
	{
		switch (textureFormat)
		{
		case TextureFormat::RGB:       return GL_RGB; break;
		case TextureFormat::RGBA:      return GL_RGBA; break;
		case TextureFormat::FLOAT16:   return GL_RGBA; break;
		default:
			Log::cometError("Unknown texture format");
			CMT_COMET_ASSERT(false);
			return 0;
			break;
		}
	}

	static GLenum getGLInternalTextureFormat(const TextureFormat textureFormat)
	{
		switch (textureFormat)
		{
		case TextureFormat::RGB:       return GL_RGB8; break;
		case TextureFormat::RGBA:      return GL_RGBA8; break;
		case TextureFormat::FLOAT16:   return GL_RGBA16F; break;
		default:
			Log::cometError("Unknown texture format");
			CMT_COMET_ASSERT(false);
			return 0;
			break;
		}
	}

	static GLenum getGLTextureWrap(const TextureWrap textureWrap)
	{
		switch (textureWrap)
		{
		case TextureWrap::CLAMP_TO_BORDER:  return GL_CLAMP_TO_BORDER; break;
		case TextureWrap::CLAMP_TO_EDGE:    return GL_CLAMP_TO_EDGE; break;
		case TextureWrap::REPEAT:           return GL_REPEAT; break;
		default:
			Log::cometError("Unknown texture wrap");
			CMT_COMET_ASSERT(false);
			return 0;
			break;
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath, const bool SRGB, const TextureWrap wrap)
		: m_textureWrap(wrap), m_filepath(filepath), m_SRGB(SRGB)
	{
		int32_t width, height, BPP;
		void* imageData;

		//openGL expects pixels to start at bottom left so y direction needs to be flipped
		stbi_set_flip_vertically_on_load(1);

		if (stbi_is_hdr(filepath.c_str()))
		{
			Log::cometInfo("Loading HDR Texture {0}, SRGB = {1}", filepath, SRGB);
			imageData = stbi_loadf(filepath.c_str(), &width, &height, &BPP, 0);
			m_HDR = true;
			m_textureFormat = TextureFormat::FLOAT16;
		}
		else
		{
			int32_t requiredChannels = 0;
			Log::cometInfo("Loading SDR Texture {0}, SRGB = {1}", filepath, SRGB);

			if (SRGB)
			{
				requiredChannels = STBI_rgb;
				m_textureFormat = TextureFormat::RGB;
			}
			else
			{
				requiredChannels = STBI_rgb_alpha;
				m_textureFormat = TextureFormat::RGBA;
			}

			imageData = stbi_load(filepath.c_str(), &width, &height, &BPP, requiredChannels);
			m_HDR = false;
		}

		if (!imageData)
		{
			Log::cometError("{0} Texture could not be loaded - {1}", filepath, stbi_failure_reason());
			CMT_COMET_ASSERT(false);
			return;
		}

		m_localData = Buffer::create(imageData, width * height * BPP);

		m_width = width;
		m_height = height;
		m_mipMapLevels = Texture::calculateMipMapLevelsNeeded(width, height);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);

		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, (m_mipMapLevels > 1) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		GLenum textureWrap = getGLTextureWrap(wrap);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, textureWrap);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, textureWrap);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_R, textureWrap);
		glTextureParameterf(m_rendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::getCapabilities().maxAnisotropy);

		if (SRGB)
		{
			glTextureStorage2D(m_rendererID, m_mipMapLevels, GL_SRGB8, width, height);
			glTextureSubImage2D(m_rendererID, 0, 0, 0, height, width, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		}
		else
		{
			GLenum dataType = (m_HDR) ? GL_FLOAT : GL_UNSIGNED_BYTE;
			glTextureStorage2D(m_rendererID, m_mipMapLevels, getGLInternalTextureFormat(m_textureFormat), width, height);
			glTextureSubImage2D(m_rendererID, 0, 0, 0, height, width, getGLTextureFormat(m_textureFormat), dataType, imageData);
		}

		glGenerateTextureMipmap(m_rendererID);

		if (imageData)
			stbi_image_free(imageData);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_rendererID);
	}

	void OpenGLTexture2D::bind(const uint32_t slot) const
	{
		glBindTextureUnit(slot, m_rendererID);
	}

	OpenGLTextureCube::OpenGLTextureCube(const std::string& filepath, const bool SRGB)
	{
	}

	OpenGLTextureCube::~OpenGLTextureCube()
	{
	}

	void OpenGLTextureCube::bind(const uint32_t slot) const
	{
	}

}
