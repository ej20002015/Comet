#pragma once
#include "CometPCH.h"

namespace Comet
{
	class ContentBrowserPanel
	{
	public:
		//TODO: Set up projects so this can be removed
		const std::filesystem::path s_AssetDirectoryPath = "assets";

		ContentBrowserPanel() = default;

		void onImGuiRender();

	private:
		std::filesystem::path m_currentDirectory = s_AssetDirectoryPath;
	};

}