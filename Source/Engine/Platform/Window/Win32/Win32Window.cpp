/**
 * @file Engine/Platform/Window/Win32/Win32Window.cpp
 * This is a Kryos Engine Implementation file
 */

#define USE_CORE_LOG
#include "Logging/Log.hpp"
#include "Win32Window.hpp"

#if defined(PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

namespace Kryos
{
    Win32Window::Win32Window(const WindowProperties &properties)
        : mHWnd(nullptr), mWidth(properties.Width), mHeight(properties.Height), mShouldClose(false)
    {
        mHInstance = GetModuleHandleW(NULL);

        WNDCLASSEXW existingClass = {0};
        if (!GetClassInfoExW(mHInstance, L"KryosWindowClass", &existingClass))
        {
            WNDCLASSEXW wc = {0};
            wc.cbSize = sizeof(WNDCLASSEXW);
            wc.hInstance = mHInstance;
            wc.lpszClassName = L"KryosWindowClass";
            wc.lpfnWndProc = hWndProc;

            wc.hIcon = LoadIconW(NULL, reinterpret_cast<LPCWSTR>(IDI_APPLICATION));
            wc.hIconSm = LoadIconW(NULL, reinterpret_cast<LPCWSTR>(IDI_APPLICATION));
            wc.hCursor = LoadCursorW(NULL, reinterpret_cast<LPCWSTR>(IDC_ARROW));
            wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

            if (!RegisterClassExW(&wc))
            {
                MessageBoxW(NULL, L"Window registration failed.", L"Error", MB_ICONERROR | MB_OK);
                return;
            }
        }

        std::wstring wideName = hStringToWString(properties.Name);

        mHWnd = CreateWindowExW(
            WS_EX_OVERLAPPEDWINDOW,
            L"KryosWindowClass",
            wideName.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            mWidth, mHeight,
            NULL, NULL, mHInstance,
            this);

        if (mHWnd == NULL)
        {
            MessageBoxW(NULL, L"Window creation failed.", L"Error", MB_ICONERROR | MB_OK);
            KS_CORE_FATAL("Can't create a new window with Win32 API!");
            return;
        }

        ShowWindow(mHWnd, SW_SHOWNORMAL);
        UpdateWindow(mHWnd);
        KS_CORE_INFO("A window with Win32 API has been created.");
    }

    void *Win32Window::GetNativeWindow() const
    {
        return reinterpret_cast<void *>(mHWnd);
    }

    UInt32 Win32Window::GetWidth() const
    {
        return mWidth;
    }

    UInt32 Win32Window::GetHeight() const
    {
        return mHeight;
    }

    void Win32Window::OnUpdate(Float32 dt)
    {
        MSG msg;
        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    bool Win32Window::ShouldClose() const
    {
        return mShouldClose;
    }

    void Win32Window::Close()
    {
        if (mHWnd)
        {
            DestroyWindow(mHWnd);
            mHWnd = nullptr;
        }
        KS_CORE_INFO("A window with Win32 API has been closed.");
    }

    void Win32Window::SetWindowTitle(const TString &title)
    {
        if (mHWnd)
        {
            SetWindowTextW(mHWnd, hStringToWString(title).c_str());
        }
    }

    std::wstring Win32Window::hStringToWString(const TString &string)
    {
        if (string.empty())
        {
            return std::wstring();
        }

        int size_needed = MultiByteToWideChar(CP_UTF8, 0, &string[0], (int)string.size(), NULL, 0);

        std::wstring wstringTo(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, &string[0], (int)string.size(), &wstringTo[0], size_needed);

        return wstringTo;
    }

    EngineLRESULT CALLBACK Win32Window::hWndProc(HWND hWnd, EngineUINT Message, EngineWPARAM wParam, EngineLPARAM lParam)
    {
        Win32Window *pWindow = nullptr;

        if (Message == WM_NCCREATE)
        {
            LPCREATESTRUCTW pCreate = reinterpret_cast<LPCREATESTRUCTW>(lParam);
            pWindow = reinterpret_cast<Win32Window *>(pCreate->lpCreateParams);
            SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
        }
        else
        {
            pWindow = reinterpret_cast<Win32Window *>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
        }

        if (pWindow)
        {
            switch (Message)
            {
            case WM_SIZE:
            {
                pWindow->mWidth = LOWORD(lParam);
                pWindow->mHeight = HIWORD(lParam);
                return 0;
            }
            case WM_CLOSE:
            {
                pWindow->mShouldClose = true;
                return 0;
            }
            case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }
            }
        }
        return DefWindowProcW(hWnd, Message, wParam, lParam);
    }
}