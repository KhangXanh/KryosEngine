/**
 * @file Engine/Logging/Logger/CoreLog.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include <string_view>
#include <chrono>
#include <format>

#include "Core/Core.hpp"

// Check if the compiler supports <print> and has C++23
#if __has_include(<print>) && __cpp_lib_print && __cplusplus >= 202302L
#include <print>
#define HAS_PRINT

#else // Fallback
#include <iostream>
#endif

namespace Kryos
{
    class CoreLog
    {
    public:
        KRYOS_API static void LogRaw(std::string_view level, std::string_view color, std::string_view msg);

        template <typename... Args>
        static void CoreInfo(std::format_string<Args...> fmt, Args &&...args)
        {
            LogRaw("INFO", "\033[32m", std::format(fmt, std::forward<Args>(args)...));
        }

        template <typename... Args>
        static void CoreWarn(std::format_string<Args...> fmt, Args &&...args)
        {
            LogRaw("WARN", "\033[33m", std::format(fmt, std::forward<Args>(args)...));
        }

        template <typename... Args>
        static void CoreError(std::format_string<Args...> fmt, Args &&...args)
        {
            LogRaw("ERROR", "\033[31m", std::format(fmt, std::forward<Args>(args)...));
        }

        template <typename... Args>
        static void CoreFatal(std::format_string<Args...> fmt, Args &&...args)
        {
            LogRaw("FATAL", "\033[41m", std::format(fmt, std::forward<Args>(args)...));
            std::abort(); // Exit the program
        }

        template <typename... Args>
        static void CoreTrace(std::format_string<Args...> fmt, Args &&...args)
        {
            LogRaw("TRACE", "\033[37m", std::format(fmt, std::forward<Args>(args)...));
        }
    };
}

// Terminal Log
#define KS_CORE_TRACE(fmt, ...) Kryos::CoreLog::CoreTrace(fmt __VA_OPT__(, ) __VA_ARGS__)

// Terminal Info
#define KS_CORE_INFO(fmt, ...) Kryos::CoreLog::CoreInfo(fmt __VA_OPT__(, ) __VA_ARGS__)

// Terminal Warning
#define KS_CORE_WARN(fmt, ...) Kryos::CoreLog::CoreWarn(fmt __VA_OPT__(, ) __VA_ARGS__)

// Terminal Error
#define KS_CORE_ERROR(fmt, ...) Kryos::CoreLog::CoreError(fmt __VA_OPT__(, ) __VA_ARGS__)

// Terminal Fatal
#define KS_CORE_FATAL(fmt, ...) Kryos::CoreLog::CoreFatal(fmt __VA_OPT__(, ) __VA_ARGS__)

/**
 * @note
 * ```text
 * - We can use like this:
 * KS_CORE_TRACE("This is a trace log!");
 * KS_CORE_INFO("This is a info with number {}!", 10);
 * ```
 */