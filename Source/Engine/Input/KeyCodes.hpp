/**
 * @file Engine/Input/KeyCodes.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Core/Core.hpp"

namespace Kryos
{
    enum class Key
    {
        // Letters
        A = 0, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

        // Numbers (top row)
        Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

        // Function keys
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
        F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,

        // Controls
        Space, Enter, Tab, Backspace, Escape,
        Insert, Delete, Home, End, PageUp, PageDown,
        CapsLock, ScrollLock, NumLock, PrintScreen, Pause,

        // Arrows
        Right, Left, Down, Up,

        // Modifiers
        LeftShift, RightShift, LeftControl, RightControl, LeftAlt, RightAlt, LeftSuper, RightSuper,

        // Punctuation
        Minus, Equal, LeftBracket, RightBracket, Backslash, Semicolon, Apostrophe, Comma, Period, Slash, GraveAccent,

        // Numpad / Keypad
        Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
        NumpadDecimal, NumpadDivide, NumpadMultiply, NumpadSubtract, NumpadAdd, NumpadEnter, NumpadEqual,

        Count // For counting the element
    };

    enum class MouseButton
    {
        Left = 0, Right, Middle, Button4, Button5,
        Count // For counting the element
    };
}