//
// Created by letsd on 24-Jul-24.
//

#ifndef BINDU_COMMONUTILITY_H
#define BINDU_COMMONUTILITY_H

#define NOMINMAX

#include <codecvt>
#include <Windows.h>
#include <cassert>
#include "../../ErrorHandler/EngineException.h"

// Defining a macro to throw a EngineException class
// Uses __FILE__ and __LINE__ constants
#ifndef THROW_EXCEPTION
#define THROW_EXCEPTION(ErrorNum, ErrorDesc) throw EngineException(ErrorNum, ErrorDesc, __FILE__, __LINE__);
#endif

#define BINDU_EXPAND_MACRO(x) x
#define BINDU_STRINGIFY_MACRO(x) #x

#define SET_BIT(x) (1 << x)

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

inline std::string RelativeResourcePath(const char* filepath = nullptr, const std::string& rootFolderName = "")
{
	std::string ResourcePath(MAX_PATH, '\0');

	GetModuleFileNameA(nullptr, const_cast<LPSTR>(ResourcePath.data()), MAX_PATH);

	std::string delimeter = rootFolderName;

	size_t t = ResourcePath.find(delimeter);
	ResourcePath = ResourcePath.substr(0, t + delimeter.size() + 1);

	if (filepath)
		ResourcePath += filepath;

	return ResourcePath;
}

namespace BINDU
{

	template<typename T>
	using Scoped = std::unique_ptr<T>;

	template<typename T, typename... Args>
	constexpr Scoped<T> CreateScoped(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#endif //BINDU_COMMONUTILITY_H
