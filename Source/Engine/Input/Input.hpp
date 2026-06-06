/**
 * @file Engine/Core/Input/Input.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Core/Event/InputEvents.hpp"
#include "Core/Containers/Hashs/HashTable.hpp"
#include "Math/Vector2D.hpp"

namespace Kryos
{
    class KRYOS_API InputSystem
    {
    public:
        bool IsKeyPressed(Key keyCode) { return mKeyState[keyCode]; }
        bool IsMousePressed(MouseButton mouseButton) { return mMouseState[mouseButton]; }
        Math::Vector2D GetMousePosition() { return { mMouseX, mMouseY }; }
        Float32 GetMouseOffset() { return mMouseOffset; }

        void Update();

    private:
        THashTable<Key, bool> mKeyState;
        THashTable<MouseButton, bool> mMouseState;

        Float32 mMouseX = 0.0f, mMouseY = 0.0f;
        Float32 mMouseOffset = 0.0f;
    };
}
