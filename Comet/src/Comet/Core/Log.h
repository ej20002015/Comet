#pragma once

#include "CometPCH.h"

#include "spdlog/spdlog.h"

#include <memory>

namespace Comet
{

	class Log
	{
	public:
		static void init();

#ifdef CMT_LOG_MACROS
		inline static std::shared_ptr<spdlog::logger>& getCometLogger() { return s_cometLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_clientLogger; }
#endif

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
		inline static void cometCritical(Args&&... args)  { s_cometLogger->Critical(std::forward<Args>(args)...); }
		
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
		inline static void clientCritical(Args&&... args) { s_clientLogger->Critical(std::forward<Args>(args)...); }
	
	private:
		static std::shared_ptr<spdlog::logger> s_cometLogger, s_clientLogger;
	};

}

#ifdef CMT_LOG_MACROS

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

#else

//For logging within the Comet engine
#define CMT_COMET_TRACE(...)       static_assert(false, "Recommended to use methods in the Comet::Log namespace for logging - Alternatively, enable macro logging by defining 'CMT_LOG_MACROS'");
#define CMT_COMET_INFO(...)        static_assert(false, "Recommended to use methods in the Comet::Log namespace for logging - Alternatively, enable macro logging by defining 'CMT_LOG_MACROS'");
#define CMT_COMET_WARN(...)        static_assert(false, "Recommended to use methods in the Comet::Log namespace for logging - Alternatively, enable macro logging by defining 'CMT_LOG_MACROS'");
#define CMT_COMET_ERROR(...)       static_assert(false, "Recommended to use methods in the Comet::Log namespace for logging - Alternatively, enable macro logging by defining 'CMT_LOG_MACROS'");
#define CMT_COMET_CRITICAL(...)    static_assert(false, "Recommended to use methods in the Comet::Log namespace for logging - Alternatively, enable macro logging by defining 'CMT_LOG_MACROS'");

//For logging from client applications
#define CMT_CLIENT_TRACE(...)      static_assert(false, "Recommended to use methods in the Comet::Log namespace for logging - Alternatively, enable macro logging by defining 'CMT_LOG_MACROS'");
#define CMT_CLIENT_INFO(...)       static_assert(false, "Recommended to use methods in the Comet::Log namespace for logging - Alternatively, enable macro logging by defining 'CMT_LOG_MACROS'");
#define CMT_CLIENT_WARN(...)       static_assert(false, "Recommended to use methods in the Comet::Log namespace for logging - Alternatively, enable macro logging by defining 'CMT_LOG_MACROS'");
#define CMT_CLIENT_ERROR(...)      static_assert(false, "Recommended to use methods in the Comet::Log namespace for logging - Alternatively, enable macro logging by defining 'CMT_LOG_MACROS'");
#define CMT_CLIENT_CRITICAL(...)   static_assert(false, "Recommended to use methods in the Comet::Log namespace for logging - Alternatively, enable macro logging by defining 'CMT_LOG_MACROS'");

#endif