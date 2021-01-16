#pragma once
#include "CometPCH.h"

#include <iomanip>

namespace Comet
{

	class UUID
	{
	public:
		UUID();
		UUID(const UUID& other) = default;
		~UUID() = default;

		operator const uint64_t() const { return m_ID; }

		operator const std::string() const
		{
			std::stringstream stream;
			stream << std::hex
				<< std::setfill('0')
				<< std::setw(16)
				<< m_ID;

			std::string unformattedHexString = stream.str();
			unformattedHexString.insert({ 4 }, "-");
			unformattedHexString.insert({ 9 }, "-");
			unformattedHexString.insert({ 14 }, "-");
			return unformattedHexString;
		}

	private:
		uint64_t m_ID;
	};

}

//Need to define how to hash UUID objects for use in maps and other data structures

namespace std
{

	template<>
	struct hash<Comet::UUID>
	{
		std::size_t operator()(const Comet::UUID& UUID) const
		{
			return hash<uint64_t>()(static_cast<uint64_t>(UUID));
		}
	};

}