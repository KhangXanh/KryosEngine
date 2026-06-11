/**
 * @file Engine/Core/Container/Hashs/HashTable.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Core/Memory/Allocator.hpp"

/**
 * @todo
 * Find and fix all problems and error
 */

namespace Kryos
{
    /**
     * @brief
     * A default hash functor (FNV-1a over raw bytes) for Kryos Engine
     */
    template <typename Key>
    struct THasher
    {
        SizeType operator()(const Key &key) const noexcept
        {
            const unsigned char *bytes = reinterpret_cast<const unsigned char *>(&key);
            SizeType hash = 14695981039346656037ULL;
            for (SizeType i = 0; i < sizeof(Key); ++i)
            {
                hash ^= static_cast<SizeType>(bytes[i]);
                hash *= 1099511628211ULL;
            }
            return hash;
        }
    };

    template <>
    struct THasher<UInt32>
    {
        SizeType operator()(UInt32 key) const noexcept
        {
            SizeType x = static_cast<SizeType>(key);
            x = ((x >> 16) ^ x) * 0x45d9f3bULL;
            x = ((x >> 16) ^ x) * 0x45d9f3bULL;
            return (x >> 16) ^ x;
        }
    };

    template <>
    struct THasher<UInt64>
    {
        SizeType operator()(UInt64 key) const noexcept
        {
            SizeType x = key;
            x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
            x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
            return x ^ (x >> 31);
        }
    };

    /**
     * @brief
     * A default equality comparator for Kryos Engine
     */
    template <typename Key>
    struct TEqual
    {
        bool operator()(const Key &a, const Key &b) const noexcept
        {
            return a == b;
        }
    };

    /**
     * @brief
     * A lightweight key-value pair for Kryos Engine
     */
    template <typename First, typename Second>
    struct TPair
    {
        First first;
        Second second;

        TPair() = default;

        TPair(const First &f, const Second &s)
            : first(f), second(s) {}

        TPair(First &&f, Second &&s)
            : first(std::move(f)), second(std::move(s)) {}

        bool operator==(const TPair &o) const { return first == o.first && second == o.second; }
        bool operator!=(const TPair &o) const { return !(*this == o); }
    };

    /**
     * @brief
     * A custom hash table for Kryos Engine
     * @note
     * ```text
     * - This is using Robin Hood linear probing
     * - Design:
     *   + PSL (probe-sequence length) stored per slot; EMPTY_SLOT == vacant
     *   + Robin Hood invariant: rich slots (low PSL) yield to poor ones (high PSL)
     *   + 2x growth; starting capacity 8
     *   + Backward-shift deletion — no tombstones, no performance decay over time
     *   + Capacity always a power of 2 — modulo replaced by cheap bitwise AND
     *   + Load factor hard cap at 75 % (same as most std implementations)
     * ```
     */
    template <
        typename Key,
        typename Value,
        typename Hasher = THasher<Key>,
        typename KeyEqual = TEqual<Key>>
    class KRYOS_API THashTable
    {
    public:
        using KeyType = Key;
        using MappedType = Value;
        using PairType = TPair<Key, Value>;

        static constexpr SizeType EMPTY_PSL = ~SizeType(0);
        static constexpr SizeType INITIAL_CAP = 8;

    private:
        /// @brief Internal slot
        struct Slot
        {
            /// @brief The Key (<Key, Value>)
            Key key;
            /// @brief The Value (<Key, Value>)
            Value value;
            /// @brief Probe-sequence length, EMPTY_PSL when vacant
            SizeType psl;

            // Since this is just a member struct and it pretty small
            // I'll use lowercase for easier typing :D
        };

        using SlotAlloc = TAllocator<Slot>;

    public:
        /// @brief Iterator
        struct Iterator
        {
            Slot *mSlots = nullptr;
            SizeType mCapacity = 0;
            SizeType mIndex = 0;

            Iterator() noexcept = default;
            Iterator(Slot *slots, SizeType cap, SizeType idx) noexcept
                : mSlots(slots), mCapacity(cap), mIndex(idx) {}

