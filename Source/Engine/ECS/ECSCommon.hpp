/**
 * @file Engine/ECS/ECSCommon.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Core/UUID/UUID.hpp"

namespace Kryos::ECS
{
    using Entity = UInt64;

    /**
     * @brief
     * A base interface class for all templated component pools
     * @note
     * ```text
     * - We need this class to enable type-erased polymorphic component storage and cleanup
     * ```
     */
    class IComponentPool
    {
    public:
        virtual ~IComponentPool() = default;

        /// @brief This function triggers concrete pool cleanup via runtime polymorphism
        virtual void EntityDestroyed(Entity entity) = 0;
    };
}