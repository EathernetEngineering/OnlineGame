#pragma once

#if defined(_WIN32)
#if defined(_WIN64)
#define CEE_PLATFORM_WINDOWS 1
#error "Windows not supported"
#else
#error "x86 Builds are not supported."
#endif
#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1
#error "IOS simulator is not supported."
#elif TARGET_OS_IPHONE == 1
#define CEE_PLATFORM_IOS 1
#error "IOS not supported."
#elif TARGET_OS_MAC == 1
#define CEE_PLATFORM_MACOS 1
#error "MacOS not supported."
#else
#error "Unknown Apple platform."
#endif
#elif defined(__ANDROID__)
#define CEE_PLATFORM_ANDROID
#error "Android not supported"
#elif defined (__linux__)
#define CEE_PLATFORM_LINUX
#endif
