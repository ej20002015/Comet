#include "CometPCH.h"
#include "Texture.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Comet
{

    const std::string Texture::s_noFilepathName = "NONE";

    std::unordered_map<std::string, Reference<Texture>> TextureManager::s_texturePool;

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

        Reference<Texture> texture = TextureManager::getTexture(filepath);
        if (texture)
            return std::static_pointer_cast<Texture2D>(texture);

        switch (RendererAPI::getCurrrentRendererAPIType())
        {
        case RendererAPIType::OPENGL:
        {
            texture = createReference<OpenGLTexture2D>(filepath, SRGB, magFilter, minFilter, wrap);
            TextureManager::addTexture(texture);
            return std::static_pointer_cast<Texture2D>(texture);
            break;
        }
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

        Reference<Texture> texture = TextureManager::getTexture(filepath);
        if (texture)
            return std::static_pointer_cast<TextureCube>(texture);

        switch (RendererAPI::getCurrrentRendererAPIType())
        {
        case RendererAPIType::OPENGL:
        {
            texture = createReference<OpenGLTextureCube>(filepath, SRGB);
            TextureManager::addTexture(texture);
            return std::static_pointer_cast<TextureCube>(texture);
            break;
        }
        default:
            CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
            return nullptr;
            break;
        }
    }

    Texture2DSubTexture::Texture2DSubTexture(const Reference<Texture2D>& textureAtlas, uint32_t textureAtlasCellSize, const glm::vec2& textureAtlasIndex, const glm::vec2& textureScale)
        : m_textureAtlas(textureAtlas), m_textureAtlasCellSize(textureAtlasCellSize), m_textureAtlasIndex(textureAtlasIndex), m_textureScale(textureScale)
    {
        calculateTextureAtlasCoordinates();
    }

    void Texture2DSubTexture::calculateTextureAtlasCoordinates()
    {
        float textureWidth = static_cast<float>(m_textureAtlas->getWidth());
        float textureHeight = static_cast<float>(m_textureAtlas->getHeight());
        //Needed to prevent flickering
        float pixelCorrectionOffset = 0.5f;
        //Bottom left
        m_textureAtlasCoordinates[0] = { (((m_textureAtlasIndex.x * m_textureAtlasCellSize)) + pixelCorrectionOffset) / textureWidth, (((m_textureAtlasIndex.y * m_textureAtlasCellSize) + pixelCorrectionOffset) / textureHeight) };
        //Bottom right
        m_textureAtlasCoordinates[1] = { (((m_textureAtlasIndex.x + m_textureScale.x) * m_textureAtlasCellSize) - pixelCorrectionOffset) / textureWidth, (((m_textureAtlasIndex.y * m_textureAtlasCellSize)) + pixelCorrectionOffset) / textureHeight };
        //Top right
        m_textureAtlasCoordinates[2] = { (((m_textureAtlasIndex.x + m_textureScale.x) * m_textureAtlasCellSize) - pixelCorrectionOffset) / textureWidth, (((m_textureAtlasIndex.y + m_textureScale.y) * m_textureAtlasCellSize) - pixelCorrectionOffset) / textureHeight };
        //Top left
        m_textureAtlasCoordinates[3] = { (((m_textureAtlasIndex.x * m_textureAtlasCellSize)) + pixelCorrectionOffset) / textureWidth, (((m_textureAtlasIndex.y + m_textureScale.y) * m_textureAtlasCellSize) - pixelCorrectionOffset) / textureHeight };
    }

    void TextureManager::addTexture(const Reference<Texture>& texture)
    {
        const std::string& filepath = texture->getFilepath();
        if (filepath == Texture::s_noFilepathName)
        {
            Log::cometError("Cannot add textures created during runtime to the Texture Manager");
            return;
        }

        s_texturePool[texture->getFilepath()] = texture;
    }

    Reference<Texture> TextureManager::getTexture(const std::string& filepath)
    {
        auto textureIterator = s_texturePool.find(filepath);
        return (textureIterator != s_texturePool.end()) ? textureIterator->second : nullptr;
    }

}