            PairType &operator*() const noexcept
            {
                return reinterpret_cast<PairType &>(mSlots[mIndex]);
            }
            PairType *operator->() const noexcept
            {
                return reinterpret_cast<PairType *>(&mSlots[mIndex]);
            }

            Iterator &operator++() noexcept
            {
                ++mIndex;
                while (mIndex < mCapacity && mSlots[mIndex].psl == EMPTY_PSL)
                    ++mIndex;
                return *this;
            }

            Iterator operator++(int) noexcept
            {
                Iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            bool operator==(const Iterator &o) const noexcept { return mIndex == o.mIndex; }
            bool operator!=(const Iterator &o) const noexcept { return mIndex != o.mIndex; }
        };

        /// @brief ConstIterator
        struct ConstIterator
        {
            const Slot *mSlots = nullptr;
            SizeType mCapacity = 0;
            SizeType mIndex = 0;

            ConstIterator() noexcept = default;
            ConstIterator(const Slot *slots, SizeType cap, SizeType idx) noexcept
                : mSlots(slots), mCapacity(cap), mIndex(idx) {}

            /// @note Implicit conversion from mutable iterator
            ConstIterator(const Iterator &it) noexcept
                : mSlots(it.mSlots), mCapacity(it.mCapacity), mIndex(it.mIndex) {}

            const PairType &operator*() const noexcept
            {
                return reinterpret_cast<const PairType &>(mSlots[mIndex]);
            }
            const PairType *operator->() const noexcept
            {
                return reinterpret_cast<const PairType *>(&mSlots[mIndex]);
            }

            ConstIterator &operator++() noexcept
            {
                ++mIndex;
                while (mIndex < mCapacity && mSlots[mIndex].psl == EMPTY_PSL)
                    ++mIndex;
                return *this;
            }

            ConstIterator operator++(int) noexcept
            {
                ConstIterator tmp = *this;
                ++(*this);
                return tmp;
            }

            bool operator==(const ConstIterator &o) const noexcept { return mIndex == o.mIndex; }
            bool operator!=(const ConstIterator &o) const noexcept { return mIndex != o.mIndex; }
        };

        // STL shims for range-for / <algorithm>
        Iterator begin() noexcept { return Begin(); }
        Iterator end() noexcept { return End(); }
        ConstIterator begin() const noexcept { return Begin(); }
        ConstIterator end() const noexcept { return End(); }
        ConstIterator cbegin() const noexcept { return CBegin(); }
        ConstIterator cend() const noexcept { return CEnd(); }

        // Kryos-style named iterators
        Iterator Begin() noexcept
        {
            SizeType i = 0;
            while (i < mCapacity && mSlots[i].psl == EMPTY_PSL)
                ++i;
            return Iterator(mSlots, mCapacity, i);
        }

        Iterator End() noexcept
        {
            return Iterator(mSlots, mCapacity, mCapacity);
        }

        ConstIterator Begin() const noexcept
        {
            SizeType i = 0;
            while (i < mCapacity && mSlots[i].psl == EMPTY_PSL)
                ++i;
            return ConstIterator(mSlots, mCapacity, i);
        }

        ConstIterator End() const noexcept { return ConstIterator(mSlots, mCapacity, mCapacity); }
        ConstIterator CBegin() const noexcept { return Begin(); }
        ConstIterator CEnd() const noexcept
        {
            return End();
        }

        THashTable() noexcept
            : mSlots(nullptr), mSize(0), mCapacity(0) {}

        /// @brief Construct with a reserved bucket count (rounded up to next power of 2).
        explicit THashTable(SizeType reserveCount)
            : mSlots(nullptr), mSize(0), mCapacity(0)
        {
            HAllocSlots(HNextPow2(reserveCount));
        }

        THashTable(const THashTable &other)
            : mSlots(nullptr), mSize(0), mCapacity(0)
        {
            if (other.mCapacity > 0)
            {
                HAllocSlots(other.mCapacity);
                for (SizeType i = 0; i < other.mCapacity; ++i)
                {
                    if (other.mSlots[i].psl != EMPTY_PSL)
                        HInsertSlot(other.mSlots[i].key, other.mSlots[i].value);
                }
            }
        }

