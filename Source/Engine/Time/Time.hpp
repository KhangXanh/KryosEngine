/**
 * @file Engine/Time/Time.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include <chrono>
#include "Core/Core.hpp"

namespace Kryos
{
    class Application;

    /**
     * @brief A class for calculate and handle time
     * @note
     * ```text
     * - This class can be manage with only 'Application' class
     * - Other classes can get value from 'Time' class but can't manage it
     * ```
     */
    class KRYOS_API Time
    {
    public:
        KS_NODISCARD static Float32 GetUnscaledDeltaTime() { return sRawDeltaTime; }
        KS_NODISCARD static Float32 GetDeltaTime() { return sDeltaTime; }
        KS_NODISCARD static Float32 GetTotalTime() { return sTotalTime; }
        KS_NODISCARD static Float32 GetFPS() { return sDeltaTime < 0.0f ? 1.0f / sRawDeltaTime : 0.0f; }
        KS_NODISCARD static UInt32 GetFrameCount() { return sFrameCount; }
        KS_NODISCARD static Float32 GetTimeScale() { return sTimeScale; }

    private:
        static void Init();
        static void Tick();
        static void SetTimeScale(Float32 scale);

        using Clock = std::chrono::steady_clock;
        using TimePoint = std::chrono::time_point<Clock>;

        static inline TimePoint sLastTime;
        static inline Float32 sRawDeltaTime = 0.0f;
        static inline Float32 sDeltaTime = 0.0f;
        static inline Float32 sTotalTime = 0.0f;
        static inline Float32 sTimeScale = 1.0f;
        static inline UInt32 sFrameCount = 0;

        friend class Application;
    };  
}