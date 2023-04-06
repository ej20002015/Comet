#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/Texture.h"

namespace Comet
{

class CometEditorResourceManager
{
public:
	CometEditorResourceManager() = delete;

	static void init();

	static Reference<Texture2D> getTexture(const std::string_view textureName);

private:
	static MapStr2T<Reference<Texture2D>> s_textures;
};

}