        THashTable(THashTable &&other) noexcept
            : mSlots(other.mSlots),
              mSize(other.mSize),
              mCapacity(other.mCapacity),
              mHasher(std::move(other.mHasher)),
              mEqual(std::move(other.mEqual)),
              mAlloc(std::move(other.mAlloc))
        {
            other.mSlots = nullptr;
            other.mSize = 0;
            other.mCapacity = 0;
        }

        ~THashTable() { HFreeSlots(); }

        THashTable &operator=(const THashTable &other)
        {
            if (this != &other)
            {
                THashTable tmp(other);
                Swap(tmp);
            }
            return *this;
        }

        THashTable &operator=(THashTable &&other) noexcept
        {
            if (this != &other)
            {
                HFreeSlots();
                mSlots = other.mSlots;
                mSize = other.mSize;
                mCapacity = other.mCapacity;
                mHasher = std::move(other.mHasher);
                mEqual = std::move(other.mEqual);
                mAlloc = std::move(other.mAlloc);

                other.mSlots = nullptr;
                other.mSize = 0;
                other.mCapacity = 0;
            }
            return *this;
        }

        SizeType GetSize() const noexcept { return mSize; }
        SizeType GetCapacity() const noexcept { return mCapacity; }
        bool IsEmpty() const noexcept { return mSize == 0; }

        /**
         * @note
         * This function inserts or overwrites, returns {iterator, true} if a new key was added
         */
        TPair<Iterator, bool> Insert(const Key &key, const Value &value)
        {
            HGrowIfNeeded();
            SizeType idx = HInsertSlot(key, value);
            bool inserted = (mSlots[idx].value == value); // always true for new, approx for overwrite
            // Actually track insertion properly:
            return {Iterator(mSlots, mCapacity, idx), mLastInsertWasNew};
        }

        TPair<Iterator, bool> Insert(Key &&key, Value &&value)
        {
            HGrowIfNeeded();
            SizeType idx = HInsertSlotMove(std::move(key), std::move(value));
            return {Iterator(mSlots, mCapacity, idx), mLastInsertWasNew};
        }

        /// @brief Construct value in-place from args
        template <typename... Args>
        TPair<Iterator, bool> Emplace(const Key &key, Args &&...args)
        {
            HGrowIfNeeded();
            Value v(std::forward<Args>(args)...);
            SizeType idx = HInsertSlotMove(key, std::move(v));
            return {Iterator(mSlots, mCapacity, idx), mLastInsertWasNew};
        }

        /// @brief inserts default-constructed value if key absent
        Value &operator[](const Key &key)
        {
            HGrowIfNeeded();
            SizeType idx = HInsertDefault(key);
            return mSlots[idx].value;
        }

        Value &operator[](Key &&key)
        {
            HGrowIfNeeded();
            SizeType idx = HInsertDefaultMove(std::move(key));
            return mSlots[idx].value;
        }

        /// @brief Erase by key, returns true if the key existed
        bool Erase(const Key &key)
        {
            SizeType idx = HFindIndex(key);
            if (idx == EMPTY_PSL)
                return false;
            HEraseAt(idx);
            return true;
        }

        /// @brief Erase by iterator, returns iterator to the next valid element
        Iterator Erase(Iterator it)
        {
            HEraseAt(it.mIndex);
            SizeType next = it.mIndex;
            while (next < mCapacity && mSlots[next].psl == EMPTY_PSL)
                ++next;
            return Iterator(mSlots, mCapacity, next);
        }

        void Clear() noexcept
        {
            for (SizeType i = 0; i < mCapacity; ++i)
            {
                if (mSlots[i].psl != EMPTY_PSL)
                {
                    mSlots[i].key.~Key();
                    mSlots[i].value.~Value();
                    mSlots[i].psl = EMPTY_PSL;
                }
            }
            mSize = 0;
        }

