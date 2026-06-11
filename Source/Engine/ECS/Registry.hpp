/**
 * @file Engine/ECS/Registry.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "ComponentPool.hpp"
#include "Core/Memory/SmartPointers.hpp"

namespace Kryos::ECS
{
    /**
     * @brief
     * A class to manage entities, components (A manager in ECS)
     */
    class Registry
    {
    public:
        Entity CreateEntity();
        void DestoyEntity(Entity entity);

        template<typename T>
        void AddComponent(Entity entity, T component)
        {
            HGetOrCreatePool<T>()->Insert(entity, component);
        }

        template<typename T>
        T &GetComponent(Entity entity)
        {
            return HGetOrCreatePool<T>()->Get(entity);
        }

    private:
        /**
         * @brief
         * This function is a helper to create OR create a pool
         * @note
         * ```text
         * - it returns the component if its type already exists 
         * - otherwise, it creates a new component pool
         * ```
         */
        template<typename T>
        TSharedPtr<ComponentPool<T>> HGetOrCreatePool()
        {
            const char* typeName = typeid(T).name();
            if (!mComponentPools.Contains(typeName))
            {
                mComponentPools[typeName] = CreateSharedPtr<ComponentPool<T>>();
            }
            return std::static_pointer_cast<ComponentPool<T>>(mComponentPools[typeName]);
        }

    private: 
        THashTable<const char*, TSharedPtr<IComponentPool>> mComponentPools;
        THashTable<Entity, SizeType> mEntityIndices;
        TDynamicArray<Entity> mEntities;
    };
}