#pragma once
#include "CometPCH.h"

#include <chrono>

namespace Comet
{

class ScopeTimer
{
public:
	ScopeTimer(const std::string_view timerName = "");
	~ScopeTimer();

	static void logTime(const bool log) { s_logTime = log; }

private:
	const std::chrono::high_resolution_clock::time_point m_startTime;
	std::string_view m_timerName;

	static bool s_logTime;
};

}