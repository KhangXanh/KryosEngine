/**
 * @file Engine/Event/Event.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Core/Core.hpp"

namespace Kryos
{
    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        WindowHide,
        WindowMoved,
        WindowFocus,
        WindowLostFocus,
    };

    /**
     * @brief
     * Event category
     * @note
     * ```text
     * - We use BIT() for bitmask
     * ```
     */
    enum EventCategory : Int32
    {
        None = 0,
        EventInput = BIT(0),
        EventWindow = BIT(1),
        EventKeyboard = BIT(2),
        EventMouse = BIT(3),
        EventMouseButton = BIT(4)
    };

    class KRYOS_API IEvent
    {
    public:
        virtual ~IEvent() = default;

        virtual EventType GetType() const = 0;
        virtual Int32 GetCategory() const = 0;
        virtual const char *GetName() const = 0;

        inline bool IsHandled() const { return pHandled; }

        inline bool IsInCategory(Int32 category)
        {
            return GetCategory() & category;
        }

    protected:
        bool pHandled = false;
        friend class EventDispatcher;
    };

    class EventDispatcher
    {
    public:
        EventDispatcher(IEvent &event) : mEvent(event) {}

        /**
         * @brief This function dispatch event if type match
         * @tparam T        Concrete event type (Ex: WindowCloseEvent)
         * @tparam Func     Callable take T& and return a bool
         * @return true if type match
         *
         * @note
         * ```text
         * - This function returns bool:
         *  + true - event consumed (mEvent.handled = true)
         *  + false - The event continues to be forwarded
         * ```
         */
        template <typename T, typename Func>
        bool Dispatch(const Func &func)
        {
            if (mEvent.GetType() == T::StaticType())
            {
                mEvent.pHandled |= func(static_cast<T &>(mEvent));
                return true;
            }
            return false;
        }

    private:
        IEvent &mEvent;
    };
}
