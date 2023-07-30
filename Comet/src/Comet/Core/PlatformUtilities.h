#pragma once
#include "CometPCH.h"

namespace Comet
{

class PlatformUtilities
{
public:
	//TODO: Determine if filters are going to be platform specific (I suspect they will be), and if so create some sort of platform to filetype/filters mapping
	static std::string openFile(const Buffer& filters);
	static std::string saveFile(const Buffer& filters, const std::string_view defaultFileExtension = "");

	static Buffer constructFilter(const std::unordered_map<std::string, std::string> fileType2ExtensionsMap, bool includeAllFiles = true);

	template<std::ranges::forward_range Range>
	static Buffer constructFilter(const Range& fileExtensions, const std::string_view filterLabel, bool includeAllFiles = true)
		requires std::same_as<std::ranges::range_value_t<Range>, std::string>;
};

template<std::ranges::forward_range Range>
inline Buffer PlatformUtilities::constructFilter(const Range& fileExtensions, const std::string_view filterLabel, bool includeAllFiles)
	requires std::same_as<std::ranges::range_value_t<Range>, std::string>
{
	std::stringstream ss;
	for (const auto& fileExtension : fileExtensions)
		ss << "*" << fileExtension << ";";

	std::string fileExtensionsStr = ss.str();
	fileExtensionsStr.pop_back(); // Remove trailing ;

	std::unordered_map<std::string, std::string> imgFilterFilterMap = { { filterLabel.data(), fileExtensionsStr}};

	return PlatformUtilities::constructFilter(imgFilterFilterMap, includeAllFiles);
}

}