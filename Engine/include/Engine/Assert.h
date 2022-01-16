#pragma once

#include <Engine/Base.h>
#include <Engine/Log.h>

#include <filesystem>

#if defined(CEE_ENABLE_ASSERTS)

#define CEE_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if (!(check)) { CEE##type##ERROR(msg, __VA_ARGS__); CEE_DEBUGBREAK(); } }
#define CEE_INTERNAL_ASSERT_WITH_MSG(type, check, ...) CEE_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define CEE_INTERNAL_ASSERT_NO_MSG(type, check, ...) CEE_INTERNAL_ASSERT_IMPL(type, check, "Assertion {0} failed at {1}:{2}", CEE_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define CEE_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2 ,macro, ...) macro
#define CEE_INTERNAL_ASSERT_GET_MACRO(...) CEE_EXPAND_MACRO(CEE_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, CEE_INTERNAL_ASSERT_WITH_MSG, CEE_INTERNAL_ASSERT_NO_MSG))

#define CEE_ASSERT(...) CEE_EXPAND_MACRO(CEE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
#define CEE_CORE_ASSERT(...) CEE_EXPAND_MACRO(CEE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))

#else
#define CEE_ASSERT(...)
#define CEE_CORE_ASSERT(...)
#endif
