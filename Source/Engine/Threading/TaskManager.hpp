/**
 * @file Engine/Threading/TaskManager.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Core/Core.hpp"

#include <thread>

/**
 * @todo
 * ```text
 * - Make a working TaskManager
 * - Optimize it with all the system
 * ```
 */

namespace Kryos
{
    class KRYOS_API TaskManager
    {
    public:

        /**
         * @note Automatically calculates the number of threads available in the CPU
         * @param[in] threadCount Number of worker threads (defaults to core count)
         */
        TaskManager(UInt32 threadCount = std::thread::hardware_concurrency());
        ~TaskManager();

        /// @note No move or copy
        TaskManager(const TaskManager &) = delete;
        TaskManager& operator=(const TaskManager &) = delete;


    };
}