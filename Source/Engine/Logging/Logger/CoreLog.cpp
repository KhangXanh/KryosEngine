/**
 * @file Engine/Logging/Logger/CoreLog.cpp
 * This is a Kryos Engine Implementation file
 */

#include "CoreLog.hpp"
#include <chrono>
#include <ctime> // Fallback
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

#if defined(KS_DEBUG_MODE)
    #ifdef PLATFORM_WINDOWS
        if (AllocConsole())
        {
            FILE *fp;
            freopen_s(&fp, "CONOUT$", "w", stdout);
            freopen_s(&fp, "CONOUT$", "w", stderr);
        }

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


        auto now = std::chrono::system_clock::now();
        std::chrono::zoned_time zoneTime{std::chrono::current_zone(), now};
        auto now_seconds = std::chrono::floor<std::chrono::seconds>(zoneTime.get_local_time());

        /// @example Example: KS_LOG_2013-05-28_17-30-59_.txt
        std::string fileName = std::format("KS_LOG_{:%Y-%m-%d_%H-%M-%S}_.txt", now_seconds);

        GetLogFile().open(fileName, std::ios::out);

        if (GetLogFile().is_open())
        {
            GetLogFile()
            <<             "======================================================================\n"       
            << std::format("                 Session started at {:%Y-%m-%d %H:%M:%S}              \n", now_seconds)
            <<             "======================================================================\n";
        }
#endif
    }

    void CoreLog::LogRaw(std::string_view level, std::string_view color, std::string_view msg)
    {
#if defined(KS_DEBUG_MODE) // Checking if KS_DEBUG_MODE is on
        EnsureInit();

        auto now = std::chrono::system_clock::now();
        std::chrono::zoned_time zoneTime{std::chrono::current_zone(), now};
        auto now_seconds = std::chrono::floor<std::chrono::seconds>(zoneTime.get_local_time());

    #if defined(HAS_PRINT) // With std::print (C++23)
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

        // Log to file
        if (GetLogFile().is_open())
        {
            GetLogFile() << std::format("[{:%H:%M:%S}] [{}] {}\n", now_seconds, level, msg);
            GetLogFile().flush();
        }
#endif    
    }
}