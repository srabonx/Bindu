#include "Logger.h"
#include <sstream>
#include <fstream>
#include <iomanip>


BINDU::Logger* BINDU::Logger::m_logger = nullptr;


class BINDU::Logger::Impl
{
  public:
        // Buffer for the logs
        std::stringstream m_logBuffer;
        // Handle to the log file
        std::ofstream m_logFile;

    public:
        // Get the time as a string. Can be used for recording time of the log
        const std::string getTimeString();
        const std::string getLogTypeString(LogType type);
};



BINDU::Logger::Logger() : m_impl(std::make_unique<Impl>())
{
}

BINDU::Logger::~Logger()
{

}


BINDU::Logger* BINDU::Logger::Get()
{
    if (m_logger == nullptr)
    {
        m_logger = new Logger();
        return m_logger;
    }
    else
        return m_logger;
}

void BINDU::Logger::Open(const std::string& filename)
{
    m_impl->m_logFile.open(filename);

    Log(LogType::Info, "LOG START\n\n\n");

}

std::ostream& BINDU::Logger::Buffer()
{
    return m_impl->m_logBuffer;
}

void BINDU::Logger::Log(LogType type,const std::string& log)
{
    m_impl->m_logBuffer << m_impl->getTimeString() + " " + m_impl->getLogTypeString(type)  << "\n" << log;
    Flush();
}

void BINDU::Logger::Flush()
{
    m_impl->m_logFile << m_impl->m_logBuffer.str();
    m_impl->m_logFile.flush();
    m_impl->m_logBuffer.str("");
}

void BINDU::Logger::Close()
{
    m_impl->m_logFile.close();
    delete m_logger;
    m_logger = nullptr;
}


const std::string BINDU::Logger::Impl::getTimeString()
{
    std::stringstream timeStr;

    struct tm* pTime{ nullptr };
    time_t ctTime;
    time(&ctTime);
    pTime = localtime(&ctTime);

    timeStr << std::setw(2) << std::setfill('0') << pTime->tm_hour << ":";
    timeStr << std::setw(2) << std::setfill('0') << pTime->tm_min << ":";
    timeStr << std::setw(2) << std::setfill('0') << pTime->tm_sec;

    return timeStr.str();
}

const std::string BINDU::Logger::Impl::getLogTypeString(LogType type)
{
    switch (type)
    {
    case BINDU::LogType::Info: return "[INFO]";
    case BINDU::LogType::Warning: return "[WARNING]";
    case BINDU::LogType::Error: return "[ERROR]";
    default:
        break;
    }
    return "";
}


