/**
 * @file Engine/Core/Containers/Arrays/DynamicArray.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Core/Memory/Allocator.hpp"

namespace Kryos
{
    /**
     * @brief
     * A dynamic array for Kryos Engine
     */
    template <typename T, typename AllocatorType = TAllocator<T>>
    class KRYOS_API TDynamicArray
    {
    public:
        using SizeType = unsigned long long;
        using Iterator = T *;
        using ConstIterator = const T *;

        /// @brief This function is for Kryos
        Iterator Begin() noexcept { return mData; }
        /// @brief This function is for Kryos
        Iterator End() noexcept { return mData + mSize; }
        /// @brief This function is for Kryos
        ConstIterator Begin() const noexcept { return mData; }
        /// @brief This function is for Kryos
        ConstIterator End() const noexcept { return mData + mSize; }
        /// @brief This function is for Kryos
        ConstIterator CBegin() const noexcept { return mData; }
        /// @brief This function is for Kryos
        ConstIterator CEnd() const noexcept { return mData + mSize; }

        /// @brief This function is for <algorithm>
        Iterator begin() noexcept { return Begin(); }
        /// @brief This function is for <algorithm>
        Iterator end() noexcept { return End(); }
        /// @brief This function is for <algorithm>
        ConstIterator begin() const noexcept { return Begin(); }
        /// @brief This function is for <algorithm>
        ConstIterator end() const noexcept { return End(); }
        /// @brief This function is for <algorithm>
        ConstIterator cbegin() const noexcept { return CBegin(); }
        /// @brief This function is for <algorithm>
        ConstIterator cend() const noexcept { return CEnd(); }

        /// @brief Constructor
        TDynamicArray() noexcept
            : mData(nullptr), mSize(0), mCapacity(0) {}

        /// @brief Fill constructor
        explicit TDynamicArray(SizeType count, const T &value = T{})
            : mData(nullptr), mSize(0), mCapacity(0)
        {
            Reserve(count);
            for (SizeType i = 0; i < count; ++i)
                mAllocator.Construct(&mData[i], value);
            mSize = count;
        }

        /// @brief Initializer list
        TDynamicArray(std::initializer_list<T> init)
            : mData(nullptr), mSize(0), mCapacity(0)
        {
            Reserve(init.size());
            for (const T &val : init)
                PushBack(val);
        }

        /// @brief Copy constructor
        TDynamicArray(const TDynamicArray &other)
            : mData(nullptr), mSize(0), mCapacity(0)
        {
            Reserve(other.mSize);
            for (SizeType i = 0; i < other.mSize; ++i)
                mAllocator.Construct(&mData[i], other.mData[i]);
            mSize = other.mSize;
        }

        /// @brief Move constructor
        TDynamicArray(TDynamicArray &&other) noexcept
            : mData(other.mData), mSize(other.mSize), mCapacity(other.mCapacity),
              mAllocator(std::move(other.mAllocator))
        {
            other.mData = nullptr;
            other.mSize = 0;
            other.mCapacity = 0;
        }

        /// @brief Destructor
        ~TDynamicArray()
        {
            Clear();
            if (mData)
                mAllocator.Deallocate(mData, mCapacity);
        }

        TDynamicArray &operator=(const TDynamicArray &other)
        {
            if (this != &other)
            {
                TDynamicArray temp(other);
                Swap(temp);
            }
            return *this;
        }

        TDynamicArray &operator=(TDynamicArray &&other) noexcept
        {
            if (this == &other)
                return *this;
            Clear();
            if (mData)
                mAllocator.Deallocate(mData, mCapacity);

            mData = other.mData;
            mSize = other.mSize;
            mCapacity = other.mCapacity;
            mAllocator = std::move(other.mAllocator);

            other.mData = nullptr;
            other.mSize = 0;
            other.mCapacity = 0;
            return *this;
        }

        // Access operator
        T &operator[](SizeType index) { return mData[index]; }
        const T &operator[](SizeType index) const { return mData[index]; }

        // Bounds-checked access
        T &At(SizeType index)
        {
            if (index >= mSize)
            {
                return nullptr;
            }
            return mData[index];
        }
        const T &At(SizeType index) const
        {
            if (index >= mSize)
            {
                return nullptr;
            }
            return mData[index];
        }

        T &Front() { return mData[0]; }
        const T &Front() const { return mData[0]; }

        T &Back() { return mData[mSize - 1]; }
        const T &Back() const { return mData[mSize - 1]; }

        T *GetData() noexcept { return mData; }
        const T *GetData() const noexcept { return mData; }

        bool IsEmpty() const noexcept { return mSize == 0; }

        SizeType GetSize() const noexcept { return mSize; }
        SizeType GetCapacity() const noexcept { return mCapacity; }

        void Reserve(SizeType newCapacity)
        {
            if (newCapacity <= mCapacity)
                return;

            T *newMemory = mAllocator.Allocate(newCapacity);
            for (SizeType i = 0; i < mSize; ++i)
            {
                mAllocator.Construct(&newMemory[i], std::move(mData[i]));
                mAllocator.Destroy(&mData[i]);
            }
            if (mData)
                mAllocator.Deallocate(mData, mCapacity);

            mData = newMemory;
            mCapacity = newCapacity;
        }

        // Shrink allocated memory to exactly fit mSize
        void ShrinkToFit()
        {
            if (mSize == mCapacity)
                return;
            if (mSize == 0)
            {
                if (mData)
                    mAllocator.Deallocate(mData, mCapacity);
                mData = nullptr;
                mCapacity = 0;
                return;
            }

            T *newMemory = mAllocator.Allocate(mSize);
            for (SizeType i = 0; i < mSize; ++i)
            {
                mAllocator.Construct(&newMemory[i], std::move(mData[i]));
                mAllocator.Destroy(&mData[i]);
            }
            mAllocator.Deallocate(mData, mCapacity);
            mData = newMemory;
            mCapacity = mSize;
        }

        // Resize - grows or shrinks the logical size
        void Resize(SizeType newSize, const T &value = T{})
        {
            if (newSize < mSize)
            {
                for (SizeType i = newSize; i < mSize; ++i)
                    mAllocator.Destroy(&mData[i]);
                mSize = newSize;
            }
            else if (newSize > mSize)
            {
                Reserve(newSize);
                for (SizeType i = mSize; i < newSize; ++i)
                    mAllocator.Construct(&mData[i], value);
                mSize = newSize;
            }
        }

        void PushBack(const T &value)
        {
            HCheckAndGrow();
            mAllocator.Construct(&mData[mSize++], value);
        }

        void PushBack(T &&value)
        {
            HCheckAndGrow();
            mAllocator.Construct(&mData[mSize++], std::move(value));
        }

        // Construct element in-place, avoids redundant copy/move
        template <typename... Args>
        T &EmplaceBack(Args &&...args)
        {
            HCheckAndGrow();
            mAllocator.Construct(&mData[mSize], std::forward<Args>(args)...);
            return mData[mSize++];
        }

        void PopBack()
        {
            if (mSize == 0)
                return;
            mAllocator.Destroy(&mData[--mSize]);
        }

        // Insert a single value at index, shifting elements right
        void Insert(SizeType index, const T &value)
        {
            if (index > mSize)
            {
                return;
            }
            HCheckAndGrow();
            // Shift right
            for (SizeType i = mSize; i > index; --i)
            {
                mAllocator.Construct(&mData[i], std::move(mData[i - 1]));
                mAllocator.Destroy(&mData[i - 1]);
            }
            mAllocator.Construct(&mData[index], value);
            ++mSize;
        }

        // Erase element at index — O(n) shift
        void Erase(SizeType index)
        {
            if (index >= mSize)
            {
                return;
            }
            mAllocator.Destroy(&mData[index]);
            for (SizeType i = index; i < mSize - 1; ++i)
            {
                mAllocator.Construct(&mData[i], std::move(mData[i + 1]));
                mAllocator.Destroy(&mData[i + 1]);
            }
            --mSize;
        }

        // O(1) erase — swaps target with last element, no order guarantee
        void EraseSwap(SizeType index)
        {
            if (index >= mSize)
            {
                return;
            }
            if (index != mSize - 1)
            {
                mAllocator.Destroy(&mData[index]);
                mAllocator.Construct(&mData[index], std::move(mData[mSize - 1]));
                mAllocator.Destroy(&mData[mSize - 1]);
            }
            else
            {
                mAllocator.Destroy(&mData[index]);
            }
            --mSize;
        }

        void Clear() noexcept
        {
            for (SizeType i = 0; i < mSize; ++i)
                mAllocator.Destroy(&mData[i]);
            mSize = 0;
        }

        void Swap(TDynamicArray &other) noexcept
        {
            std::swap(mData, other.mData);
            std::swap(mSize, other.mSize);
            std::swap(mCapacity, other.mCapacity);
            std::swap(mAllocator, other.mAllocator);
        }

        bool operator==(const TDynamicArray &other) const
        {
            if (mSize != other.mSize)
                return false;
            for (SizeType i = 0; i < mSize; ++i)
                if (!(mData[i] == other.mData[i]))
                    return false;
            return true;
        }

        bool operator!=(const TDynamicArray &other) const
        {
            return !(*this == other);
        }

    private:
        void HCheckAndGrow()
        {
            if (mSize < mCapacity)
                return;
            SizeType newCapacity = (mCapacity == 0) ? 2 : mCapacity * 2;
            Reserve(newCapacity);
        }

    private:
        T *mData = nullptr;
        SizeType mSize = 0;
        SizeType mCapacity = 0;
        AllocatorType mAllocator;
    };
}