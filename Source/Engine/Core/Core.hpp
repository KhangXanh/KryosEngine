/**
 * @file Engine/Core/Core.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

/**
 * @note
 * ```text
 * Kryos Engine naming convention
 * *** Variable: ***
 * - VarName  : Public member variables (PascalCase)
 * - mVarName : Private member variables ('m' prefix for Member)
 * - pVarName : Protected member variables ('p' prefix for Protected)
 * - sVarName : Static member variables ('s' prefix for Static)
 *
 * *** Functions / Methods: ***
 * - FuncName()  : Public methods (PascalCase)
 * - HFuncName() : Private helper methods ('H' prefix for Helper)
 * - PFuncName() : Protected helper methods ('P' prefix for Protected)
 * ```
 */

#include <memory>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <map>
#include <functional>

#include <string>
#include <string_view>

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
#define BE_LIKELY [[likely]]
#define BE_UNLIKELY [[unlikely]]
#else
#define BE_LIKELY
#define BE_UNLIKELY
#endif

// An utility for bit calulation
#define BIT(x) (1 << x)

/**
 * @brief
 * A wrapper for unique pointer
 */
template <typename T>
using TUniquePtr = std::unique_ptr<T>;

/**
 * @brief
 * A wrapper for shared pointer
 */
template <typename T>
using TSharedPtr = std::shared_ptr<T>;

/**
 * @brief
 * A wrapper for weak pointer
 */
template <typename T>
using TWeakPtr = std::weak_ptr<T>;

/**
 * @brief
 * A function to create a unique pointer
 * @return
 * ```text
 * TUniquePtr<T> (a.k.a std::unique_ptr<T>)
 * ```
 * @note
 * ```text
 * - You can use like this:
 * TUniquePtr<Type> value = CreateUniquePtr<Type>(ParamsIfNeeded);
 * ```
 */
template <typename T, typename... Args>
inline constexpr TUniquePtr<T> CreateUniquePtr(Args &&...args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

/**
 * @brief
 * A function to create a shared pointer
 * @return
 * ```text
 * TSharedPtr<T> (a.k.a std::shared_ptr<T>)
 * ```
 * @note
 * ```text
 * - You can use like this:
 * TSharedPtr<Type> value = CreateSharedPtr<Type>(ParamsIfNeeded);
 * ```
 */
template <typename T, typename... Args>
inline constexpr TSharedPtr<T> CreateSharedPtr(Args &&...args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

/**
 * @brief
 * A custom string for Kryos Engine
 * @todo
 * ```text
 * - For now we are using std::string
 * - But we will make a custom string
 * ```
 */
using TString = std::string;

/**
 * @brief
 * A custom dynamic array for Kryos Engine
 * @todo
 * ```text
 * - For now we are using std::vector
 * - But we will make a custom dynamic array
 * ```
 */
template <typename T>
using TDynamicArray = std::vector<T>;

/**
 * @brief
 * A custom hash table for Kryos Engine
 * @todo
 * ```text
 * - For now we are using std::unordered_map for core
 * - But we will make a cutom hash table with our algorithms
 * ```
 */
template <typename T, typename U>
class THashTable : public std::unordered_map<T, U>
{
public:
    bool Contains(const T &key) const
    {
        return this->find(key) != this->end();
    }
};

using UInt8 = std::uint8_t;   // @brief uint8_t
using UInt16 = std::uint16_t; // @brief uint16_t
using UInt32 = std::uint32_t; // @brief uint32_t
using UInt64 = std::uint64_t; // @brief uint64_t

using Int8 = std::int8_t;   // @brief int8_t
using Int16 = std::int16_t; // @brief int16_t
using Int32 = std::int32_t; // @brief int32_t
using Int64 = std::int64_t; // @brief int64_t

using Float32 = float;  // @brief float
using Float64 = double; // @brief double

// Checking the type on the device
static_assert(sizeof(void *) == 8, "Kryos Engine only supports 64-bit architectures!");
static_assert(sizeof(UInt32) == 4, "Kryos Engine requires standard 32-bit int size!");
static_assert(sizeof(Float32) == 4, "Kryos Engine requires standard 32-bit float size!");

// Checking system and device requires
static_assert(std::endian::native == std::endian::little, "Kryos Engine only supports Little-Endian platforms!");
static_assert(std::numeric_limits<Float32>::is_iec559, "Kryos Engine requires IEEE 754 / IEC 556 floating-point standard!");