        void Swap(THashTable &other) noexcept
        {
            std::swap(mSlots, other.mSlots);
            std::swap(mSize, other.mSize);
            std::swap(mCapacity, other.mCapacity);
            std::swap(mHasher, other.mHasher);
            std::swap(mEqual, other.mEqual);
            std::swap(mAlloc, other.mAlloc);
        }

        /// @brief This function forces rehash to at least newBucketCount buckets
        void Rehash(SizeType newBucketCount)
        {
            SizeType cap = HNextPow2(newBucketCount);
            if (cap > mCapacity)
                HRehash(cap);
        }

        Iterator Find(const Key &key) noexcept
        {
            SizeType idx = HFindIndex(key);
            return (idx == EMPTY_PSL) ? End() : Iterator(mSlots, mCapacity, idx);
        }

        ConstIterator Find(const Key &key) const noexcept
        {
            SizeType idx = HFindIndex(key);
            return (idx == EMPTY_PSL) ? End() : ConstIterator(mSlots, mCapacity, idx);
        }

        bool Contains(const Key &key) const noexcept
        {
            return HFindIndex(key) != EMPTY_PSL;
        }

        /// @brief Safe access, returns nullptr if key not found
        Value *TryGet(const Key &key) noexcept
        {
            SizeType idx = HFindIndex(key);
            return (idx == EMPTY_PSL) ? nullptr : &mSlots[idx].value;
        }

        const Value *TryGet(const Key &key) const noexcept
        {
            SizeType idx = HFindIndex(key);
            return (idx == EMPTY_PSL) ? nullptr : &mSlots[idx].value;
        }

        bool operator==(const THashTable &other) const noexcept
        {
            if (mSize != other.mSize)
                return false;
            for (SizeType i = 0; i < mCapacity; ++i)
            {
                if (mSlots[i].psl == EMPTY_PSL)
                    continue;
                SizeType idx = other.HFindIndex(mSlots[i].key);
                if (idx == EMPTY_PSL)
                    return false;
                if (!(mSlots[i].value == other.mSlots[idx].value))
                    return false;
            }
            return true;
        }

        bool operator!=(const THashTable &other) const noexcept
        {
            return !(*this == other);
        }

    private:
        /// @brief This function computes home bucket (capacity is always power-of-2)
        SizeType HBucketOf(const Key &key) const noexcept
        {
            return mHasher(key) & (mCapacity - 1);
        }

        /// @brief This function Allocates 'cap' uninitialised slots, mark all as EMPTY
        void HAllocSlots(SizeType cap)
        {
            mSlots = mAlloc.Allocate(cap);
            mCapacity = cap;
            for (SizeType i = 0; i < cap; ++i)
                mSlots[i].psl = EMPTY_PSL;
        }

        /// @brief function destroys live elements and free memory
        void HFreeSlots() noexcept
        {
            if (!mSlots)
                return;
            for (SizeType i = 0; i < mCapacity; ++i)
            {
                if (mSlots[i].psl != EMPTY_PSL)
                {
                    mSlots[i].key.~Key();
                    mSlots[i].value.~Value();
                }
            }
            mAlloc.Deallocate(mSlots, mCapacity);
            mSlots = nullptr;
            mCapacity = 0;
            mSize = 0;
        }

        /**
         * @brief
         * This function uses Robin Hood insertion (copy variant)
         * @note
         * ```text
         * - Returns the final slot index of the key. Sets mLastInsertWasNew
         * ```
         */
        SizeType HInsertSlot(const Key &key, const Value &value)
        {
            SizeType idx = HBucketOf(key);
            SizeType psl = 0;

            // Working copies — may be displaced during Robin Hood swaps
            Key rKey = key;
            Value rVal = value;
            bool isNew = true;

            // Infinte loop
            for (;;) // I want to try using 'for (;;)' instead of 'while (true)' :D
            {
                Slot &s = mSlots[idx];

                if (s.psl == EMPTY_PSL)
                {
                    // Place into empty slot
                    ::new (static_cast<void *>(&s.key)) Key(std::move(rKey));
                    ::new (static_cast<void *>(&s.value)) Value(std::move(rVal));
                    s.psl = psl;
                    ++mSize;
                    mLastInsertWasNew = isNew;
                    return idx;
                }

                if (mEqual(s.key, key))
                {
                    // Key already present — overwrite value
                    s.value = value;
                    mLastInsertWasNew = false;
                    return idx;
                }

                // Robin Hood: evict rich incumbent (smaller PSL) to make room
                if (s.psl < psl)
                {
                    std::swap(rKey, s.key);
                    std::swap(rVal, s.value);
                    std::swap(psl, s.psl);
                    isNew = false; // we stole someone else's slot
                }

                ++psl;
                idx = (idx + 1) & (mCapacity - 1);
            }
        }

