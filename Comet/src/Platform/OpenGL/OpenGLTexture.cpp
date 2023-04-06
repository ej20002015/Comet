#include "CometPCH.h"
#include "OpenGLTexture.h"

#include "glad/glad.h"

#include "stb_image.h"

namespace Comet
{

static Buffer loadImage(const std::filesystem::path& filepath, const bool isSRGB,
						int32_t& out_width, int32_t& out_height, int32_t& out_BPP, Texture::Format& out_format, bool& out_isHDR);

static GLenum getGLTextureFormat(const Texture::Format textureFormat)
{
	switch (textureFormat)
	{
	case Texture::Format::RGB:       return GL_RGB; break;
	case Texture::Format::RGBA:      return GL_RGBA; break;
	case Texture::Format::FLOAT16:   return GL_RGBA; break;
	default:
		CMT_COMET_ASSERT_MESSAGE(false, "Unknown texture format");
		return 0;
		break;
	}
}

static GLenum getGLInternalTextureFormat(const Texture::Format textureFormat)
{
	switch (textureFormat)
	{
	case Texture::Format::RGB:       return GL_RGB8; break;
	case Texture::Format::RGBA:      return GL_RGBA8; break;
	case Texture::Format::FLOAT16:   return GL_RGBA16F; break;
	default:
		CMT_COMET_ASSERT_MESSAGE(false, "Unknown texture format");
		return 0;
		break;
	}
}

static GLenum getGLTextureFiltering(const Texture::Filter textureFilter, const int32_t mipMapLevels = 1)
{
	switch (textureFilter)
	{
	case Texture::Filter::NEAREST:  return GL_NEAREST; break;
	case Texture::Filter::LINEAR:   return (mipMapLevels > 1) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR; break;
	default:
		CMT_COMET_ASSERT_MESSAGE(false, "Unknown texture filtering policy");
		return 0;
		break;
	}
}

static GLenum getGLTextureWrap(const Texture::Wrap textureWrap)
{
	switch (textureWrap)
	{
	case Texture::Wrap::CLAMP_TO_BORDER:  return GL_CLAMP_TO_BORDER; break;
	case Texture::Wrap::CLAMP_TO_EDGE:    return GL_CLAMP_TO_EDGE; break;
	case Texture::Wrap::REPEAT:           return GL_REPEAT; break;
	default:
		CMT_COMET_ASSERT_MESSAGE(false, "Unknown texture wrap");
		return 0;
		break;
	}
}

OpenGLTexture2D::OpenGLTexture2D(const Format format, const uint32_t width, const uint32_t height, const Filter magFilter, const Filter minFilter, const Wrap wrap)
	: m_textureFormat(format), m_textureWrap(wrap), m_textureMagFilter(magFilter), m_textureMinFilter(minFilter), m_width(width), m_height(height), 
	  m_mipMapLevels(Texture::calculateMipMapLevelsNeeded(width, height)), m_isSRGB(false), m_filepath(Texture::NO_FILEPATH_NAME), m_isHDR(m_textureFormat == Format::FLOAT16)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);

	glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, getGLTextureFiltering(minFilter, m_mipMapLevels));
	glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, getGLTextureFiltering(magFilter));
	const GLenum textureWrap = getGLTextureWrap(wrap);
	glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, textureWrap);
	glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, textureWrap);
	glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_R, textureWrap);
	glTextureParameterf(m_rendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::getCapabilities().maxAnisotropy);

	glTextureStorage2D(m_rendererID, m_mipMapLevels, getGLInternalTextureFormat(m_textureFormat), width, height);
}

OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& filepath, const bool SRGB, Filter magFilter, Filter minFilter, const Wrap wrap)
	: m_textureWrap(wrap), m_textureMagFilter(magFilter), m_textureMinFilter(minFilter), m_width(0), m_height(0), m_mipMapLevels(0), m_isSRGB(SRGB), m_filepath(filepath)
{
	Log::cometInfo("Creating Texture {0} with SRGB = {1}", filepath.string(), m_isSRGB);

	int32_t width, height, BPP;
	m_localData = loadImage(filepath, m_isSRGB, width, height, BPP, m_textureFormat, m_isHDR);

	m_width = static_cast<uint32_t>(width);
	m_height = static_cast<uint32_t>(height);
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
		glTextureSubImage2D(m_rendererID, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, m_localData.getData());
	}
	else
	{
		GLenum dataType = (m_isHDR) ? GL_FLOAT : GL_UNSIGNED_BYTE;
		glTextureStorage2D(m_rendererID, m_mipMapLevels, getGLInternalTextureFormat(m_textureFormat), width, height);
		glTextureSubImage2D(m_rendererID, 0, 0, 0, width, height, getGLTextureFormat(m_textureFormat), dataType, m_localData.getData());
	}

	glGenerateTextureMipmap(m_rendererID);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &m_rendererID);
}

void OpenGLTexture2D::bind(const uint32_t slot) const
{
	glBindTextureUnit(slot, m_rendererID);
}

void OpenGLTexture2D::setData(const void* const data, const uint32_t size)
{
	if (size != m_width * m_height * Texture::getBPP(m_textureFormat))
		throw CometException() << "Data does not fill the whole texture";

	m_localData = Buffer(data, size);

	GLenum dataType = (m_isHDR) ? GL_FLOAT : GL_UNSIGNED_BYTE;
	glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, getGLTextureFormat(m_textureFormat), dataType, data);
}

