#include "CometPCH.h"
#include "UUID.h"

#include <random>

namespace Comet
{

	static std::random_device s_randomDevice;
	static std::mt19937_64 s_eng(s_randomDevice());
	static std::uniform_int_distribution<uint64_t> s_uniformDistribution;

	UUID::UUID()
		: m_ID(s_uniformDistribution(s_eng))
	{
	}

}
