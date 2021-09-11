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
		void renderHeader();
		void renderFooter();
		void renderDirectoryEntry(const std::filesystem::directory_entry& directoryEntry, float thumbnailSize);

	private:
		//TODO: Set up projects so this can be removed
		const std::filesystem::path m_assetDirectoryPath = "assets";
		std::filesystem::path m_currentDirectory = m_assetDirectoryPath;

		Reference<Texture2D> m_directoryIcon, m_fileIcon, m_backButtonIcon;

		uint32_t m_thumbnailSizeExponent = 6;

		static const float s_buttonPadding;
	};

}