//NEEDS TO BE TESTED FULLY - SEEMS TO BE WORKING

OpenGLTextureCube::OpenGLTextureCube(const Format textureFormat, const uint32_t width, const uint32_t height)
	: m_textureFormat(textureFormat), m_width(width), m_height(height), m_mipMapLevels(Texture::calculateMipMapLevelsNeeded(width, height)), m_isSRGB(false), m_filepath(Texture::NO_FILEPATH_NAME)
{
	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_rendererID);

	glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, (m_mipMapLevels > 1) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTextureParameterf(m_rendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::getCapabilities().maxAnisotropy);

	GLenum glTextureFormat = getGLInternalTextureFormat(textureFormat);
	m_isHDR = glTextureFormat == GL_RGBA16F;
	glTextureStorage2D(m_rendererID, m_mipMapLevels, glTextureFormat, width, height);
}

OpenGLTextureCube::OpenGLTextureCube(const std::filesystem::path& filepath, const bool SRGB)
	: m_filepath(filepath), m_isSRGB(SRGB)
{
	Log::cometInfo("Creating Texture Cube {0} with SRGB = {1}", filepath.string(), m_isSRGB);

	int32_t width, height, BPP;
	m_localData = loadImage(filepath, m_isSRGB, width, height, BPP, m_textureFormat, m_isHDR);

	uint32_t faceWidth = static_cast<uint32_t>(width) / 4;
	uint32_t faceHeight = static_cast<uint32_t>(height) / 3;

	if (faceWidth != faceHeight)
		throw CometException() << fmt::format("'{0}' Cube Map Texture does not have square faces", m_filepath.string());

	m_width = faceWidth;
	m_height = faceHeight;

	//Allocate space for each separate face
	std::array<Buffer, 6> faces;
	for (uint32_t i = 0; i < 6; ++i)
		faces[i] = Buffer(faceWidth * faceHeight * BPP);

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
				{
					Buffer& faceData = faces[faceIndex];
					faceData[(y * faceWidth) + x + b] = m_localData[((yTopLeft + y) * faceWidth) + xTopLeft + x + b];
				}
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
				{
					Buffer& faceData = faces[faceIndex];
					faceData[(y * faceWidth) + x + b] = m_localData[((yTopLeft + y) * faceWidth) + xTopLeft + x + b];
				}
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
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_SRGB8, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, faces[2].getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_SRGB8, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, faces[0].getData());

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_SRGB8, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, faces[4].getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_SRGB8, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, faces[5].getData());

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_SRGB8, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, faces[1].getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_SRGB8, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, faces[3].getData());
	}
	else
	{
		GLenum dataType = (m_isHDR) ? GL_FLOAT : GL_UNSIGNED_BYTE;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, getGLInternalTextureFormat(m_textureFormat), faceWidth, faceHeight, 0, getGLTextureFormat(m_textureFormat), dataType, faces[2].getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, getGLInternalTextureFormat(m_textureFormat), faceWidth, faceHeight, 0, getGLTextureFormat(m_textureFormat), dataType, faces[0].getData());

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, getGLInternalTextureFormat(m_textureFormat), faceWidth, faceHeight, 0, getGLTextureFormat(m_textureFormat), dataType, faces[4].getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, getGLInternalTextureFormat(m_textureFormat), faceWidth, faceHeight, 0, getGLTextureFormat(m_textureFormat), dataType, faces[5].getData());

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, getGLInternalTextureFormat(m_textureFormat), faceWidth, faceHeight, 0, getGLTextureFormat(m_textureFormat), dataType, faces[1].getData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, getGLInternalTextureFormat(m_textureFormat), faceWidth, faceHeight, 0, getGLTextureFormat(m_textureFormat), dataType, faces[3].getData());
	}

	glGenerateTextureMipmap(m_rendererID);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

OpenGLTextureCube::~OpenGLTextureCube()
{
	glDeleteTextures(1, &m_rendererID);
}

void OpenGLTextureCube::bind(const uint32_t slot) const
{
	glBindTextureUnit(slot, m_rendererID);
}

static Buffer loadImage(const std::filesystem::path& filepath, const bool isSRGB,
						int32_t& out_width, int32_t& out_height, int32_t& out_BPP, Texture::Format& out_format, bool& out_isHDR)
{
	void* imageData;

	//OpenGL expects pixels to start at bottom left so y direction needs to be flipped
	stbi_set_flip_vertically_on_load(1);

	const std::string filepathStr = filepath.string();
	if (stbi_is_hdr(filepathStr.c_str()))
	{
		imageData = stbi_loadf(filepathStr.c_str(), &out_width, &out_height, &out_BPP, 0);
		out_isHDR = true;
		out_format = Texture::Format::FLOAT16;
	}
	else
	{
		int32_t requiredChannels = 0;

		if (isSRGB)
		{
			requiredChannels = STBI_rgb;
			out_format = Texture::Format::RGB;
		}

		imageData = stbi_load(filepathStr.c_str(), &out_width, &out_height, &out_BPP, requiredChannels);
		out_format = out_BPP == 4 ? Texture::Format::RGBA : Texture::Format::RGB;
		out_isHDR = false;
	}

	if (!imageData)
		throw CometException() << "'" << filepathStr << "' Texture could not be loaded - " << stbi_failure_reason();

	Buffer buf(imageData, out_width * out_height * out_BPP);
	stbi_image_free(imageData);
	return buf;
}

}
