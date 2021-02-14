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

	static GLenum getGLTextureFiltering(const TextureFilter textureFilter, int32_t mipMapLevels = 1)
	{
		switch (textureFilter)
		{
		case TextureFilter::NEAREST:  return GL_NEAREST; break;
		case TextureFilter::LINEAR:   return (mipMapLevels > 1) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR; break;
		default:
			Log::cometError("Unknown texture filtering policy");
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

	OpenGLTexture2D::OpenGLTexture2D(TextureFormat format, uint32_t width, uint32_t height, TextureFilter magFilter, TextureFilter minFilter, TextureWrap wrap)
		: m_textureFormat(format), m_textureWrap(wrap), m_width(width), m_height(height), m_mipMapLevels(Texture::calculateMipMapLevelsNeeded(width, height)), m_SRGB(false), m_filepath(Texture::s_noFilepathName), m_localData(nullptr)
	{
		m_HDR = m_textureFormat == TextureFormat::FLOAT16;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);

		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, getGLTextureFiltering(minFilter, m_mipMapLevels));
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, getGLTextureFiltering(magFilter));
		GLenum textureWrap = getGLTextureWrap(wrap);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, textureWrap);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, textureWrap);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_R, textureWrap);
		glTextureParameterf(m_rendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::getCapabilities().maxAnisotropy);

		glTextureStorage2D(m_rendererID, m_mipMapLevels, getGLInternalTextureFormat(m_textureFormat), width, height);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath, const bool SRGB, TextureFilter magFilter, TextureFilter minFilter, const TextureWrap wrap)
		: m_textureWrap(wrap), m_width(0), m_height(0), m_mipMapLevels(0), m_SRGB(SRGB), m_filepath(filepath)
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

		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, getGLTextureFiltering(minFilter, m_mipMapLevels));
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, getGLTextureFiltering(magFilter));
		GLenum textureWrap = getGLTextureWrap(wrap);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, textureWrap);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, textureWrap);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_R, textureWrap);
		glTextureParameterf(m_rendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::getCapabilities().maxAnisotropy);

		if (SRGB)
		{
			glTextureStorage2D(m_rendererID, m_mipMapLevels, GL_SRGB8, width, height);
			glTextureSubImage2D(m_rendererID, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		}
		else
		{
			GLenum dataType = (m_HDR) ? GL_FLOAT : GL_UNSIGNED_BYTE;
			glTextureStorage2D(m_rendererID, m_mipMapLevels, getGLInternalTextureFormat(m_textureFormat), width, height);
			glTextureSubImage2D(m_rendererID, 0, 0, 0, width, height, getGLTextureFormat(m_textureFormat), dataType, imageData);
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

	void OpenGLTexture2D::setData(void* data, uint32_t size)
	{
		if (size != m_width * m_height * Texture::getBPP(m_textureFormat))
		{
			Log::cometError("Data does not fill the whole texture");
			CMT_COMET_ASSERT(false);
			return;
		}

		m_localData = Buffer::create(data, size);

		GLenum dataType = (m_HDR) ? GL_FLOAT : GL_UNSIGNED_BYTE;
		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, getGLTextureFormat(m_textureFormat), dataType, data);
	}

	//NEEDS TO BE TESTED FULLY - SEEMS TO BE WORKING

	OpenGLTextureCube::OpenGLTextureCube(const TextureFormat textureFormat, const uint32_t width, const uint32_t height)
		: m_textureFormat(textureFormat), m_width(width), m_height(height), m_mipMapLevels(Texture::calculateMipMapLevelsNeeded(width, height)), m_SRGB(false), m_filepath(Texture::s_noFilepathName)
	{
		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_rendererID);

		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, (m_mipMapLevels > 1) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTextureParameterf(m_rendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::getCapabilities().maxAnisotropy);

		GLenum glTextureFormat = getGLInternalTextureFormat(textureFormat);
		m_HDR = glTextureFormat == GL_RGBA16F;
		glTextureStorage2D(m_rendererID, m_mipMapLevels, glTextureFormat, width, height);
	}

	OpenGLTextureCube::OpenGLTextureCube(const std::string& filepath, const bool SRGB)
		: m_filepath(filepath), m_SRGB(SRGB)
	{
		int32_t width, height, BPP;
		void* imageData;

		//openGL expects pixels to start at bottom left so y direction needs to be flipped
		stbi_set_flip_vertically_on_load(1);

		if (stbi_is_hdr(filepath.c_str()))
		{
			Log::cometInfo("Loading HDR Cube Map Texture {0}, SRGB = {1}", filepath, SRGB);
			imageData = stbi_loadf(filepath.c_str(), &width, &height, &BPP, 0);
			m_HDR = true;
			m_textureFormat = TextureFormat::FLOAT16;
		}
		else
		{
			int32_t requiredChannels;;
			Log::cometInfo("Loading SDR Cube Map Texture {0}, SRGB = {1}", filepath, SRGB);

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
			Log::cometError("{0} Cube Map Texture could not be loaded - {1}", filepath, stbi_failure_reason());
			CMT_COMET_ASSERT(false);
			return;
		}

		m_localData = Buffer::create(imageData, width * height * BPP);

		uint32_t faceWidth = width / 4;
		uint32_t faceHeight = height / 3;
		m_width = faceWidth;
		m_height = faceHeight;
		if (faceWidth != faceHeight)
		{
			Log::cometError("{0} Cube Map Texture does not have square faces", filepath);
			CMT_COMET_ASSERT(false);
			return;
		}

		//Allocate space for each separate face
		std::array<uint8_t*, 6> faces;
		for (uint32_t i = 0; i < 6; ++i)
			faces[i] = new uint8_t[faceWidth * faceHeight * BPP];

		uint32_t faceIndex = 0;

		//Get Image data for the 4 horizontally arranged cube faces
		uint32_t yTopLeft = faceHeight;
		uint32_t xTopLeft = 0;
		for (uint32_t i = 0; i < 4; ++i)
		{
			for (uint32_t y = 0; y < faceHeight; ++y)
			{
				for (uint32_t x = 0; x < faceWidth; ++x)
				{
					for (int32_t b = 0; b < BPP; ++b)
						faces[faceIndex][(y * faceWidth) + x + b] = (*m_localData)[((yTopLeft + y) * faceWidth) + xTopLeft + x + b];
				}
			}
			xTopLeft += faceWidth;
			++faceIndex;
		}

		//Get image data for the remaining 2 vertically arranged cube faces
		yTopLeft = 0;
		xTopLeft = faceWidth * 2;
		for (uint32_t i = 0; i < 3; ++i)
		{
			//Skip middle face as this data has already been pulled
			if (i == 1)
			{
				yTopLeft += faceHeight;
				continue;
			}

			for (uint32_t y = 0; y < faceHeight; ++y)
			{
				for (uint32_t x = 0; x < faceWidth; ++x)
				{
					for (int32_t b = 0; b < BPP; ++b)
						faces[faceIndex][(y * faceWidth) + x + b] = (*m_localData)[((yTopLeft + y) * faceWidth) + xTopLeft + x + b];
				}
			}

			yTopLeft += faceHeight;
			++faceIndex;
		}


		m_mipMapLevels = Texture::calculateMipMapLevelsNeeded(width, height);

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_rendererID);

		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, (m_mipMapLevels > 1) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTextureParameterf(m_rendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::getCapabilities().maxAnisotropy);

		glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID);

		if (SRGB)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_SRGB8, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, faces[2]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_SRGB8, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, faces[0]);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_SRGB8, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, faces[4]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_SRGB8, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, faces[5]);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_SRGB8, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, faces[1]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_SRGB8, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, faces[3]);
		}
		else
		{
			GLenum dataType = (m_HDR) ? GL_FLOAT : GL_UNSIGNED_BYTE;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, getGLInternalTextureFormat(m_textureFormat), faceWidth, faceHeight, 0, getGLTextureFormat(m_textureFormat), dataType, faces[2]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, getGLInternalTextureFormat(m_textureFormat), faceWidth, faceHeight, 0, getGLTextureFormat(m_textureFormat), dataType, faces[0]);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, getGLInternalTextureFormat(m_textureFormat), faceWidth, faceHeight, 0, getGLTextureFormat(m_textureFormat), dataType, faces[4]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, getGLInternalTextureFormat(m_textureFormat), faceWidth, faceHeight, 0, getGLTextureFormat(m_textureFormat), dataType, faces[5]);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, getGLInternalTextureFormat(m_textureFormat), faceWidth, faceHeight, 0, getGLTextureFormat(m_textureFormat), dataType, faces[1]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, getGLInternalTextureFormat(m_textureFormat), faceWidth, faceHeight, 0, getGLTextureFormat(m_textureFormat), dataType, faces[3]);
		}

		glGenerateTextureMipmap(m_rendererID);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		for (uint32_t i = 0; i < 6; ++i)
			delete[] faces[i];

		if (imageData)
			stbi_image_free(imageData);
	}

	OpenGLTextureCube::~OpenGLTextureCube()
	{
		glDeleteTextures(1, &m_rendererID);
	}

	void OpenGLTextureCube::bind(const uint32_t slot) const
	{
		glBindTextureUnit(slot, m_rendererID);
	}

}
