/**
 * @file Engine/Time/Time.cpp
 * This is a Kryos Engine Implementation file
 */

#include "Time.hpp"

namespace Kryos
{
    void Time::Init()
    {
        sLastTime = Clock::now();
    }

    void Time::Tick()
    {
        auto now = Clock::now();
        Float32 raw = std::chrono::duration<Float32>(now - sLastTime).count();
        sLastTime = now;

        sRawDeltaTime = raw;

        Float32 clampedDeltaTime = std::min(raw, 1.0f / 30.0f);
        sDeltaTime = clampedDeltaTime * sTimeScale;
        sTotalTime += sDeltaTime;
        sFrameCount++;
    }

    void Time::SetTimeScale(Float32 scale)
    {
        // If the scale is negative
        // We will set the 'sTimeScale' to 0.0f
        sTimeScale = std::max(scale, 0.0f);
    }
}
