/**
 * @file Engine/Platform/Window/Win32/Win32Window.cpp
 * This is a Kryos Engine Implementation file
 */

#define USE_CORE_LOG
#include "Logging/Log.hpp"
#include "Win32Window.hpp"
#include "Core/Event/WindowEvents.hpp"
#include "Core/Event/EventBus.hpp"

#if defined(PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace Kryos
{
    Win32Window::Win32Window(const WindowProperties &properties)
        : mHWnd(nullptr), mWidth(properties.Width), mHeight(properties.Height)
    {
        mHInstance = GetModuleHandleW(NULL);

        WNDCLASSEXW existingClass = {0};
        if (!GetClassInfoExW(mHInstance, L"KryosWindowClass", &existingClass))
        {
            WNDCLASSEXW wc = {0};
            wc.cbSize = sizeof(WNDCLASSEXW);
            wc.hInstance = mHInstance;
            wc.lpszClassName = L"KryosWindowClass";
            wc.lpfnWndProc = HWndProc;

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

        std::wstring wideName = HStringToWString(properties.Name);

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

    void Win32Window::OnUpdate()
    {
        MSG msg;
        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    void Win32Window::PClose()
    {
        if (mHWnd)
        {
            DestroyWindow(mHWnd);
            mHWnd = nullptr;
        }
        KS_CORE_INFO("A window with Win32 API has been closed.");
    }

    void Win32Window::SetWindowTitle(const String &title)
    {
        if (mHWnd)
        {
            SetWindowTextW(mHWnd, HStringToWString(title).c_str());
        }
    }

    std::wstring Win32Window::HStringToWString(const String &string)
    {
        if (string.IsEmpty())
        {
            return std::wstring();
        }

        int size_needed = MultiByteToWideChar(CP_UTF8, 0, &string[0], (int)string.GetSize(), NULL, 0);

        std::wstring wstringTo(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, &string[0], (int)string.GetSize(), &wstringTo[0], size_needed);

        return wstringTo;
    }

    EngineLRESULT CALLBACK Win32Window::HWndProc(HWND hWnd, EngineUINT Message, EngineWPARAM wParam, EngineLPARAM lParam)
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

            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            {
                Key kryosKey = HTranslateWin32Key(wParam, lParam);
                if (kryosKey != Key::Count)
                {
                    bool isRepeat = (lParam & (1 << 30)) != 0;
                    EventBus::Publish<InputEvents::KeyPressedEvent>(kryosKey, isRepeat);
                }
                return 0;
            }
            case WM_KEYUP:
            case WM_SYSKEYUP:
            {
                Key kryosKey = HTranslateWin32Key(wParam, lParam);
                if (kryosKey != Key::Count)
                {
                    EventBus::Publish<InputEvents::KeyReleasedEvent>(kryosKey);
                }
                return 0;
            }

            case WM_MOUSEMOVE:
            {
                Float32 x = static_cast<Float32>(LOWORD(wParam));
                Float32 y = static_cast<Float32>(LOWORD(wParam));
                EventBus::Publish<InputEvents::MouseMovedEvent>(x, y);
                return 0;
            }

            case WM_LBUTTONDOWN:
            {
                EventBus::Publish<InputEvents::MouseButtonPressedEvent>(MouseButton::LeftButton);
                return 0;
            }
            case WM_LBUTTONUP:
            {
                EventBus::Publish<InputEvents::MouseButtonReleasedEvent>(MouseButton::LeftButton);
                return 0;
            }

            case WM_RBUTTONDOWN:
            {
                EventBus::Publish<InputEvents::MouseButtonPressedEvent>(MouseButton::RightButton);
                return 0;
            }
            case WM_RBUTTONUP:
            {
                EventBus::Publish<InputEvents::MouseButtonReleasedEvent>(MouseButton::RightButton);
                return 0;
            }

            case WM_MBUTTONDOWN:
            {
                EventBus::Publish<InputEvents::MouseButtonPressedEvent>(MouseButton::MiddleButton);
            }
            case WM_MBUTTONUP:
            {
                EventBus::Publish<InputEvents::MouseButtonReleasedEvent>(MouseButton::MiddleButton);
                return 0;
            }
            case WM_MOUSEWHEEL:
            {
                // GET_WHEEL_DELTA_WPARAM returns a negative or positive value
                // Positive means scrolling up (Zoom In), Negative means scrolling down (Zoom Out)
                Float32 wheelDelta = static_cast<Float32>(GET_WHEEL_DELTA_WPARAM(wParam)) / static_cast<Float32>(WHEEL_DELTA);
                EventBus::Publish<InputEvents::MouseScrolledEvent>(wheelDelta);
                return 0;
            }

            case WM_CLOSE:
            {
                EventBus::Publish<WindowEvents::WindowCloseEvent>(pWindow->pID); // Push the window close event to the event bus
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

    Key Win32Window::HTranslateWin32Key(EngineWPARAM wParam, EngineLPARAM lParam)
    {
        // Letters A-Z
        if (wParam >= 'A' && wParam <= 'Z')
            return static_cast<Key>(static_cast<Int32>(Key::A) + (wParam - 'A'));

        // Numbers on the top row 0-9
        if (wParam >= '0' && wParam <= '9')
            return static_cast<Key>(static_cast<Int32>(Key::Number0) + (wParam - '0'));

        // Numpad digits
        if (wParam >= VK_NUMPAD0 && wParam <= VK_NUMPAD9)
            return static_cast<Key>(static_cast<Int32>(Key::NPNumber0) + (wParam - VK_NUMPAD0));

        switch (wParam)
        {
        // Punctuation / OEM keys
        case VK_OEM_3:
            return Key::GraveAccent;
        case VK_OEM_MINUS:
            return Key::Minus;
        case VK_OEM_PLUS:
            return Key::Equal;
        case VK_OEM_4:
            return Key::LeftBracket;
        case VK_OEM_6:
            return Key::RightBracket;
        case VK_OEM_5:
            return Key::Backslash;
        case VK_OEM_1:
            return Key::Semicolon;
        case VK_OEM_7:
            return Key::Apostrophe;
        case VK_OEM_COMMA:
            return Key::Comma;
        case VK_OEM_PERIOD:
            return Key::Period;
        case VK_OEM_2:
            return Key::Slash;

        // Whitespace / control
        case VK_ESCAPE:
            return Key::Escape;
        case VK_SPACE:
            return Key::Space;
        case VK_TAB:
            return Key::Tab;
        case VK_BACK:
            return Key::Backspace;
        case VK_CAPITAL:
            return Key::CapsLock;

        case VK_RETURN:
        {
            // Check the 24th bit of lParam to distinguish between Numpad Enter and the main Enter key
            bool isExtended = (lParam & (1 << 24)) != 0;
            return isExtended ? Key::NPEnter : Key::Enter;
        }

        case VK_CONTROL:
            return (GetKeyState(VK_RCONTROL) & 0x8000) ? Key::RightControl : Key::LeftControl;
        case VK_SHIFT:
            return (GetKeyState(VK_RSHIFT) & 0x8000) ? Key::RightShift : Key::LeftShift;
        case VK_MENU: // Phím ALT
            return (GetKeyState(VK_RMENU) & 0x8000) ? Key::RightAlt : Key::LeftAlt;
        case VK_LWIN:
            return Key::LeftSuper;
        case VK_RWIN:
            return Key::RightSuper;
        case VK_APPS:
            return Key::Menu;

        // Navigation
        case VK_INSERT:
            return Key::Insert;
        case VK_DELETE:
            return Key::Delete;
        case VK_HOME:
            return Key::Home;
        case VK_END:
            return Key::End;
        case VK_PRIOR:
            return Key::PageUp;
        case VK_NEXT:
            return Key::PageDown;

        case VK_LEFT:
            return Key::LeftArrow;
        case VK_RIGHT:
            return Key::RightArrow;
        case VK_UP:
            return Key::UpArrow;
        case VK_DOWN:
            return Key::DownArrow;

        // Function keys
        case VK_F1:
            return Key::F1;
        case VK_F2:
            return Key::F2;
        case VK_F3:
            return Key::F3;
        case VK_F4:
            return Key::F4;
        case VK_F5:
            return Key::F5;
        case VK_F6:
            return Key::F6;
        case VK_F7:
            return Key::F7;
        case VK_F8:
            return Key::F8;
        case VK_F9:
            return Key::F9;
        case VK_F10:
            return Key::F10;
        case VK_F11:
            return Key::F11;
        case VK_F12:
            return Key::F12;
        case VK_F13:
            return Key::F13;
        case VK_F14:
            return Key::F14;
        case VK_F15:
            return Key::F15;
        case VK_F16:
            return Key::F16;
        case VK_F17:
            return Key::F17;
        case VK_F18:
            return Key::F18;
        case VK_F19:
            return Key::F19;
        case VK_F20:
            return Key::F20;
        case VK_F21:
            return Key::F21;
        case VK_F22:
            return Key::F22;
        case VK_F23:
            return Key::F23;
        case VK_F24:
            return Key::F24;

        // Numpad / keypad
        case VK_DECIMAL:
            return Key::NPDecimal;
        case VK_DIVIDE:
            return Key::NPDivide;
        case VK_MULTIPLY:
            return Key::NPMultiply;
        case VK_ADD:
            return Key::NPPlus;
        case VK_SUBTRACT:
            return Key::NPMinus;

        // Misc
        case VK_SNAPSHOT:
            return Key::PrintScreen;
        case VK_SCROLL:
            return Key::ScrollLock;
        case VK_PAUSE:
            return Key::Pause;
        }

        return Key::Count;
    }
}