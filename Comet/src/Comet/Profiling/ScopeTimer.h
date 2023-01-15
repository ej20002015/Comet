#pragma once
#include "CometPCH.h"

#include <chrono>

namespace Comet
{

	class ScopeTimer
	{
	public:
		ScopeTimer(const std::string& timerName = "");
		~ScopeTimer();

		static void logTime(bool log) { s_logTime = log; }

	private:
		const std::chrono::high_resolution_clock::time_point m_startTime;
		std::string m_timerName;

		static bool s_logTime;
	};

}