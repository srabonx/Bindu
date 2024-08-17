//
// Created by letsd on 24-Jul-24.
//

#ifndef BINDU_COMMONUTILITY_H
#define BINDU_COMMONUTILITY_H

#include <codecvt>

#include "../../ErrorHandler/EngineException.h"

// Defining a macro to throw a EngineException class
// Uses __FILE__ and __LINE__ constants
#ifndef THROW_EXCEPTION
#define THROW_EXCEPTION(ErrorNum, ErrorDesc) throw EngineException(ErrorNum, ErrorDesc, __FILE__, __LINE__);
#endif

// TODO: Find better methods
inline std::wstring StringToWstring(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> convertX;
	return convertX.from_bytes(str);

	/*std::wstring ws(str.size(), L'');
	ws.resize(std::mbstowcs(&ws[0], str.c_str(), str.size()));
	return ws;*/
}

inline std::string WstringToString(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.to_bytes(wstr);
}


#endif //BINDU_COMMONUTILITY_H
