/**
 * @file Engine/Core/Container/Hashs/HashTable.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include <unordered_map>

/**
 * @brief
 * A custom hash table for Kryos Engine
 * @todo
 * ```text
 * - For now we are using std::unordered_map
 * - but we will make a custom hash table for Kryos Engine
 * ```
 */
template <typename T, typename U>
class THashTable : public std::unordered_map<T, U>
{
public:
    bool Contains(const T &key) const
    {
        return this->find(key) != this->end();
    }
};