//
// Created by letsd on 24-Jul-24.
//

#include <string>
#include <sstream>
#include "EngineException.h"

class BINDU::EngineException::Impl
{
public:
    int m_errorNum{0};
    std::string m_errorDesc;
    std::string m_filename;
    int m_lineNum{0};
    std::string m_errorText;
};

BINDU::EngineException::EngineException(int errorNum, std::string errorDesc, std::string filename, int lineNum) : m_impl(new Impl())
{
    m_impl->m_errorNum = errorNum;
    m_impl->m_errorDesc = std::move(errorDesc);
    m_impl->m_filename = std::move(filename);
    m_impl->m_lineNum = lineNum;

    // Write properties to a human-readable string
    std::stringstream errStr;

    errStr<<"Error Num: " << m_impl->m_errorNum <<"\nError Desc: " << m_impl->m_errorDesc <<
    "\nSrc File: " << m_impl->m_filename << "\nLine Num: " << m_impl->m_lineNum << "\n";

    m_impl->m_errorText = errStr.str();
}

BINDU::EngineException::~EngineException() noexcept
{
    delete m_impl;
}

const char* BINDU::EngineException::what() const noexcept
{
    return m_impl->m_errorText.c_str();
}


