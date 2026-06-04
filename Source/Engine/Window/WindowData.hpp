/**
 * @file Engine/Window/WindowData.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Core/Core.hpp"

/**
 * @brief
 * An alias name for UInt32 (a.k.a uin32_t)
 */
using WindowID = UInt32;
constexpr WindowID INVALID_WINDOW_ID = std::numeric_limits<UInt32>::max();