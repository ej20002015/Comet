#include "CometPCH.h"
#include "Texture.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Comet
{

    //Calculate how many times the largest dimension of the texture can be divided in two to calculate the required number of mip maps
    uint32_t Texture::calculateMipMapLevelsNeeded(const uint32_t width, const uint32_t height)
    {
        uint32_t levels = 1;
        while ((width | height) >> levels)
            levels++;

        return levels;
    }

    //Get bytes per pixel for texture format
    uint32_t Texture::getBPP(const TextureFormat format)
    {
		switch (format)
		{
		case TextureFormat::RGB:      return 3;
		case TextureFormat::RGBA:     return 4;
		case TextureFormat::FLOAT16:  return 8;
		
		default:
			CMT_COMET_ASSERT_MESSAGE(false, "Unknown Texture format");
			return 0;
		}
    }

    Reference<Texture2D> Texture2D::create(TextureFormat format, uint32_t width, uint32_t height, TextureWrap wrap)
    {
        switch (RendererAPI::getCurrrentRendererAPIType())
        {
        case RendererAPIType::OPENGL:
            return createReference<OpenGLTexture2D>(format, width, height, wrap);
            break;

        default:
            CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
            return nullptr;
            break;
        }
    }

    Reference<Texture2D> Texture2D::create(const std::string& filepath, const bool SRGB, const TextureWrap wrap)
    {
        CMT_COMET_ASSERT_MESSAGE(filepath.size(), "filepath is empty");

        switch (RendererAPI::getCurrrentRendererAPIType())
        {
        case RendererAPIType::OPENGL:
            return createReference<OpenGLTexture2D>(filepath, SRGB, wrap);
            break;

        default:
            CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
            return nullptr;
            break;
        }
    }

    Reference<TextureCube> TextureCube::create(const TextureFormat textureFormat, const uint32_t width, const uint32_t height)
    {
        switch (RendererAPI::getCurrrentRendererAPIType())
        {
        case RendererAPIType::OPENGL:
            return createReference<OpenGLTextureCube>(textureFormat, width, height);
            break;

        default:
            CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
            return nullptr;
            break;
        }
    }

    Reference<TextureCube> TextureCube::create(const std::string& filepath, const bool SRGB)
    {
        CMT_COMET_ASSERT_MESSAGE(filepath.size(), "filepath is empty");

        switch (RendererAPI::getCurrrentRendererAPIType())
        {
        case RendererAPIType::OPENGL:
            return createReference<OpenGLTextureCube>(filepath, SRGB);
            break;

        default:
            CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
            return nullptr;
            break;
        }
    }

}
