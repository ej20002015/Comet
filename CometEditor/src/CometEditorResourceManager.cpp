#include "CometEditorResourceManager.h"

namespace Comet
{

	std::unordered_map<std::string, Reference<Texture2D>> CometEditorResourceManager::s_textures;

	void CometEditorResourceManager::init()
	{
		s_textures["MenuIcon"] = Texture2D::create("EditorResources/Textures/Icons/EntityPropertiesPanel/MenuIcon.png");
		s_textures["DirectoryIcon"] = Texture2D::create("EditorResources/Textures/Icons/ContentBrowserPanel/DirectoryIcon.png");
		s_textures["FileIcon"] = Texture2D::create("EditorResources/Textures/Icons/ContentBrowserPanel/FileIcon.png");
		s_textures["BackButtonIcon"] = Texture2D::create("EditorResources/Textures/Icons/ContentBrowserPanel/BackButtonIcon.png");
		s_textures["NoTextureIcon"] = Texture2D::create("EditorResources/Textures/Icons/EntityPropertiesPanel/NoTextureIcon.png");
	}

	Reference<Texture2D> CometEditorResourceManager::getTexture(const std::string& textureName)
	{
		if (s_textures.find(textureName) != s_textures.end())
			return s_textures.at(textureName);
		else
		{
			CMT_COMET_ASSERT_MESSAGE(false, "No Comet Editor Resource Texture called " + textureName);
			return nullptr;
		}
	}

}
