#include "CometPCH.h"
#include "Log.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Comet
{
Reference<spdlog::logger> Log::s_cometLogger, Log::s_clientLogger;

void Log::init()
{
	//Set up sinks for logger (where to log/files)
	std::vector<spdlog::sink_ptr> logSinks;
	logSinks.emplace_back(createReference<spdlog::sinks::stdout_color_sink_mt>());
	logSinks.emplace_back(createReference<spdlog::sinks::basic_file_sink_mt>("Comet.log", true));

	//Set logger format for each sink (file sink format contains no colour information)
	logSinks[0]->set_pattern("%^[%T] [%l] %n: %v%$");  //For console
	logSinks[1]->set_pattern("[%T] [%l] %n: %v");      //For file

	s_cometLogger = createReference<spdlog::logger>("COMET", begin(logSinks), end(logSinks));
	spdlog::register_logger(s_cometLogger);
	s_cometLogger->set_level(spdlog::level::trace);
	s_cometLogger->flush_on(spdlog::level::trace);

	s_clientLogger = createReference<spdlog::logger>("CLIENT", begin(logSinks), end(logSinks));
	spdlog::register_logger(s_clientLogger);
	s_clientLogger->set_level(spdlog::level::trace);
	s_clientLogger->flush_on(spdlog::level::trace);
}

void Log::setLogLevel(LogLevel level)
{
	switch (level)
	{
	case LogLevel::CMT_TRACE: s_cometLogger->set_level(spdlog::level::trace); s_clientLogger->set_level(spdlog::level::trace); break;
	case LogLevel::CMT_INFO: s_cometLogger->set_level(spdlog::level::info); s_clientLogger->set_level(spdlog::level::info); break;
	case LogLevel::CMT_WARN: s_cometLogger->set_level(spdlog::level::warn); s_clientLogger->set_level(spdlog::level::warn); break;
	case LogLevel::CMT_ERROR: s_cometLogger->set_level(spdlog::level::err); s_clientLogger->set_level(spdlog::level::err); break;
	case LogLevel::CMT_CRITICAL: s_cometLogger->set_level(spdlog::level::critical); s_clientLogger->set_level(spdlog::level::critical); break;
	default: CMT_COMET_ASSERT_MESSAGE(false, "Unknown logging level"); break;
	}
}

}
