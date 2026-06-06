/**
 * @file Engine/Core/Memory/SmartPointers.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

/**
 * @todo
 * ```text
 * - We are using std::smart_ptr but in furture we will create our smart pointers
 * ```
 */

#include <memory>

/**
 * @brief
 * A wrapper for unique pointer
 */
template <typename T, typename Deleter = std::default_delete<T>>
using TUniquePtr = std::unique_ptr<T, Deleter>;

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