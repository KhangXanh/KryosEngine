/**
 * @file Engine/Core/LayerStack.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Layer.hpp"

namespace Kryos
{
    class KRYOS_API LayerStack
    {
    public:
        /**
         * @brief
         * This function pushes a layer to the 'mLayers'
         * @param[in] layer A unique pointer of the layer
         */
        void PushLayer(TUniquePtr<ILayer> layer);

        /**
         * @brief
         * This function pops a layer to the 'mLayers'
         * @param[in] layer A unique pointer of the layer
         */
        void PopLayer(TUniquePtr<ILayer> layer);

        /**
         * @brief
         * This function updates every layers have in 'mLayers' every frame
         */
        void OnUpdate(Float32 dt);

        /**
         * @brief
         * This function calls the detach event of each layer
         */
        void OnDetach();

        /**
         * @brief 
         * This function take event and ask if any layer need that
         * @note
         * For now we don't need this
         */
        void OnEvent(IEvent &event);
    
    private:
        /**
         * @brief
         * A dynamic array (a.k.a std::vector) contains layers
         */
        TDynamicArray<TUniquePtr<ILayer>> mLayers;
    };
}