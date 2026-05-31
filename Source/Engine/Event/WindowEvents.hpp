/**
 * @file Engine/Event/WindowEvents.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Event.hpp"
#include "Core/WindowData.hpp"

namespace Kryos
{
    class WindowEvent : public IEvent
    {
    public:
        WindowID GetWindowID() const { return pWindowID; }
        Int32 GetCategory() const override { return EventCategory::EventWindow; }

    protected:
        WindowEvent(WindowID id) : pWindowID(id) {}
        WindowID pWindowID;
    };

    class WindowCloseEvent : public WindowEvent
    {
    public:
        WindowCloseEvent(WindowID id) : WindowEvent(id) {}

        static EventType StaticType() { return EventType::WindowClose; }
        EventType GetType() const override { return StaticType(); }
        const char *GetName() const override { return "WindowClose"; }
    };

    class WindowResizeEvent : public WindowEvent
    {
    public:
        WindowResizeEvent(WindowID id, int width, int height)
            : WindowEvent(id), mWidth(width), mHeight(height) {}

        int GetWidth() const { return mWidth; }
        int GetHeight() const { return mHeight; }

        static EventType StaticType() { return EventType::WindowResize; }
        EventType GetType() const override { return StaticType(); }
        const char *GetName() const override { return "WindowResize"; }

    private:
        int mWidth, mHeight;
    };

    class WindowFocusEvent : public WindowEvent
    {
    public:
        WindowFocusEvent(WindowID id) : WindowEvent(id) {}

        static EventType StaticType() { return EventType::WindowFocus; }
        EventType GetType() const override { return StaticType(); }
        const char *GetName() const override { return "WindowFocus"; }
    };

    class WindowLostFocusEvent : public WindowEvent
    {
    public:
        WindowLostFocusEvent(WindowID id) : WindowEvent(id) {}

        static EventType StaticType() { return EventType::WindowLostFocus; }
        EventType GetType() const override { return StaticType(); }
        const char *GetName() const override { return "WindowLostFocus"; }
    };

    class WindowMovedEvent : public WindowEvent
    {
    public:
        WindowMovedEvent(WindowID id, int x, int y)
            : WindowEvent(id), mX(x), mY(y) {}

        int GetX() const { return mX; }
        int GetY() const { return mY; }

        static EventType StaticType() { return EventType::WindowMoved; }
        EventType GetType() const override { return StaticType(); }
        const char *GetName() const override { return "WindowMoved"; }

    private:
        int mX, mY;
    };
}