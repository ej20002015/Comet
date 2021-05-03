#pragma once
#include "CometPCH.h"

namespace Comet
{

	class PlatformUtilities
	{
	public:
		static std::string openFile(const char* filters);
		static std::string saveFile(const char* filters, const std::string& defaultFileExtension = "");
	};

}