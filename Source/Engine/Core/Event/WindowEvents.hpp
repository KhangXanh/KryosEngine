/**
 * @file Engine/Core/Event/WindowEvents.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Core/Core.hpp"

namespace Kryos
{

    /**
     * @todo
     * ```text
     * - Implement all the window events
     * ```
     */
    namespace WindowEvents
    {
        // Forward declare - we don't need to include entire 'WindowData.hpp' file
        using WindowID = UInt32;

        struct KRYOS_API WindowCloseEvent
        {
            WindowID ID;
        };

        struct KRYOS_API WindowResizeEvent
        {
            WindowID ID;
            UInt32 Width;
            UInt32 Height;
        };
    }
}