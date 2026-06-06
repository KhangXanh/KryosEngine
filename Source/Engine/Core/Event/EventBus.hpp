/**
 * @file Engine/Core/Event/EventBus.hpp
 * This is a Kryos Engine Library header
 */

#pragma once
//#include "Core/Core.hpp"
#include "Core/Containers/Arrays/DynamicArray.hpp"

namespace Kryos
{
    /**
     * @brief
     * A special static class to send and give the event to needed systems
     */
    class EventBus
    {
    public:
        /**
         * @brief
         * This function pushes an event to the bus
         */
        template <typename T, typename... Args>
        inline static void Publish(Args &&...args)
        {
            HGetQueue<T>().EmplaceBack(std::forward<Args>(args)...);
        }

        /**
         * @brief
         * This function returns the entire list of T-type events in this frame for processing
         */
        template <typename T>
        inline static const TDynamicArray<T> &GetEvents()
        {
            return HGetQueue<T>();
        }

        /**
         * @brief
         * This function clears all the event at the end of the frame
         */
        static KRYOS_API void Clear();

        /**
         * @brief
         * This function initializes the event bus system
         * @note
         * ```text
         * - you must call this only once
         * ```
         */
        static KRYOS_API void Init();

        /**
         * @brief
         * This function cleans up and shuts down the event bus system
         * @note
         * ```text
         * - You must call this only once
         * ```
         */
        static KRYOS_API void Shutdown();

    private:
        /**
         * @brief
         * This function returns a unique TDynamicArray<T> array for EACH Event type T
         */
        template <typename T>
        static TDynamicArray<T> &HGetQueue()
        {
            static TDynamicArray<T> queue;

            // Automatically register the clear function of this array to the system cleanup list
            static bool registered = []()
            {
                if (sData)
                {
                    sData->ClearFunctions.PushBack([]() { queue.Clear(); });
                }
                return true;
            }();

            return queue;
        }

    private:
        /**
         * @brief
         * A struct to implement the event bus
         */
        struct EventBusStorage
        {
            TDynamicArray<std::function<void()>> ClearFunctions;
        };

        inline static EventBusStorage *sData = nullptr;
    };
}