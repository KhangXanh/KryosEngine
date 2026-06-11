/**
 * @file Engine/ECS/ComponetPool.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "ECSCommon.hpp"
#include "Core/Container/Arrays/DynamicArray.hpp"
#include "Core/Container/Hashs/HashTable.hpp"

namespace Kryos::ECS
{
    /**
     * @brief
     * A tightly-packed contiguous storage (Sparse Set) for a specific component type T
     */
    template<typename T>
    class ComponentPool : public IComponentPool
    {
    public:
        void Insert(Entity&entity, T component)
        {
            SizeType newIndex = mDenseArray.GetSize();
            mSparseTable[entity] = newIndex;
            mIndexToEntityTable[newIndex] = entity;
            mDenseArray.PushBack(component);
        }

        T* Get(Entity entity) const
        {
            return mDenseArray[mSparseTable[entity]];
        }

        void EntityDestroyed(Entity entity) override
        {
            if (mSparseTable.Contains(entity))
            {
                SizeType indexToDestroy = mSparseTable[entity];
                SizeType lastIndex = mDenseArray.GetSize() - 1;

                mDenseArray[indexToDestroy] = mDenseArray[lastIndex];

                Entity lastEntity = mIndexToEntityTable[lastIndex];
                mSparseTable[lastEntity] = indexToDestroy;
                mIndexToEntityTable[indexToDestroy] = lastEntity;

                mDenseArray.PopBack();
                mSparseTable.erase(entity);
                mIndexToEntityTable.erase(lastIndex);
            }
        }

    private:
        TDynamicArray<T> mDenseArray;
        THashTable<Entity, SizeType> mSparseTable;
        THashTable<SizeType, Entity> mIndexToEntityTable;
    };
}