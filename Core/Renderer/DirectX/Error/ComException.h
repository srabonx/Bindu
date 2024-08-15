#ifndef COMEXCEPTION_H
#define COMEXCEPTION_H

#include <exception>
#include <winerror.h>
#include <string>


class ComException : public std::exception
{
public:
	ComException(std::string errorFile, int errorLine) : m_errorFile(std::move(errorFile)), m_errorLine(errorLine)
	{
		m_errorStr = "Error with HRESULT at " + m_errorFile + " in line: " + std::to_string(m_errorLine);
	}

	const char* what() const noexcept override
	{
		return m_errorStr.c_str();
	}

	
private:
	std::string m_errorStr;
	std::string m_errorFile;
	int m_errorLine;
};

#endif