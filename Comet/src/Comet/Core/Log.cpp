#include "Log.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Comet
{
	//Define static members;
	std::shared_ptr<spdlog::logger> Log::s_cometLogger, Log::s_clientLogger;

	void Log::init()
	{
		//Set up sinks for logger (where to log/files)
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Comet.log", true));

		//Set logger patterns for each sink (file sink contains no colour information so explicitly writes log level)
		logSinks[0]->set_pattern("%^[%T] %n: %v%$");  //For console
		logSinks[1]->set_pattern("[%T] [%l] %n: %v"); //For file

		s_cometLogger = std::make_shared<spdlog::logger>("COMET", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_cometLogger);
		s_cometLogger->set_level(spdlog::level::trace);
		s_cometLogger->flush_on(spdlog::level::trace);

		s_clientLogger = std::make_shared<spdlog::logger>("CLIENT", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_clientLogger);
		s_clientLogger->set_level(spdlog::level::trace);
		s_clientLogger->flush_on(spdlog::level::trace);
	}

}