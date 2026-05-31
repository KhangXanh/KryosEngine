/**
 * @file Engine/Input/Input.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Math/DataTypes.hpp"
#include "KeyCodes.hpp"

namespace Kryos
{
    class KRYOS_API Input
    {
    public:
        Input() { Reset(); }

        bool IsKeyDown(Key key) const;
        bool IsKeyPressed(Key key) const;
        bool IsKeyReleased(Key key) const;

        bool IsMouseButtonDown(MouseButton button) const;
        bool IsMouseButtonPressed(MouseButton button) const;
        bool IsMouseButtonReleased(MouseButton button) const;

        Math::Vec2 GetMousePosition() const { return mCurrentMousePos; }
        Math::Vec2 GetMouseDelta() const { return mCurrentMousePos - mPreviousMousePos; }
        float GetScrollDelta() const { return mScrollDelta; }

        // Feed API
        void UpdateKey(Key key, bool pressed);
        void UpdateMouseButton(MouseButton button, bool pressed);
        void UpdateMousePosition(Math::Vec2 position);
        void UpdateScroll(float delta);

        void Update();
        void Reset();

    private:
        static constexpr int KEY_COUNT = static_cast<int>(Key::Count);
        static constexpr int BUTTON_COUNT = static_cast<int>(MouseButton::Count);

        bool mCurrentKeys[KEY_COUNT] = {};
        bool mPreviousKeys[KEY_COUNT] = {};

        bool mCurrentButtons[BUTTON_COUNT] = {};
        bool mPreviousButtons[BUTTON_COUNT] = {};

        Math::Vec2 mCurrentMousePos = {};
        Math::Vec2 mPreviousMousePos = {};
        float mScrollDelta = 0.0f;
    };
}