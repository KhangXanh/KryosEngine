/**
 * @file Engine/ECS/Registry.cpp
 * This is a Kryos Engine Implementation file
 */

#include "Registry.hpp"

namespace Kryos::ECS
{
    Entity Registry::CreateEntity()
    {
        Entity ID = UUID();
        mEntityIndices[ID] = mEntities.GetSize();
        mEntities.PushBack(ID);
        return ID;
    }

    void Registry::DestoyEntity(Entity entity)
    {
        for (auto &[name, pool] : mComponentPools)
        {
            pool->EntityDestroyed(entity);
        }
        if (mEntityIndices.contains(entity))
        {
            size_t removedIndex = mEntityIndices[entity];
            size_t lastIndex = mEntities.GetSize() - 1;
            Entity lastEntity = mEntities[lastIndex];

            mEntities[removedIndex] = lastEntity;

            mEntityIndices[lastEntity] = removedIndex;
            mEntities.PopBack();
            mEntityIndices.erase(entity);
        }
    }

    /**
     * @note
     * ```text
     * - We don't implement the 'HGetOrCreatePool', GetComponent' and 'AddComponent' here
     * - Because they are template functions
     * ```
     */
}