/**
 * @file Engine/Core/Window.cpp
 * This is a Kryos Engine Implementation file
 */

#define USE_CORE_LOG
#include "Logging/Log.hpp"

#include "Window.hpp"
#include "Platform/Window/GLFW/GLFWWindow.hpp"
#include "Platform/Window/Win32/Win32Window.hpp"

namespace Kryos
{
    TUniquePtr<IWindow> IWindow::CreateWindow(const WindowProperties &properties)
    {
// We check the target platform to decide which Window API we will using
#if defined(PLATFORM_WINDOWS)
        return CreateUniquePtr<Win32Window>(properties); // Using Win32 API
#elif defined(PLATFORM_LINUX)
        return CreateUniquePtr<GLFWWindow>(properties); // Using GLFW API
#elif defined(PLATFORM_MACOS)
        return nullptr;
#endif
    }

    void WindowManager::Init()
    {
        // First we clear all the trash in 'mWindows' for safety
        mWindows.clear();
        // Then we prepare 1 slot for next window
        mWindows.reserve(1);
        // Then we assign the 'mNextWindowID' to 0
        mNextWindowID = 0;
        KS_CORE_INFO("WindowManager has been initialized.");
    }

    WindowID WindowManager::AddWindow(const WindowProperties &properties)
    {
        // We create a new window with the 'properties'
        auto newWindow = IWindow::CreateWindow(properties);
        WindowID newID = mNextWindowID++;
        mWindows.emplace_back(newID, std::move(newWindow));
        return newID;
    }

    IWindow *WindowManager::GetWindow(WindowID id)
    {
        // If we can find a window with the same ID
        // we will return the pointer to that window without changing ownership
        for (auto &container : mWindows)
        {
            if (container.ID == id)
            {
                return container.Instance.get();
            }
        }
        // If don't then we return a nullptr
        KS_CORE_ERROR("There is no window with ID {}!", id);
        return nullptr;
    }

    void WindowManager::UpdateAllWindows(Float32 dt)
    {
        for (auto &container : mWindows)
        {
            container.Instance->OnUpdate(dt);
            if (container.Instance->ShouldClose())
                mWindowsToClose.push_back(container.ID);
        }

        for (WindowID id : mWindowsToClose)
            CloseWindow(id);

        // Reset the 'mWindowToClose' to prevent overriding
        mWindowsToClose.clear();
    }

    void WindowManager::CloseWindow(WindowID id)
    {
        auto it = std::find_if(
            mWindows.begin(), mWindows.end(),
            [id](const WindowContainer &c)
            {
                return c.ID == id;
            });

        if (it == mWindows.end())
        {
            KS_CORE_ERROR("Can't find window with ID {}!", id);
            return;
        }

        (*it).Instance->Close();
        mWindows.erase(it);

        KS_CORE_INFO("Window with ID {} closed.", id);
    }

    void WindowManager::hCloseAllWindow()
    {
        for (auto &container : mWindows)
            container.Instance->Close();
        mWindows.clear();
        KS_CORE_INFO("All windows closed.");
    }

    bool WindowManager::ShouldCloseAllWindow() const
    {
        // We check does the 'mWindows' is empty
        return mWindows.empty();
    }
}