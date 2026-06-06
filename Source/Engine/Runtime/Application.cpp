/**
 * @file Engine/Runtime/Application.cpp
 * This is a Kryos Engine Implementation file
 */

#include "Application.hpp"

#define USE_CORE_LOG
#include "Logging/Log.hpp"

#include "Core/Event/WindowEvents.hpp"
#include "Core/Event/EventBus.hpp"
#include "Time/Time.hpp"

namespace Kryos
{
    Application::Application(const StringView &appName)
        : mApplicationName(appName), mWindowManager(CreateUniquePtr<WindowManager>()), mInputSystem(CreateUniquePtr<InputSystem>())
    {
        mInstance = this;

        EventBus::Init();
        mWindowManager->Init();
        Time::Init();
        KS_CORE_INFO("An application using Kryos Engine named '{}' has been created.", mApplicationName);
    }

    void Application::Run()
    {
        mIsRunning = true;
        while (mIsRunning)
        {
            Time::Tick();
            Float32 deltaTime = Time::GetDeltaTime();

            mWindowManager->Update();
            HOnUpdate();

            if (mWindowManager->ShouldCloseAllWindow())
            {
                mIsRunning = false;
                break;
            }

            mInputSystem->Update();
            PCustomOnUpdate(deltaTime);
            EventBus::Clear();
        }
        HShutdown();
    }

    void Application::HOnUpdate()
    {
        const auto &closeEvents = EventBus::GetEvents<WindowEvents::WindowCloseEvent>();
        for (const auto &event : closeEvents)
        {
            mWindowManager->CloseWindow(event.ID);
        }
    }

    void Application::HShutdown()
    {
        PCustomShutdown();
        mInputSystem.reset();
        mWindowManager.reset();

        EventBus::Shutdown();
        mInstance = nullptr;
        KS_CORE_INFO("An application using Kryos Engine named '{}' has been closed.", mApplicationName);
    }
}