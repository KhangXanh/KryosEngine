/**
 * @file Engine/Core/Event/EventBus.cpp
 * This is a Kryos Engine Implementation file
 */

#include "EventBus.hpp"

namespace Kryos
{
    void EventBus::Init()
    {
        if (!sData)
        {
            sData = new EventBusStorage;
        }
    }

    void EventBus::Shutdown()
    {
        if (sData)
        {
            Clear();

            delete sData;
            sData = nullptr;
        }
    }

    void EventBus::Clear()
    {
        if (sData)
        {
            for (const auto &clearFunc : sData->ClearFunctions)
            {
                clearFunc();
            }
        }
    }
}