/**
 * @file Engine/Core/Core.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

/**
 * @note
 * ```text
 * Kryos Engine naming convention
 *
 * ====================== Class/Struct members: ======================
 *** Variable:
 * - VarName  : Public member variables (PascalCase)
 * - mVarName : Private member variables ('m' prefix for Member)
 * - pVarName : Protected member variables ('p' prefix for Protected)
 * - sVarName : Static member variables ('s' prefix for Static)
 *
 *** Functions / Methods:
 * - FuncName()  : Public methods (PascalCase)
 * - HFuncName() : Private hidden methods ('H' prefix for Hidden)
 * - PFuncName() : Protected helper methods ('P' prefix for Protected)
 *
 * =================== Data Types/Classes/Structs: ===================
 * - IClassName  : Interfaces / Pure Abstract Classes ('I' prefix for Interface)
 * - TClassName  : Templates / Generic Classes ('T' prefix for Template Type)
 * - ClassName   : Concrete Classes / Value Types (PascalCase, no prefix)
 * - mStructName : Private struct or class ('m' prefix for Member)
 * - pStructName : Protected struct or class ('p' prefix for Protected)
 *
 * ============================== Note: ==============================
 * - Members in private/protected struct or class still use 'Class/Struct members' rules
 * ```
 */

#include <cstdint>
#include <functional>
#include <bit>    // For checking Endian (byte order)
#include <limits> // For checking Floating-Point Standard

#if defined(_WIN32)
#if defined(_WIN64)
#define PLATFORM_WINDOWS // Macro for Windows platform
#else
#error "Only support Windows 64-bit!"
#endif

#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
#if TARGET_OS_OSX == 1
#define PLATFORM_MACOS // Macro for macOS platform
#else
#error "Only support macOS!"
#endif

#elif defined(__linux__) // Macro for Linux
#define PLATFORM_LINUX

#else
#error "This platform is not supported!"
#endif

#if defined(PLATFORM_WINDOWS)
#if defined(KRYOS_ENGINE_EXPORT)
#define KRYOS_API __declspec(dllexport)
#else
#define KRYOS_API __declspec(dllimport)
#endif
#else
#define KRYOS_API
#endif

#define KS_STATIC_ASSERT(Expression, Message) static_assert(Expression, "[COMPILE ERROR] " Message)

#if defined(KS_DEBUG_MODE)
#if defined(PLATFORM_WINDOWS)
#define KS_DEBUG_BREAK() __debugbreak()
#else
#define KS_DEBUG_BREAK() __builtin_trap()
#endif

#define KS_RUNTIME_ASSERT(Expression, Message) \
    do                                         \
    {                                          \
        if ((!(Expression)))                   \
            KS_UNLIKELY                        \
            {                                  \
                KS_DEBUG_BREAK();              \
            }                                  \
    } while (0)
#else
#define KS_RUNTIME_ASSERT(Expression, Message) ((void)0)
#endif

/// @note All the compiler attrubutes wrap in macros
#if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard)
#define KS_NODISCARD [[nodiscard]]
#else
#define KS_NODISCARD
#endif

#if defined(__has_cpp_attribute) && __has_cpp_attribute(maybe_unused)
#define KS_UNUSED [[maybe_unused]]
#else
#define KS_UNUSED
#endif

#if defined(__has_cpp_attribute) && __has_cpp_attribute(no_unique_address)
#define KS_NO_UNIQUE_ADDRESS [[no_unique_address]]
#else
#define KS_NO_UNIQUE_ADDRESS
#endif

#if defined(__has_cpp_attribute) && __has_cpp_attribute(fallthrough)
#define KS_FALLTHROUGH [[fallthrough]]
#else
#define KS_FALLTHROUGH
#endif

#if defined(__has_cpp_attribute) && __has_cpp_attribute(deprecated)
#define KS_DEPRECATED(msg) [[deprecated(msg)]]
#else
#define KS_DEPRECATED(msg)
#endif

#if defined(__has_cpp_attribute) && __has_cpp_attribute(likely) && __has_cpp_attribute(unlikely)
#define KS_LIKELY [[likely]]
#define KS_UNLIKELY [[unlikely]]
#else
#define KS_LIKELY
#define KS_UNLIKELY
#endif

// An utility for bit calulation
#define BIT(x) (1 << x)

using UInt8 = std::uint8_t;
using UInt16 = std::uint16_t;
using UInt32 = std::uint32_t;
using UInt64 = std::uint64_t;

using Int8 = std::int8_t;
using Int16 = std::int16_t;
using Int32 = std::int32_t;
using Int64 = std::int64_t;

using Float32 = float;
using Float64 = double;

using SizeType = std::size_t;

// Checking type on the device
KS_STATIC_ASSERT(sizeof(void *) == 8, "Kryos Engine only supports 64-bit architectures!");
KS_STATIC_ASSERT(sizeof(UInt32) == 4, "Kryos Engine requires standard 32-bit int size!");
KS_STATIC_ASSERT(sizeof(Float32) == 4, "Kryos Engine requires standard 32-bit float size!");

// Checking system and device requires
KS_STATIC_ASSERT(std::endian::native == std::endian::little, "Kryos Engine only supports Little-Endian platforms!");
KS_STATIC_ASSERT(std::numeric_limits<Float32>::is_iec559, "Kryos Engine requires IEEE 754 / IEC 556 floating-point standard!");