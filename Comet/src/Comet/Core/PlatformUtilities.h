#pragma once
#include "CometPCH.h"

namespace Comet
{

	class PlatformUtilities
	{
	public:
		//TODO: Determine if filters are going to be platform specific (I suspect they will be), and if so create some sort of platform to filetype/filters mapping
		static std::string openFile(const char* filters);
		static std::string saveFile(const char* filters, const std::string& defaultFileExtension = "");
	};

}