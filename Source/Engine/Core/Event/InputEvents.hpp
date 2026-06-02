/**
 * @file Engine/Core/Event/InputEvents.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Core/Core.hpp"

namespace Kryos
{
    enum class Key : Int32
    {
        A = 0,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        Number0,
        Number1,
        Number2,
        Number3,
        Number4,
        Number5,
        Number6,
        Number7,
        Number8,
        Number9,

        GraveAccent,
        Minus,
        Equal,
        LeftBracket,
        RightBracket,
        Backslash,
        Semicolon,
        Apostrophe,
        Comma,
        Period,
        Slash,

        Escape,
        Space,
        Enter,
        Tab,
        Backspace,
        CapsLock,
        LeftControl,
        RightControl,
        LeftShift,
        RightShift,
        LeftAlt,
        RightAlt,
        LeftSuper,
        RightSuper,
        Menu,

        Insert,
        Delete,
        Home,
        End,
        PageUp,
        PageDown,
        LeftArrow,
        RightArrow,
        UpArrow,
        DownArrow,

        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,

        NPNumber0,
        NPNumber1,
        NPNumber2,
        NPNumber3,
        NPNumber4,
        NPNumber5,
        NPNumber6,
        NPNumber7,
        NPNumber8,
        NPNumber9,
        NPDecimal,
        NPDivide,
        NPMultiply,
        NPPlus,
        NPMinus,
        NPEnter,

        PrintScreen,
        ScrollLock,
        Pause,

        Count
    };

    enum class MouseButton : Int32
    {
        LeftButton = 0,
        RightButton,
        MiddleButton,
        Button4,
        Button5,
        Count
    };

    namespace InputEvents
    {
        struct KRYOS_API KeyPressedEvent
        {
            Key KeyCode;
            bool IsRepeat;
        };

        struct KRYOS_API KeyReleasedEvent
        {
            Key KeyCode;
        };

        struct KRYOS_API MouseButtonPressedEvent
        {
            MouseButton Button;
        };

        struct KRYOS_API MouseButtonReleasedEvent
        {
            MouseButton Button;
        };

        struct KRYOS_API MouseScrolledEvent
        {
            Float32 Offset;
        };

        struct KRYOS_API MouseMovedEvent
        {
            Float32 X;
            Float32 Y;
        };
    }
}