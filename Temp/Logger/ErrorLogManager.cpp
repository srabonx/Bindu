//
// Created by letsd on 24-Jul-24.
//

#include <sstream>
#include <fstream>
#include <iomanip>
#include "ErrorLogManager.h"
#include "../ErrorHandler/EngineException.h"

namespace BINDU {

    ErrorLogManager ErrorLogManager::m_errorLogManager;

    class ErrorLogManager::Impl
    {
    public:
        // Buffer for the logs
        std::stringstream m_logBuffer;
        // Handle to the log file
        std::ofstream m_logFile;

    public:
        // Get the time as a string. Can be used for recording time of the log
        std::string getTimeString();
    };




    ErrorLogManager::ErrorLogManager() : m_impl(std::make_unique<Impl>())
    {

    }

    ErrorLogManager::~ErrorLogManager()
    {
        this->Close();
    }

    ErrorLogManager *ErrorLogManager::Get()
    {
        return &m_errorLogManager;
    }

    void ErrorLogManager::Open(const std::string &filename)
    {
        m_impl->m_logFile.open(filename);
    }

    void ErrorLogManager::Flush()
    {
        m_impl->m_logFile << m_impl->m_logBuffer.str();
        m_impl->m_logFile.flush();
        m_impl->m_logBuffer.str("");
    }

    void ErrorLogManager::Close()
    {
        m_impl->m_logFile.close();
    }

    void ErrorLogManager::Log(LogType type, const std::string &log)
    {
        m_impl->m_logBuffer << m_impl->getTimeString() << "\n" << log;
        Flush();
    }

    void ErrorLogManager::Log(std::exception &e)
    {
        m_impl->m_logBuffer << m_impl->getTimeString() << "\n" << e.what();
        Flush();
    }

    std::ostream&ErrorLogManager::Buffer() {
        return m_impl->m_logBuffer;
    }

    std::string ErrorLogManager::Impl::getTimeString()
    {
        std::stringstream timeStr;

        struct tm* pTime{nullptr};
        time_t ctTime;
        time(&ctTime);
        pTime = localtime(&ctTime);

        timeStr << std::setw(2) << std::setfill('0') << pTime->tm_hour << ":";
        timeStr << std::setw(2) << std::setfill('0') << pTime->tm_min << ":";
        timeStr << std::setw(2) <<std::setfill('0') << pTime->tm_sec;

        return timeStr.str();
    }


} // BINDU