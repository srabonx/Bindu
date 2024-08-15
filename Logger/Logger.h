#ifndef LOGGER_H
#define LOGGER_H

#include "ILogger.h"
#include <memory>

namespace BINDU
{
	

	class Logger : public ILogger
	{
	public:

		static Logger* Get();

		// Inherited via ILogger
		void Open(const std::string& filename) override;
		std::ostream& Buffer() override;
		void Log(LogType type, const std::string& log) override;
		void Flush() override;
		void Close() override;


	private:
		Logger();
		~Logger() override;

		static Logger* m_logger;

		class Impl;
		std::unique_ptr<Impl> m_impl{ nullptr };
	};
}


#endif