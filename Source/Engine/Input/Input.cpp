/**
 * @file Engine/Input/Input.cpp
 * This is a Kryos Engine Implementation file
 */

#include "Input.hpp"
#include <cstring>

namespace Kryos
{
    // Query

    bool Input::IsKeyDown(Key key) const
    {
        return mCurrentKeys[static_cast<int>(key)];
    }

    bool Input::IsKeyPressed(Key key) const
    {
        int i = static_cast<int>(key);
        return mCurrentKeys[i] && !mPreviousKeys[i];
    }

    bool Input::IsKeyReleased(Key key) const
    {
        int i = static_cast<int>(key);
        return !mCurrentKeys[i] && mPreviousKeys[i];
    }

    bool Input::IsMouseButtonDown(MouseButton button) const
    {
        return mCurrentButtons[static_cast<int>(button)];
    }

    bool Input::IsMouseButtonPressed(MouseButton button) const
    {
        int i = static_cast<int>(button);
        return mCurrentButtons[i] && !mPreviousButtons[i];
    }

    bool Input::IsMouseButtonReleased(MouseButton button) const
    {
        int i = static_cast<int>(button);
        return !mCurrentButtons[i] && mPreviousButtons[i];
    }

    // Feed

    void Input::UpdateKey(Key key, bool pressed)
    {
        if (key == Key::Count) return;
        mCurrentKeys[static_cast<int>(key)] = pressed;
    }

    void Input::UpdateMouseButton(MouseButton button, bool pressed)
    {
        mCurrentButtons[static_cast<int>(button)] = pressed;
    }

    void Input::UpdateMousePosition(glm::vec2 position)
    {
        mCurrentMousePos = position;
    }

    void Input::UpdateScroll(float delta)
    {
        mScrollDelta += delta;
    }

    // Lifecycle

    void Input::Update()
    {
        memcpy(mPreviousKeys, mCurrentKeys, sizeof(mCurrentKeys));
        memcpy(mPreviousButtons, mCurrentButtons, sizeof(mCurrentButtons));
        mPreviousMousePos = mCurrentMousePos;
        mScrollDelta = 0.0f;
    }

    void Input::Reset()
    {
        memset(mCurrentKeys, 0, sizeof(mCurrentKeys));
        memset(mPreviousKeys, 0, sizeof(mPreviousKeys));
        memset(mCurrentButtons, 0, sizeof(mCurrentButtons));
        memset(mPreviousButtons, 0, sizeof(mPreviousButtons));
        mCurrentMousePos = {};
        mPreviousMousePos = {};
        mScrollDelta = 0.0f;
    }
}