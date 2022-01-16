#pragma once

#if defined(_DEBUG) || !defined(NDEBUG)
#define CEE_DEBUG 1
#endif

#include <Engine/PlatformDetection.h>

#if defined(CEE_DEBUG)
#if defined(CEE_PLATFORM_WINDOWS)
#define CEE_DEBUGBREAK() __debugbreak()
#elif defined(CEE_PLATFORM_LINUX)
#include <signal.h>
#define CEE_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet."
#endif
#else
#define CEE_DEBUGBREAK() ((void)0)
#endif

#define CEE_EXPAND_MACRO(x) x
#define CEE_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define CEE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#include <Engine/Log.h>
#include <Engine/Assert.h>
