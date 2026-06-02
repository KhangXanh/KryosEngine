// Copyright 2026 Nguyễn Mạnh Khang (Blue)
//
// Licensed under the Apache License, Version 2.0.
// See LICENSE file in the project root for license information.

/**
 * @file Engine/Kryos.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

/**
 * @note 
 * ```text
 * - We make this file so the application can includes (calls) the engine from here
 * - Also you will need to define the logging flags (USE_CORE_LOG / USE_CILENT_LOG)
 *   for using the log library (Log.hpp)
 * ```
 */

#include "Logging/Log.hpp"
#include "Core/Entrypoint.hpp"

#include "Core/Input/Input.hpp"