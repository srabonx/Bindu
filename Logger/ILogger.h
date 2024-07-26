//
// Created by letsd on 24-Jul-24.
//

#ifndef BINDU_ILOGGER_H
#define BINDU_ILOGGER_H

#include <string>

namespace BINDU
{
    class ILogger
    {
    public:
        virtual ~ILogger() = default;

        // Open the log file at the provided directory, if not found then will create one
        virtual void Open(const std::string& filename) = 0;

        // Get the Log buffer
        virtual std::ostream& Buffer() = 0;

        // Commit log to the buffer and flush
        virtual void Log(const std::string& log) = 0;

        // Commits logs to the file
        virtual void Flush() = 0;

        // Closes the file
        virtual void Close() = 0;
    };
}

#endif //BINDU_ILOGGER_H
