#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/Texture.h"

namespace Comet
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void onImGuiRender();

	private:
		void renderDirectoryEntry(const std::filesystem::directory_entry& directoryEntry, float thumbnailSize);

	private:
		//TODO: Set up projects so this can be removed
		const std::filesystem::path m_AssetDirectoryPath = "assets";
		std::filesystem::path m_currentDirectory = m_AssetDirectoryPath;

		Reference<Texture2D> m_directoryIcon;
		Reference<Texture2D> m_fileIcon;

		uint32_t m_thumbnailSizeExponent = 6;

		static const float s_buttonPadding;
	};

}