        /// @brief This function uses Robin Hood insertion (move variant)
        SizeType HInsertSlotMove(Key &&key, Value &&value)
        {
            SizeType idx = HBucketOf(key);
            SizeType psl = 0;

            Key rKey = std::move(key);
            Value rVal = std::move(value);

            // Infinte loop
            for (;;)
            {
                Slot &s = mSlots[idx];

                if (s.psl == EMPTY_PSL)
                {
                    ::new (static_cast<void *>(&s.key)) Key(std::move(rKey));
                    ::new (static_cast<void *>(&s.value)) Value(std::move(rVal));
                    s.psl = psl;
                    ++mSize;
                    mLastInsertWasNew = true;
                    return idx;
                }

                if (mEqual(s.key, rKey))
                {
                    s.value = std::move(rVal);
                    mLastInsertWasNew = false;
                    return idx;
                }

                if (s.psl < psl)
                {
                    std::swap(rKey, s.key);
                    std::swap(rVal, s.value);
                    std::swap(psl, s.psl);
                }

                ++psl;
                idx = (idx + 1) & (mCapacity - 1);
            }
        }

        SizeType HInsertSlotMove(const Key& key, Value&& value)
        {
            SizeType idx = HBucketOf(key);
            SizeType psl = 0;

            Key rKey = std::move(key);
            Value rVal = std::move(value);

            // Infinte loop
            for (;;)
            {
                Slot &s = mSlots[idx];

                if (s.psl == EMPTY_PSL)
                {
                    ::new (static_cast<void *>(&s.key)) Key(std::move(rKey));
                    ::new (static_cast<void *>(&s.value)) Value(std::move(rVal));
                    s.psl = psl;
                    ++mSize;
                    mLastInsertWasNew = true;
                    return idx;
                }

                if (mEqual(s.key, rKey))
                {
                    s.value = std::move(rVal);
                    mLastInsertWasNew = false;
                    return idx;
                }

                if (s.psl < psl)
                {
                    std::swap(rKey, s.key);
                    std::swap(rVal, s.value);
                    std::swap(psl, s.psl);
                }

                ++psl;
                idx = (idx + 1) & (mCapacity - 1);
            }
        }

        /// @brief This function inserts with default-constructed value, returns slot index
        SizeType HInsertDefault(const Key &key)
        {
            SizeType idx = HBucketOf(key);
            SizeType psl = 0;

            Key rKey = key;
            Value rVal = Value{};

            // Infinte loop
            for (;;)
            {
                Slot &s = mSlots[idx];

                if (s.psl == EMPTY_PSL)
                {
                    ::new (static_cast<void *>(&s.key)) Key(std::move(rKey));
                    ::new (static_cast<void *>(&s.value)) Value(std::move(rVal));
                    s.psl = psl;
                    ++mSize;
                    return idx;
                }

                if (mEqual(s.key, key))
                    return idx; // already exists — return existing

                if (s.psl < psl)
                {
                    std::swap(rKey, s.key);
                    std::swap(rVal, s.value);
                    std::swap(psl, s.psl);
                }

                ++psl;
                idx = (idx + 1) & (mCapacity - 1);
            }
        }

