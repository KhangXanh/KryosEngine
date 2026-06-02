/**
 * @file Engine/Platform/Window/GLFW/GLFWWindow.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Core/Window.hpp"
#include <GLFW/glfw3.h>

namespace Kryos
{
    /**
     * @brief
     * A derived class of 'IWindow' using GLFW API
     * @note
     * ```text
     * - This is only supported on the Linux platform
     * - You can use it on other platform but we don't recommend it
     * ```
     * @todo
     * ```text
     * - Handle the input event for Linux
     * ```
     */
    class GLFWWindow : public IWindow
    {
    public:
        GLFWWindow(const WindowProperties &properties);

        void *GetNativeWindow() const override;

        UInt32 GetWidth() const override;
        UInt32 GetHeight() const override;

        void PClose() override;
        void OnUpdate() override;
        void SetWindowTitle(const TString &title) override;

    private:
        GLFWwindow *mWindow = nullptr;
        /// @brief A static flag to check if GLFW has been initialized
        static inline bool sInitialized = false;
    };
}