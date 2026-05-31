/**
 * @file Engine/Logging/Logger/CoreLog.cpp
 * This is a Kryos Engine Implementation file
 */

#include "CoreLog.hpp"
#include <chrono>
#include <fstream>

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <stdio.h> // For freopen_s
#endif

namespace Kryos
{
    static bool sLogInitialized = false;
    static bool sColorEnabled = true;

    static std::ofstream &GetLogFile()
    {
        static std::ofstream logFile;
        return logFile;
    }

    static void EnsureInit()
    {
        if (sLogInitialized)
            return;
        sLogInitialized = true;

#ifdef PLATFORM_WINDOWS
        if (AllocConsole())
        {
            FILE *fp;
            freopen_s(&fp, "CONOUT$", "w", stdout);
            freopen_s(&fp, "CONOUT$", "w", stderr);
        }

        // 2. Kích hoạt mã màu ANSI cho Console
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        if (!GetConsoleMode(hOut, &mode))
        {
            sColorEnabled = false;
        }
        else if (!SetConsoleMode(hOut, mode | 0x0004)) // 0x0004 = ENABLE_VIRTUAL_TERMINAL_PROCESSING
        {
            sColorEnabled = false;
        }
#endif

        /// @details Create file
        auto now = std::chrono::system_clock::now();
        auto now_seconds = std::chrono::floor<std::chrono::seconds>(now);

        /// @example Example: KS_LOG_2013-05-28_17-30-59_.txt
        std::string fileName = std::format("KS_LOG_{:%Y-%m-%d_%H-%M-%S}_.txt", now_seconds);

        GetLogFile().open(fileName, std::ios::out);

        if (GetLogFile().is_open())
        {
            GetLogFile()
            << std::format("======================================================================\n")       
            << std::format("                 Session started at {:%Y-%m-%d %H:%M:%S}              \n", now_seconds)
            << std::format("======================================================================\n");
        }
    }

    void CoreLog::LogRaw(std::string_view level, std::string_view color, std::string_view msg)
    {
        EnsureInit();

        auto now = std::chrono::system_clock::now();
        auto now_seconds = std::chrono::floor<std::chrono::seconds>(now);
#if defined(KS_DEBUG_MODE) // Checking if KS_DEBUG_MDOE is on
    #if defined(HAS_PRINT) // With std::pring (C++23)
        if (sColorEnabled) // With color
        {
            std::println("{}[{:%H:%M:%S}] [{}] {}{}", color, now_seconds, level, msg, "\033[0m");
        }
        else // Without color
        {
            std::println("[{:%H:%M:%S}] [{}] {}", now_seconds, level, msg);
        }

    #else // With std::cout and std::format
        if (sColorEnabled) // With color
        {
            std::cout << std::format("{}[{:%H:%M:%S}] [{}] {}{}\n", color, now_seconds, level, msg, "\033[0m");
        }
        else // Without color
        {
            std::cout << std::format("[{:%H:%M:%S}] [{}] {}\n", now_seconds, level, msg);
        }
    #endif
#endif
        // Log to file
        if (GetLogFile().is_open())
        {
            GetLogFile() << std::format("[{:%H:%M:%S}] [{}] {}\n", now_seconds, level, msg);

            GetLogFile().flush();
        }
    }
}