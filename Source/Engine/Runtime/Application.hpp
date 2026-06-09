/**
 * @file Engine/Runtime/Application.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Window/Window.hpp"
#include "Input/Input.hpp"
#include "Core/Memory/SmartPointers.hpp"
#include "Core/Container/Strings/String.hpp"
#include "Core/Container/Strings/StringView.hpp"

namespace Kryos
{
    /**
     * @brief
     * A class to control and manage all the systems (Base class)
     * @note
     * ```text
     * - We can make more applications from this class
     * ```
     */
    class KRYOS_API Application
    {
    public:
        /**
         * @brief
         * Runs the application
         */
        void Run();

        /**
         * @brief This function return instance of the application
         * @return The application itself
         */
        Application *Get() { return mInstance; }

    protected:
        /**
         * @note We don't want other class call this except derived classes
         * @param appName A name for the application
         */
        Application(const StringView &appName);

        /**
         * @brief
         * This function will be call in every frame to update the 
         * @param deltaTime The delta time
         * @note
         * ```text
         * - Derived classes will overrided this function
         * ```
         */ 
        virtual void PCustomOnUpdate(Float32 deltaTime) = 0;

        /**
         * @brief
         * This function will be call in internal shutdown function
         * @note
         * ```text
         * - Derived classes will overrided this function
         * ```
         */ 
        virtual void PCustomShutdown() = 0;

        WindowManager *PGetWindowManager() { return mWindowManager.get(); }
        InputSystem *PGetInputSystem() { return mInputSystem.get(); }

    private:
        /**
         * @brief
         * This function will be call in every frame to update the application
         */
        void HOnUpdate();

        /**
         * @brief
         * This function shuts down the application at the end of the run's loop
         */
        void HShutdown();

    private:
        TUniquePtr<WindowManager> mWindowManager;
        TUniquePtr<InputSystem> mInputSystem;
        bool mIsRunning = false;

        /// @note For debugging
        String mApplicationName;
        /// @brief The application itself
        Application *mInstance;
    };

    /// @brief This function will implement in cilents (Derived classes of 'Application')
    Application* CreateApplication();
}