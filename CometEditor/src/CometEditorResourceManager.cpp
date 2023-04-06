#include "CometEditorResourceManager.h"

namespace Comet
{

MapStr2T<Reference<Texture2D>> CometEditorResourceManager::s_textures;

void CometEditorResourceManager::init()
{
	s_textures["MenuIcon"] = Texture2D::create("EditorResources/Textures/Icons/EntityPropertiesPanel/MenuIcon.png");
	s_textures["NoTextureIcon"] = Texture2D::create("EditorResources/Textures/Icons/EntityPropertiesPanel/NoTextureIcon.png");

	s_textures["DirectoryIcon"] = Texture2D::create("EditorResources/Textures/Icons/ContentBrowserPanel/DirectoryIcon.png");
	s_textures["FileIcon"] = Texture2D::create("EditorResources/Textures/Icons/ContentBrowserPanel/FileIcon.png");
	s_textures["BackButtonIcon"] = Texture2D::create("EditorResources/Textures/Icons/ContentBrowserPanel/BackButtonIcon.png");

	s_textures["PlayButtonIcon"] = Texture2D::create("EditorResources/Textures/Icons/SceneStateToolbarPanel/PlayButtonIcon.png");
	s_textures["StopButtonIcon"] = Texture2D::create("EditorResources/Textures/Icons/SceneStateToolbarPanel/StopButtonIcon.png");
}

Reference<Texture2D> CometEditorResourceManager::getTexture(const std::string_view textureName)
{
	if (const auto it = s_textures.find(textureName); it != s_textures.end())
		return it->second;
	else
	{
		CMT_COMET_ASSERT_MESSAGE(false, fmt::format("No Comet Editor Resource Texture called {0}", textureName));
		return nullptr;
	}
}

}
