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

    Reference<Texture2D> Texture2D::create(TextureFormat format, uint32_t width, uint32_t height, TextureFilter magFilter, TextureFilter minFilter, TextureWrap wrap)
    {
        switch (RendererAPI::getCurrrentRendererAPIType())
        {
        case RendererAPIType::OPENGL:
            return createReference<OpenGLTexture2D>(format, width, height, magFilter, minFilter, wrap);
            break;

        default:
            CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
            return nullptr;
            break;
        }
    }

    Reference<Texture2D> Texture2D::create(const std::string& filepath, const bool SRGB, TextureFilter magFilter, TextureFilter minFilter, const TextureWrap wrap)
    {
        CMT_COMET_ASSERT_MESSAGE(filepath.size(), "filepath is empty");

        switch (RendererAPI::getCurrrentRendererAPIType())
        {
        case RendererAPIType::OPENGL:
            return createReference<OpenGLTexture2D>(filepath, SRGB, magFilter, minFilter, wrap);
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

    Reference<Texture2DAtlas> Texture2DAtlas::create(Reference<Texture2D> texture, uint32_t cellSize)
    {
        return createReference<Texture2DAtlas>(texture, cellSize);
    }

    Reference<Texture2DAtlas> Texture2DAtlas::create(const std::string& filepath, const uint32_t cellSize, const bool SRGB, TextureFilter magFilter, TextureFilter minFilter, const TextureWrap wrap)
    {
        auto texture = Texture2D::create(filepath, SRGB, magFilter, minFilter, wrap);
        return createReference<Texture2DAtlas>(texture, cellSize);
    }

    Texture2DSubTexture::Texture2DSubTexture(Reference<Texture2DAtlas> textureAtlas, const glm::vec2& offset, const glm::vec2& scale)
        : m_textureAtlas(textureAtlas)
    {
        uint32_t cellSize = m_textureAtlas->m_cellSize;
        float textureWidth = static_cast<float>(m_textureAtlas->m_texture->getWidth());
        float textureHeight = static_cast<float>(m_textureAtlas->m_texture->getHeight());
        //Needed to prevent flickering
        float pixelCorrectionOffset = 0.5f;
        //Bottom left
        m_textureCoordinates[0] = { (((offset.x * cellSize)) + pixelCorrectionOffset) / textureWidth, (((offset.y * cellSize) + pixelCorrectionOffset) / textureHeight) };
        //Bottom right
        m_textureCoordinates[1] = { (((offset.x + scale.x) * cellSize) - pixelCorrectionOffset) / textureWidth, (((offset.y * cellSize)) + pixelCorrectionOffset) / textureHeight };
        //Top right
        m_textureCoordinates[2] = { (((offset.x + scale.x) * cellSize) - pixelCorrectionOffset) / textureWidth, (((offset.y + scale.y) * cellSize) - pixelCorrectionOffset) / textureHeight };
        //Top left
        m_textureCoordinates[3] = { (((offset.x * cellSize)) + pixelCorrectionOffset) / textureWidth, (((offset.y + scale.y) * cellSize) - pixelCorrectionOffset) / textureHeight };
    }

    Reference<Texture2DSubTexture> Texture2DSubTexture::create(Reference<Texture2DAtlas> textureAtlas, const glm::vec2& offset, const glm::vec2& scale)
    {
        return createReference<Texture2DSubTexture>(textureAtlas, offset, scale);
    }

}
