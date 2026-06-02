/**
 * @file Engine/Math/Vector2D.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

// Forward declare
using Float32 = float;

namespace Kryos
{
    namespace Math
    {
        struct Vector2D
        {
            Float32 X = 0.0f, Y = 0.0f;

            Vector2D() = default;
            Vector2D(Float32 x, Float32 y) : X(x), Y(y) {}
        };
    }
}