        SizeType HInsertDefaultMove(Key &&key)
        {
            SizeType idx = HBucketOf(key);
            SizeType psl = 0;

            Key rKey = std::move(key);
            Value rVal = Value{};

            // Infinte loop
            for (;;)
            {
                Slot &s = mSlots[idx];

                if (s.psl == EMPTY_PSL)
                {
                    ::new (static_cast<void *>(&s.key)) Key(std::move(rKey));
                    ::new (static_cast<void *>(&s.value)) Value(std::move(rVal));
                    s.psl = psl;
                    ++mSize;
                    return idx;
                }

                if (mEqual(s.key, rKey))
                    return idx;

                if (s.psl < psl)
                {
                    std::swap(rKey, s.key);
                    std::swap(rVal, s.value);
                    std::swap(psl, s.psl);
                }

                ++psl;
                idx = (idx + 1) & (mCapacity - 1);
            }
        }

        /// @brief This function returns slot index or EMPTY_PSL
        SizeType HFindIndex(const Key &key) const noexcept
        {
            if (mCapacity == 0)
                return EMPTY_PSL;

            SizeType idx = HBucketOf(key);
            SizeType psl = 0;

            for (;;)
            {
                const Slot &s = mSlots[idx];

                // Early-exit: Robin Hood guarantees no element can have a larger PSL
                // than what we would have if it were present, so stop searching here.
                if (s.psl == EMPTY_PSL || s.psl < psl)
                    return EMPTY_PSL;

                if (mEqual(s.key, key))
                    return idx;

                ++psl;
                idx = (idx + 1) & (mCapacity - 1);
            }
        }

        /// @note Backward-shift deletion. No tombstones, O(1) amortised
        void HEraseAt(SizeType idx) noexcept
        {
            mSlots[idx].key.~Key();
            mSlots[idx].value.~Value();
            mSlots[idx].psl = EMPTY_PSL;
            --mSize;

            // Pull each successor one step back while it has PSL > 0
            for (;;)
            {
                SizeType next = (idx + 1) & (mCapacity - 1);
                Slot &succ = mSlots[next];

                if (succ.psl == EMPTY_PSL || succ.psl == 0)
                    break;

                ::new (static_cast<void *>(&mSlots[idx].key)) Key(std::move(succ.key));
                ::new (static_cast<void *>(&mSlots[idx].value)) Value(std::move(succ.value));
                mSlots[idx].psl = succ.psl - 1;

                succ.key.~Key();
                succ.value.~Value();
                succ.psl = EMPTY_PSL;

                idx = next;
            }
        }

        /// @brief Growth
        void HGrowIfNeeded()
        {
            if (mCapacity == 0)
            {
                HAllocSlots(INITIAL_CAP);
                return;
            }
            // Integer equivalent of load > 0.75: size * 4 >= capacity * 3
            if (mSize * 4 >= mCapacity * 3)
                HRehash(mCapacity * 2);
        }

        /// @brief Rehash
        void HRehash(SizeType newCap)
        {
            Slot *oldSlots = mSlots;
            SizeType oldCap = mCapacity;

            mSlots = nullptr;
            mSize = 0;
            mCapacity = 0;

            HAllocSlots(newCap);

            for (SizeType i = 0; i < oldCap; ++i)
            {
                if (oldSlots[i].psl != EMPTY_PSL)
                    HInsertSlotMove(std::move(oldSlots[i].key), std::move(oldSlots[i].value));
            }

            // Free old buffer (elements already moved out)
            mAlloc.Deallocate(oldSlots, oldCap);
        }

        static SizeType HNextPow2(SizeType n) noexcept
        {
            if (n <= INITIAL_CAP)
                return INITIAL_CAP;
            --n;
            n |= n >> 1;
            n |= n >> 2;
            n |= n >> 4;
            n |= n >> 8;
            n |= n >> 16;
            n |= n >> 32;
            return n + 1;
        }

    private:
        Slot *mSlots = nullptr;
        SizeType mSize = 0;
        SizeType mCapacity = 0;
        Hasher mHasher = {};
        KeyEqual mEqual = {};
        SlotAlloc mAlloc = {};

        /**
         * @brief
         * Tiny helper flag used by Insert() to report whether it was a new key
         * @note
         * ```text
         * - Stored as a member to avoid an extra struct return on hot paths
         * ```
         */
        bool mLastInsertWasNew = false;
    };
}