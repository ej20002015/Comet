#include "CometPCH.h"
#include "Texture.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Comet
{

const std::filesystem::path Texture::NO_FILEPATH_NAME= "NONE";
const UnorderedStrSet Texture::SUPPORTED_IMG_FILE_TYPES = { ".png", ".jpg" };
std::unordered_map<std::filesystem::path, Reference<Texture>> TextureManager::s_texturePool;

//Calculate how many times the largest dimension of the texture can be divided in two to calculate the required number of mip maps
uint32_t Texture::calculateMipMapLevelsNeeded(const uint32_t width, const uint32_t height)
{
    uint32_t levels = 1;
    while ((width | height) >> levels)
        levels++;

    return levels;
}

//Get bytes per pixel for texture format
uint32_t Texture::getBPP(const Format format)
{
	switch (format)
	{
	case Format::RGB:      return 3;
	case Format::RGBA:     return 4;
	case Format::FLOAT16:  return 8;
    case Format::R8:       return 1;
		
	default:
		CMT_COMET_ASSERT_MESSAGE(false, "Unknown texture format");
		return 0;
	}
}

Reference<Texture2D> Texture2D::create(const Format format, const uint32_t width, const uint32_t height, const Filter magFilter, const Filter minFilter, const Wrap wrap)
{
    switch (RendererAPI::getCurrrentRendererAPIType())
    {
    case RendererAPI::Type::OPENGL:
        return createReference<OpenGLTexture2D>(format, width, height, magFilter, minFilter, wrap);
        break;

    default:
        CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
        return nullptr;
        break;
    }
}

Reference<Texture2D> Texture2D::create(const void* const data, const uint32_t size, const Format format, const uint32_t width, const uint32_t height, const Filter magFilter, const Filter minFilter, const Wrap wrap)
{
    Reference<Texture2D> texture = Texture2D::create(format, width, height, magFilter, minFilter, wrap);
    texture->setData(data, size);
    return texture;
}

Reference<Texture2D> Texture2D::create(const std::filesystem::path& filepath, const bool SRGB, const Filter magFilter, const Filter minFilter, const Wrap wrap)
{
    if (filepath.empty())
        throw CometException() << "Texture2D filepath cannot be empty";

    const std::string extension = filepath.extension().string();
    if (SUPPORTED_IMG_FILE_TYPES.find(extension) == SUPPORTED_IMG_FILE_TYPES.end())
        throw CometException() << "Image file extension " << extension << " is not supported by Texture2D";

    Reference<Texture> texture = TextureManager::getTexture(filepath);
    if (texture)
        return std::static_pointer_cast<Texture2D>(texture);

    switch (RendererAPI::getCurrrentRendererAPIType())
    {
    case RendererAPI::Type::OPENGL:
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

Reference<TextureCube> TextureCube::create(const Format textureFormat, const uint32_t width, const uint32_t height)
{
    switch (RendererAPI::getCurrrentRendererAPIType())
    {
    case RendererAPI::Type::OPENGL:
        return createReference<OpenGLTextureCube>(textureFormat, width, height);
        break;

    default:
        CMT_COMET_ASSERT_MESSAGE(false, "Unknown GraphicsAPI");
        return nullptr;
        break;
    }
}

Reference<TextureCube> TextureCube::create(const std::filesystem::path& filepath, const bool SRGB)
{
    if (filepath.empty())
        throw CometException() << "Texture Cube filepath cannot be empty";

    const std::string extension = filepath.extension().string();
    if (SUPPORTED_IMG_FILE_TYPES.find(extension) == SUPPORTED_IMG_FILE_TYPES.end())
        throw CometException() << "Image file extension " << extension << " is not supported by TextureCube";

    Reference<Texture> texture = TextureManager::getTexture(filepath);
    if (texture)
        return std::static_pointer_cast<TextureCube>(texture);

    switch (RendererAPI::getCurrrentRendererAPIType())
    {
    case RendererAPI::Type::OPENGL:
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

Texture2DSubTexture::Texture2DSubTexture(const Reference<Texture2D>& textureAtlas, const uint32_t textureAtlasCellSize, const glm::vec2& textureAtlasIndex, const glm::vec2& textureScale)
    : m_textureAtlas(textureAtlas), m_textureAtlasCellSize(textureAtlasCellSize), m_textureAtlasIndex(textureAtlasIndex), m_textureScale(textureScale)
{
    calculateTextureAtlasCoordinates();
}

void Texture2DSubTexture::calculateTextureAtlasCoordinates()
{
    CMT_COMET_ASSERT_MESSAGE(m_textureAtlas, "Cannot calculate texture atlas coordinates - texture atlas is null");

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
    const std::filesystem::path& filepath = texture->getFilepath();
    if (filepath == Texture::NO_FILEPATH_NAME)
    {
        Log::cometError("Cannot add runtime generated textures to the Texture Manager");
        return;
    }

    s_texturePool[filepath] = texture;
}

Reference<Texture> TextureManager::getTexture(const std::filesystem::path& filepath)
{
    auto textureIterator = s_texturePool.find(filepath);
    return (textureIterator != s_texturePool.end()) ? textureIterator->second : nullptr;
}

}
