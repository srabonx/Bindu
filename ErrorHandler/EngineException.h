//
// Created by letsd on 23-Jul-24.
//

#ifndef BINDU_ENGINEEXCEPTION_H
#define BINDU_ENGINEEXCEPTION_H

#include <exception>


namespace BINDU {

class EngineException : public std::exception
{
public:

    EngineException(int errorNum, std::string errorDesc, std::string filename, int lineNum);

    ~EngineException() noexcept override;

    // Returns string with Error number, Error description, Source file name and Line number
    [[nodiscard]] const char* what() const noexcept override;


private:
    class Impl;
    Impl* m_impl{nullptr};
};

} // BINDU

#endif //BINDU_ENGINEEXCEPTION_H
