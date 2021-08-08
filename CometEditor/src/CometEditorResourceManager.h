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

		static Reference<Texture2D> getTexture(const std::string& textureName);

	private:
		static std::unordered_map<std::string, Reference<Texture2D>> s_textures;
	};

}