#include "CometPCH.h"
#include "ScopeTimer.h"

namespace Comet
{

bool ScopeTimer::s_logTime = true;

ScopeTimer::ScopeTimer(const std::string_view timerName)
	: m_timerName(timerName),
		m_startTime(std::chrono::high_resolution_clock::now())
{
	if (s_logTime)
		Log::cometInfo("Starting Scope Timer: {0}", m_timerName);
}

ScopeTimer::~ScopeTimer()
{
	const auto endTime = std::chrono::high_resolution_clock::now();
	const auto duration = endTime - m_startTime;
	if (s_logTime)
		Log::cometInfo("Ending Scope Timer: {0} - Time elapsed = {1}ms", m_timerName, duration.count() / 1000000.0f);
}

}
