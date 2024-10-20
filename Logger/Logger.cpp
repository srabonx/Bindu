#include "Logger.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace BINDU
{
	Ref<spdlog::logger>	Logger::s_coreLogger;
	Ref<spdlog::logger> Logger::s_clientLogger;


	void Logger::Init()
	{
		std::vector<spdlog::sink_ptr>	logSinks;
		logSinks.emplace_back(CreateRef<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(CreateRef<spdlog::sinks::basic_file_sink_mt>("Bindu.log", true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%1] %n: %v");

		s_coreLogger = CreateRef<spdlog::logger>("BINDU", std::begin(logSinks), std::end(logSinks));
		spdlog::register_logger(s_coreLogger);
		s_coreLogger->set_level(spdlog::level::trace);
		s_coreLogger->flush_on(spdlog::level::trace);

		s_clientLogger = CreateRef<spdlog::logger>("APP", std::begin(logSinks), std::end(logSinks));
		spdlog::register_logger(s_clientLogger);
		s_clientLogger->set_level(spdlog::level::trace);
		s_clientLogger->flush_on(spdlog::level::trace);
	}
}





