#ifndef LOGGER_H
#define LOGGER_H

#include "ILogger.h"
#include <memory>

#include "../Utility/Common/CommonUtility.h"
#include <spdlog/spdlog.h>

namespace BINDU
{

	class Logger
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }

		static Ref<spdlog::logger>& GetClientLogger() { return s_clientLogger; }

	private:
		static Ref<spdlog::logger>	s_coreLogger;

		static Ref<spdlog::logger>	s_clientLogger;
	};
}


// Core log macros
#define BINDU_CORE_TRACE(...)		::BINDU::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define BINDU_CORE_INFO(...)		::BINDU::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define BINDU_CORE_WARN(...)		::BINDU::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define BINDU_CORE_ERROR(...)		::BINDU::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define BINDU_CORE_CRITICAL(...)	::BINDU::Logger::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define BINDU_CLIENT_TRACE(...)		::BINDU::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define BINDU_CLIENT_INFO(...)		::BINDU::Logger::GetClientLogger()->info(__VA_ARGS__)
#define BINDU_CLIENT_WARN(...)		::BINDU::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define BINDU_CLIENT_ERROR(...)		::BINDU::Logger::GetClientLogger()->error(__VA_ARGS__)
#define BINDU_CLIENT_CRITICAL(...)	::BINDU::Logger::GetClientLogger()->critical(__VA_ARGS__)


#endif