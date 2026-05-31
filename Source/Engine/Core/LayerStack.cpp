/**
 * @file Engine/Core/LayerStack.cpp
 * This is a Kryos Engine Implementation file
 */

#define USE_CORE_LOG
#include "Logging/Log.hpp"

#include "LayerStack.hpp"

namespace Kryos
{
    void LayerStack::PushLayer(TUniquePtr<ILayer> layer)
    {
        // We attach the layer first
        // Then we push it to the 'mLayers'
        layer->OnAttach();
        mLayers.push_back(std::move(layer));
    }

    void LayerStack::PopLayer(TUniquePtr<ILayer> layer)
    {
        // We detach the layer first
        // Then we pop it to the 'mLayers'
        layer->OnDetach();
        mLayers.erase(std::remove(mLayers.begin(), mLayers.end(), layer), mLayers.end());
    }

    void LayerStack::OnUpdate(Float32 dt)
    {
        for (const auto &layer : mLayers)
        {
            layer->OnUpdate(dt);
        }
    }

    void LayerStack::OnDetach()
    {
        for (const auto &layer : mLayers)
        {
            layer->OnDetach();
        }
        mLayers.clear();
        KS_CORE_INFO("The layer stack has been detached successfully.");
    }

    void LayerStack::OnEvent(IEvent &event)
    {
        for (auto &layer : mLayers)
        {
            layer->OnEvent(event);
            if (event.IsHandled()) break;
        }
    }
}