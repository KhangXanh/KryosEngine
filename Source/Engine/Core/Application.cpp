/**
 * @file Engine/Core/Application.cpp
 * This is a Kryos Engine Implementation file
 */

#define USE_CORE_LOG
#include "Logging/Log.hpp"
#include "Application.hpp"
#include "Time/Time.hpp"
#include <iostream>

namespace Kryos
{
    Application::Application()
        : mWindowManager(CreateUniquePtr<WindowManager>()), mLayerStack(CreateUniquePtr<LayerStack>())
    {
        mRunning = true;
        mWindowManager->Init();
        Time::Init();
    }

    void Application::Run()
    {
        // For debugging
        Float32 timer = 0.0f;
        Int32 frameCount = 0;

        // For physics
        constexpr Float32 FixedDeltaTime = 1.0f / 60.0f;
        Float32 accumulator = 0.0f;

        while (mRunning)
        {
            Time::Tick();
            Float32 rawDt = Time::GetUnscaledDeltaTime();
            Float32 dt = Time::GetDeltaTime();

            accumulator += rawDt;

            mWindowManager->UpdateAllWindows(rawDt);

            while (accumulator >= FixedDeltaTime)
            {
                accumulator -= FixedDeltaTime;
            }

            // Checking is the application still running
            if (mWindowManager->ShouldCloseAllWindow())
            {
                mRunning = false;
                KS_CORE_INFO("The application is closing right now.");
                break;
            }

            hInternalOnUpdate(dt);        

            timer += rawDt;
            frameCount++;

            // Debugging
            if (timer >= 1.0f)
            {
                Float32 avgFps = static_cast<Float32>(frameCount) / timer;
                // KS_CORE_TRACE("FPS avg: {} | Delta Time: {} | Frame count: {}", avgFps, (1000.0f / avgFps), frameCount);
                timer = 0.0f;
                frameCount = 0;
            }
        }
        // After escape the loop, we shutdown the application
        pCustomShutdown();
        hInternalShutdown();
    }

    void Application::hInternalOnUpdate(Float32 dt)
    {
        pCustomOnUpdate(dt);
        mLayerStack->OnUpdate(dt);
    }

    void Application::hInternalShutdown()
    {

        mLayerStack->OnDetach();

        mWindowManager.reset();
        mLayerStack.reset();
    }
}