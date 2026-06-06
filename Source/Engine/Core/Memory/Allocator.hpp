/**
 * @file Engine/Core/Memory/Allocator.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include <new> // For Placement New
#include <utility>
#include "Core/Core.hpp"

namespace Kryos
{
    /**
     * @brief
     * A memory allocator for Kryos Engine
     */
    template <typename T>
    class KRYOS_API TAllocator
    {
    public:
        using ValueType = T;
        using Pointer = T *;
        using ConstPointer = const T *;
        using SizeType = unsigned long long;

        TAllocator() noexcept = default;

        template <typename U>
        TAllocator(const TAllocator<U> &) noexcept {}

        T *Allocate(SizeType count) noexcept
        {
            if (count == 0)
                return nullptr;

            void *rawMemory = ::operator new(count * sizeof(T));
            return static_cast<Pointer>(rawMemory);
        }

        void Deallocate(Pointer p, SizeType count) noexcept
        {
            if (p == nullptr)
                return;
            ::operator delete(p);
        }

        template <typename... Args>
        void Construct(Pointer p, Args &&...args)
        {
            // Using placement new
            ::new (static_cast<void *>(p)) T(std::forward<Args>(args)...);
        }

        void Destroy(Pointer p)
        {
            if (p != nullptr)
                p->~T();
        }
    };

    template <typename T, typename U>
    bool operator==(const TAllocator<T> &, const TAllocator<U> &) noexcept
    {
        return true;
    }

    template <typename T, typename U>
    bool operator!=(const TAllocator<T> &, const TAllocator<U> &) noexcept
    {
        return false;
    }
}