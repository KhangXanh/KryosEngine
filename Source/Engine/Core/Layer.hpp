/**
 * @file Engine/Core/Layer.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Core.hpp"
#include "Event/Event.hpp"

namespace Kryos
{
    /**
     * @brief
     * An interface layer class (Base class)
     * @note
     * ```text
     * - We will inherit more layer from this class
     * - We don't need to override these functions because they already got a body
     * ```
     */
    class KRYOS_API ILayer
    {
    public:
        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Float32 dt) {}
        virtual void OnEvent(IEvent &event) {}
    };
}