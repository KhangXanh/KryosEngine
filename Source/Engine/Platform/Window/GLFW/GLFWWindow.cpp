/**
 * @file Engine/Platform/Window/GLFW/GLFWWindow.cpp
 * This is a Kryos Engine Implementation file
 */

#define USE_CORE_LOG
#include "Logging/Log.hpp"
#include "GLFWWindow.hpp"
#include "Core/Event/WindowEvents.hpp"
#include "Core/Event/EventBus.hpp"

namespace Kryos
{
    GLFWWindow::GLFWWindow(const WindowProperties &properties)
    {
        if (!sInitialized)
        {
            if (!glfwInit())
                KS_CORE_FATAL("Can't initialize GLFW!");
            sInitialized = true;
            KS_CORE_INFO("GLFW API has been initialized.");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        mWindow = glfwCreateWindow(
            properties.Width, properties.Height,
            properties.Name.c_str(),
            nullptr, nullptr);

        if (!mWindow)
            KS_CORE_FATAL("Can't create a window with GLFW API!");

        KS_CORE_INFO("A window with GLFW API has been created.");
    }

    void *GLFWWindow::GetNativeWindow() const { return mWindow; }

    UInt32 GLFWWindow::GetWidth() const
    {
        int width = 0;
        glfwGetFramebufferSize(mWindow, &width, nullptr);
        return static_cast<UInt32>(width);
    }

    UInt32 GLFWWindow::GetHeight() const
    {
        int height = 0;
        glfwGetFramebufferSize(mWindow, &height, nullptr);
        return static_cast<UInt32>(height);
    }

    void GLFWWindow::PClose()
    {
        if (mWindow)
        {
            glfwDestroyWindow(mWindow);
            mWindow = nullptr;
            KS_CORE_INFO("A window with GLFW API has been closed.");
        }
    }

    void GLFWWindow::OnUpdate()
    {
        glfwPollEvents();

        if (glfwWindowShouldClose(mWindow))
        {
            EventBus::Publish<WindowEvents::WindowCloseEvent>(pID);
        }
    }

    void GLFWWindow::SetWindowTitle(const String &title)
    {
        glfwSetWindowTitle(mWindow, title.c_str());
    }
}