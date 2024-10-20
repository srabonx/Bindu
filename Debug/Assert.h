#ifndef BINDU_ASSERT_H
#define BINDU_ASSERT_H

#include "CommonUtility.h"
#include <filesystem>

#define BINDU_DEBUGBREAK() __debugbreak()


#ifdef BINDU_ENABLE_ASSERTS

		#define BINDU_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { BINDU##type##ERROR(msg,__VA_ARGS__); BINDU_DEBUGBREAK(); } }
		#define BINDU_INTERNAL_ASSERT_WITH_MSG(type, check, ...) BINDU_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed : {0}", __VA_ARGS__)
		#define BINDU_INTERNAL_ASSERT_NO_MSG(type, check) BINDU_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", BINDU_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

		#define BINDU_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
		#define BINDU_INTERNAL_ASSERT_GET_MACRO(...) BINDU_EXPAND_MACRO(BINDU_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, BINDU_INTERNAL_ASSERT_WITH_MSG, BINDU_INTERNAL_ASSERT_NO_MSG))

		#define BINDU_ASSERT(...) BINDU_EXPAND_MACRO(BINDU_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
		#define BINDU_CORE_ASSERT(...) BINDU_EXPAND_MACRO(BINDU_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))

#else

		#define BINDU_ASSERT(...)
		#define BINDU_CORE_ASSERT(...)

#endif

#endif