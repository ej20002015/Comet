#pragma once
#include "CometPCH.h"

#include "Core.h"

#include "spdlog/spdlog.h"

namespace Comet
{

class Log
{
public:
	static void init();

	enum class LogLevel
	{
		CMT_TRACE, CMT_INFO, CMT_WARN, CMT_ERROR, CMT_CRITICAL
	};

	static void setLevel(const LogLevel level);

	//For logging within the Comet engine
	template<typename... Args>
	inline static void cometTrace(Args&&... args)     { s_cometLogger->trace(std::forward<Args>(args)...); }
	template<typename... Args>					    
	inline static void cometInfo(Args&&... args)      { s_cometLogger->info(std::forward<Args>(args)...); }
	template<typename... Args>					    
	inline static void cometWarn(Args&&... args)      { s_cometLogger->warn(std::forward<Args>(args)...); }
	template<typename... Args>					    
	inline static void cometError(Args&&... args)     { s_cometLogger->error(std::forward<Args>(args)...); }
	template<typename... Args>					    
	inline static void cometCritical(Args&&... args)  { s_cometLogger->critical(std::forward<Args>(args)...); }
		
	//For logging from client applications
	template<typename... Args>					    
	inline static void clientTrace(Args&&... args)    { s_clientLogger->trace(std::forward<Args>(args)...); }
	template<typename... Args>					    
	inline static void clientInfo(Args&&... args)     { s_clientLogger->info(std::forward<Args>(args)...); }
	template<typename... Args>					    
	inline static void clientWarn(Args&&... args)     { s_clientLogger->warn(std::forward<Args>(args)...); }
	template<typename... Args>					    
	inline static void clientError(Args&&... args)    { s_clientLogger->error(std::forward<Args>(args)...); }
	template<typename... Args>
	inline static void clientCritical(Args&&... args) { s_clientLogger->critical(std::forward<Args>(args)...); }
	
private:
	static Reference<spdlog::logger> s_cometLogger, s_clientLogger;
};

}