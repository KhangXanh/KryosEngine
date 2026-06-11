/**
 * @file Engine/Window/Window.cpp
 * This is a Kryos Engine Implementation file
 */

#define USE_CORE_LOG
#include "Logging/Log.hpp"

#include "Window.hpp"
#include "Platform/Window/GLFW/GLFWWindow.hpp"
#include "Platform/Window/Win32/Win32Window.hpp"

namespace Kryos
{
    TUniquePtr<IWindow> IWindow::HCreateWindow(const WindowProperties &properties)
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
        mWindows.Clear();
        // Then we assign the 'mNextWindowID' to 0
        mNextWindowID = 0;
        KS_CORE_INFO("WindowManager has been initialized.");
    }

    WindowID WindowManager::AddWindow(const WindowProperties &properties)
    {
        // We create a new window with the 'properties'
        auto newWindow = IWindow::HCreateWindow(properties);
        WindowID newID = mNextWindowID++;
        // Set new window' ID to 'newID'
        newWindow->pID = newID;
        mWindows.Emplace(newID, std::move(newWindow));
        KS_CORE_INFO("A window with ID {} has been created.", newID);
        return newID;
    }

    IWindow *WindowManager::GetWindow(WindowID id)
    {
        // If we can find a window with the same ID
        // we will return the pointer to that window without changing ownership
        auto it = mWindows.Find(id);
        if (it == mWindows.End())
        {
            // If don't then we return a nullptr
            KS_CORE_ERROR("There is no window with ID {}!", id);
            return nullptr;
        }
        
        return it->second.get();
    }

    void WindowManager::Update()
    {
        for (auto &[id, window] : mWindows)
        {
            window->OnUpdate();
        }
    }

    void WindowManager::CloseWindow(WindowID id)
    {
        auto it = mWindows.Find(id);

        if (it == mWindows.end())
        {
            KS_CORE_ERROR("Can't find window with ID {}!", id);
            return;
        }

        it->second->PClose();
        mWindows.Erase(it);

        KS_CORE_INFO("A Window with ID {} closed.", id);
    }

    void WindowManager::HCloseAllWindow()
    {
        for (auto &[id, window] : mWindows)
            window->PClose();
        mWindows.Clear();
        KS_CORE_INFO("All windows closed.");
    }

    bool WindowManager::ShouldCloseAllWindow() const
    {
        // We check does the 'mWindows' is empty
        return mWindows.IsEmpty();
    }
}