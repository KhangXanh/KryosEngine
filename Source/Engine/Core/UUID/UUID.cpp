/**
 * @file Engine/Core/UUID/UUID.cpp
 * This is a Kryos Engine Implementation file
 */

#include "UUID.hpp"
#include <random>

namespace Kryos
{
    UUID::UUID()
    {
        static std::random_device sRandomDevice;
        static std::mt19937_64 sEngine(sRandomDevice());
        static std::uniform_int_distribution<UInt64> sUniformDistribution;

        mUUID = sUniformDistribution(sEngine);
    }

    UUID::UUID(UInt64 ID) : mUUID(ID) {}
}