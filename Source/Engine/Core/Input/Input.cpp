/**
 * @file Engine/Core/Input/Input.cpp
 * This is a Kryos Engine Implementation file
 */

#include "Input.hpp"
#include "Core/Event/EventBus.hpp"

namespace Kryos
{
    void InputSystem::Update()
    {
        /**
         * @note
         * ```text
         * - Tip: We use const auto& for zero-copy and safety 
         * ```
         */

        // Reset value
        mMouseOffset = 0.0f;
        
        const auto &pressedKeys = EventBus::GetEvents<InputEvents::KeyPressedEvent>();
        for (const auto &event : pressedKeys)
        {
            mKeyState[event.KeyCode] = true;
        }

        const auto &releasedKeys = EventBus::GetEvents<InputEvents::KeyReleasedEvent>();
        for (const auto &event : releasedKeys)
        {
            mKeyState[event.KeyCode] = false;
        }

        const auto &mouseMoves = EventBus::GetEvents<InputEvents::MouseMovedEvent>();
        if (!mouseMoves.empty())
        {
            // We use back() to get the newest position
            mMouseX = mouseMoves.back().X;
            mMouseY = mouseMoves.back().Y;
        }

        const auto &mouseScrolls = EventBus::GetEvents<InputEvents::MouseScrolledEvent>();
        for (const auto &event : mouseScrolls)
        {
            mMouseOffset += event.Offset;
        }

        const auto &pressedButtons = EventBus::GetEvents<InputEvents::MouseButtonPressedEvent>();
        for (const auto &event : pressedButtons)
        {
            mMouseState[event.Button] = true;
        }

        const auto &releasedButtons = EventBus::GetEvents<InputEvents::MouseButtonReleasedEvent>();
        for (const auto &event : releasedButtons)
        {
            mMouseState[event.Button] = false;
        }
    }
}