/**
 * @file Engine/Core/UUID/UUID.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Core/Core.hpp"

namespace Kryos
{
    /**
     * @brief
     * Universally Unique Identifier
     * @note
     * ```text
     * - This version is using 64-bit number
     * - There are 2^64 (~= 18.44Q) possible combinations
     * ```
     */
    class UUID
    {
    public:
        UUID();
        UUID(UInt64 ID);
        UUID(const UUID &) = default;

        operator UInt64() const { return mUUID; }

    private:
        UInt64 mUUID;
    };
}