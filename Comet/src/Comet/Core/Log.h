#pragma once

#include "spdlog/spdlog.h"

#include <memory>

namespace Comet
{

	class Log
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getCometLogger() { return s_cometLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_clientLogger; }
	
	private:
		static std::shared_ptr<spdlog::logger> s_cometLogger, s_clientLogger;
	};

}

//For logging within the Comet engine
#define CMT_COMET_TRACE(...)       ::Comet::Log::getCometLogger()->trace(__VA_ARGS__)
#define CMT_COMET_INFO(...)        ::Comet::Log::getCometLogger()->info(__VA_ARGS__)
#define CMT_COMET_WARN(...)        ::Comet::Log::getCometLogger()->warn(__VA_ARGS__)
#define CMT_COMET_ERROR(...)       ::Comet::Log::getCometLogger()->error(__VA_ARGS__)
#define CMT_COMET_CRITICAL(...)    ::Comet::Log::getCometLogger()->critical(__VA_ARGS__)

//For logging from client applications
#define CMT_CLIENT_TRACE(...)      ::Comet::Log::getClientLogger()->trace(__VA_ARGS__)
#define CMT_CLIENT_INFO(...)       ::Comet::Log::getClientLogger()->info(__VA_ARGS__)
#define CMT_CLIENT_WARN(...)       ::Comet::Log::getClientLogger()->warn(__VA_ARGS__)
#define CMT_CLIENT_ERROR(...)      ::Comet::Log::getClientLogger()->error(__VA_ARGS__)
#define CMT_CLIENT_CRITICAL(...)   ::Comet::Log::getClientLogger()->critical(__VA_ARGS__)
