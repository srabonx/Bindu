//
// Created by letsd on 24-Jul-24.
//

#ifndef BINDU_ERRORLOGMANAGER_H
#define BINDU_ERRORLOGMANAGER_H

#include <memory>
#include "ILogger.h"

namespace BINDU {

    // Singleton ErrorLogManager class
    class ErrorLogManager : public ILogger
    {
    public:

        // Returns the singleton instance of the class
        static ErrorLogManager* Get();

        // Creates and opens log file ready for writing.
        // Should be called at application startup
        void Open(const std::string &filename) override;

        void Log(const std::string& log) override;

        void Flush() override;

        void Close() override;

        // Logs exceptions to the log file
        void Log(std::exception& e);

        std::ostream& Buffer() override;

    private:
        ErrorLogManager();
        ~ErrorLogManager() override;

        // Singleton Instance of the error log manager
        static ErrorLogManager m_errorLogManager;

        class Impl;
        std::unique_ptr<Impl> m_impl{nullptr};

    };

} // BINDU

#endif //BINDU_ERRORLOGMANAGER_H
