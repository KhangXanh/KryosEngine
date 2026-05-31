/**
 * @file Engine/Platform/Window/Win32/Win32Window.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Core/Window.hpp" /// @note Already contains <cstdint>

/// @details Forward declare
#if defined(PLATFORM_WINDOWS)
    #ifndef HWND
        struct HWND__;
        typedef HWND__ *HWND;
    #endif

    #ifndef HINSTANCE
        struct HINSTANCE__;
        typedef HINSTANCE__ *HINSTANCE;
    #endif

    typedef uintptr_t EngineWPARAM;
    typedef intptr_t  EngineLPARAM;
    typedef int64_t   EngineLRESULT;
    typedef unsigned int EngineUINT;

    #ifndef CALLBACK
        #define CALLBACK __stdcall
    #endif
#endif

namespace Kryos
{
/**
 * @brief
 * A derived class of 'IWindow' using Win32 API
 * @note
 * ```text
 * - This is only supported on the Windows platform
 * ```
 */
#if defined(PLATFORM_WINDOWS)
    class Win32Window : public IWindow
    {
    public:
        Win32Window(const WindowProperties &properties);

        void *GetNativeWindow() const override;

        UInt32 GetWidth() const override;
        UInt32 GetHeight() const override;

        bool ShouldClose() const override;

        void Close() override;
        void OnUpdate(Float32 dt) override;
        void SetWindowTitle(const TString &title) override;

    private:
        /**
         * @brief
         * This function converts from TString (a.k.a td::string) to std::wstring
         * @param[in] string The string to convert
         */
        std::wstring hStringToWString(const TString &string);

        /**
         * @brief
         * This function is the Win32 callback
         */
        static EngineLRESULT CALLBACK hWndProc(HWND hWnd, EngineUINT Message, EngineWPARAM wParam, EngineLPARAM lParam);

    private:
        HINSTANCE mHInstance = nullptr;
        HWND mHWnd = nullptr;

        UInt32 mWidth;
        UInt32 mHeight;

        bool mShouldClose;
    };

#else // If the platform is not Windows
    /**
     * @brief
     * This is the place holder for 'Win32Window'
     */
    class Win32Window : public IWindow
    {
    public:
        Win32Window(const WindowProperties &properties) mProperties(properties)
        {
            KS_CORE_FATAL("You are using wrong platform! Please check again!");
        };

        ~Win32Window() {};

        void *GetNativeWindow() const override { return nullptr; };

        UInt32 GetWidth() const override { return mProperties.Width; };
        UInt32 GetHeight() const override { return mProperties.Height; };

        bool ShouldClose() const override { return true; };

        void Close() override {};
        void OnUpdate(Float32 dt) override {};
        void SetWindowTitle(const TString &title) override {};

    private:
        WindowProperties mProperties;
    };
#endif
}