/**
 * @file Engine/Logging/Log.hpp
 * This is a Kryos Engine Library header
 */


#pragma once

/**
 * @note
 * ```text
 * - We check which logging system we want to use
 * - We need to define 'USE_CORE_LOG' or "USE_CLIENT_LOG' or both to get the right system
 * ```
 * @example
 * ```
 * #define USE_CORE
 * #define USE_CLIENT
 * #include "Logging/Log.hpp"
 * ```
 */

#if defined(USE_CORE_LOG) && defined(USE_CLIENT_LOG)
    #include "Logging/Logger/CoreLog.hpp"
    // Include Client log

#elif defined(USE_CORE_LOG)
    #include "Logging/Logger/CoreLog.hpp"

#elif defined(USE_CORE_LOG)
    // Include Client log

#else
    #error "You must define either USE_CORE or USE_CLIENT before including Log.hpp!"
#endif