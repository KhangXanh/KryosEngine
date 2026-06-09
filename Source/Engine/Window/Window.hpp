/**
 * @file Engine/Window/Window.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "WindowData.hpp"

namespace Kryos
{
    /**
     * @note
     * ```text
     * - We need this for creating the window
     * - The variable 'Name' is the title, also it's the name of the window
     * ```
     */
    struct KRYOS_API WindowProperties
    {
        String Name;
        UInt32 Width;
        UInt32 Height;

        WindowProperties(String name, UInt32 width, UInt32 height)
            : Name(name), Width(width), Height(height) {}
    };

    /**
     * @brief
     * The interface window (Base window class)
     * @note
     * ```text
     * - Each Windows API will have different classes that inherit from IWindow
     * - Example: GLFWWindow, SDLWindow, etc.
     * ```
     */
    class KRYOS_API IWindow
    {
        friend class WindowManager;
    public:
        virtual ~IWindow() = default;

        /**
         * @brief
         * A function to return the native handle of the window
         * @note
         * ```text
         * - We use the return type 'void*' so that the function can return any pointer type
         * - Example: GLFWWindow*, SDLWindow*, etc.
         * - We use 'const' to prevent modifying value
         * ```
         */
        virtual void *GetNativeWindow() const = 0;

        /**
         * @brief
         * This function will return the width of the window
         * @return
         * ```text
         * UInt32 (a.k.a uint32_t)
         * ```
         * @note
         * ```text
         * - We use 'const' to prevent modifying value
         * ```
         */
        virtual UInt32 GetWidth() const = 0;

        /**
         * @brief
         * This function will return the height of the window
         * @return
         * ```text
         * UInt32 (a.k.a uint32_t)
         * ```
         * @note
         * ```text
         * - We use 'const' to prevent modifying value
         * ```
         */
        virtual UInt32 GetHeight() const = 0;

        /**
         * @brief
         * This function updates the window and polls all events every frame in the application's run loop
         */
        virtual void OnUpdate() = 0;

        /**
         * @brief
         * This function change the window title bar name
         * @param[in] title The name of the new title
         */
        virtual void SetWindowTitle(const String &title) = 0;

    private:
        /**
         * @brief
         * A function to create a new window using the WindowProperties
         * @param[in] properties The window properties for creating window
         * @return
         * ```text
         * TUniquePtr<IWindow> (a.k.a std::unique_ptr<IWindow> / The window ifself)
         * ```
         * @note
         * ```text
         * - We need constant reference for safety and better performace
         * - The reference (&) don't copy all the data (deep copy), so the performace is better
         * - We use 'const' to prevent modifying value, which will cause error
         * ```
         */
        static TUniquePtr<IWindow> HCreateWindow(const WindowProperties &properties);

    protected:
         /**
         * @brief
         * This function closes the window
         */
        virtual void PClose() = 0;
    
    protected:
        WindowID pID;
    };

    /**
     * @brief
     * A class for managing windows (IWindow)
     */
    class KRYOS_API WindowManager
    {
    public:
        /**
         * @brief
         * This function initializes the 'WindowManager' system
         * @note
         * ```text
         * - You must call this function exactly once
         * ```
         */
        void Init();

        /**
         * @brief
         * This function adds (pushes) a new window to the 'mWindows'
         * @param[in] properties We need this for the 'CreateWindow()' function in IWindow
         * @return
         * ```text
         * WindowID (a.k.a UInt32)
         * Because we need an ID for getting and closing window
         * ```
         */
        // void SetEventCallback(const EventCallbackFn &callback)
        // {
        //     mEventCallback = callback;
        // }

        /**
         * @brief
         * This function adds (pushes) a new window to the 'mWindows'
         * @param[in] properties We need this for the 'CreateWindow()' function in IWindow
         * @return
         * ```text
         * WindowID (a.k.a UInt32)
         * Because we need an ID for getting and closing window
         * ```
         */
        WindowID AddWindow(const WindowProperties &properties);

        /**
         * @brief
         * This function gets a reference to a window with the ID you give
         * @param[in] id The ID you give to get the window
         * @return
         * ```text
         * IWindow*
         * Because we only need to access the window, not change ownership
         * ```
         */
        IWindow *GetWindow(WindowID id);

        /**
         * @brief
         * This function updates all the windows in 'mWindows'
         */
        void Update();

        /**
         * @brief
         * This function closes a window with the ID you give
         * @param[in] id The ID you give to close the window
         * @note
         * ```text
         * - This function closes the window forcibly and unexpectedly (Can be use in debug)
         * ```
         */
        void CloseWindow(WindowID id);

        /**
         * @brief
         * This function checks if 'WindowManager' should close all windows
         * @return
         * ```text
         * boolean (true / false)
         * ```
         * @note
         * ```text
         * - We use 'const' to prevent modifying value
         * ```
         */
        bool ShouldCloseAllWindow() const;

    private:
        /**
         * @brief
         * This function closes all the windows in 'mWindows'
         * @note
         * ```text
         * - This function closes all the windows forcibly and unexpectedly (Can be use in debug)
         * ```
         */
        void HCloseAllWindow();
        friend class Application;

    private:
        /**
         * @brief
         * A hash table (a.k.a std::vector) to manage and contain 'IWindow' and 'WindowID'
         * @note
         * ```text
         * - We use hash table for O(1) instead of dynamic array which only have O(n)
         * ```
         */
        THashTable<WindowID, TUniquePtr<IWindow>> mWindows;

        /**
         * @brief
         * The ID of the next window will be pushed to mWindows
         * @note
         * ```text
         * - We assign the 'mNextWindowID' to the new window ID
         * - Then we increase the ID (mNextWindowID += 1;)
         * ```
         */
        WindowID mNextWindowID;
    };

    class Application;
}