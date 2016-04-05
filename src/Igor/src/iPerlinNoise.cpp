// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iPerlinNoise.h>

#include <stdlib.h>

namespace Igor
{

    iPerlinNoise::iPerlinNoise()
    {
        generateBase(1337);
    }

    void iPerlinNoise::generateBase(uint32 seed)
    {
        srand(seed);

        for (int64 i = 0; i < RANDOM_NUMBERS_COUNT; ++i)
        {
            p[i] = i;
        }

        for (int64 i = 0; i < 5000; ++i)
        {
            int64 src = rand() % RANDOM_NUMBERS_COUNT;
            int64 dst = rand() % RANDOM_NUMBERS_COUNT;

            int64 value = p[dst];
            p[dst] = p[src];
            p[src] = value;
        }

        for (int64 i = 0; i < RANDOM_NUMBERS_COUNT; ++i)
        {
            p[i + RANDOM_NUMBERS_COUNT] = p[i];
        }
    }

    float64 iPerlinNoise::getValue(const float64 pos, int64 octaves, float64 persistence)
    {
        float64 total = 0;
        float64 frequency = 1;
        float64 amplitude = 1;
        float64 maxValue = 0;  // Used for normalizing result to 0.0 - 1.0

        for (int64 i = 0; i<octaves; i++)
        {
            float64 temp = pos;
            temp *= frequency;

            total += getValue(temp) * amplitude;

            maxValue += amplitude;

            amplitude *= persistence;
            frequency *= 2;
        }

        return total / maxValue;
    }

    float64 iPerlinNoise::getValue(const iaVector3d& pos, int64 octaves, float64 persistence)
    {
        float64 total = 0;
        float64 frequency = 1;
        float64 amplitude = 1;
        float64 maxValue = 0;  // Used for normalizing result to 0.0 - 1.0

        for (int64 i = 0; i<octaves; i++) 
        {
            iaVector3d temp = pos;
            temp *= frequency;

            total += getValue(temp) * amplitude;

            maxValue += amplitude;

            amplitude *= persistence;
            frequency *= 2;
        }

        return total / maxValue;
    }

    float64 iPerlinNoise::getValue(const float64 pos)
    {
        float64 x = pos;

        x = fmod(x, RANDOM_NUMBERS_COUNT);
        int64 xi = static_cast<int64>(x) & 255;
        float64 xf = x - static_cast<int64>(x);
        float64 u = fade(xf);

        return lerp(p[xi], p[inc(xi)], u) / 255.0;
    }

    float64 iPerlinNoise::getValue(const iaVector3d& pos)
    {
        float64 intpart;
        float64 x = pos._x;
        float64 y = pos._y;
        float64 z = pos._z;

        x = fmod(x, RANDOM_NUMBERS_COUNT);
        y = fmod(y, RANDOM_NUMBERS_COUNT);
        z = fmod(z, RANDOM_NUMBERS_COUNT);

        int64 xi = static_cast<int64>(x) & 255;  // Calculate the "unit cube" that the point asked will be located in
        int64 yi = static_cast<int64>(y) & 255;  // The left bound is ( |_x_|,|_y_|,|_z_| ) and the right bound is that
        int64 zi = static_cast<int64>(z) & 255;  // plus 1.  Next we calculate the location (from 0.0 to 1.0) in that cube.

        float64 xf = x - static_cast<int64>(x);
        float64 yf = y - static_cast<int64>(y);
        float64 zf = z - static_cast<int64>(z);

        float64 u = fade(xf);
        float64 v = fade(yf);
        float64 w = fade(zf);

        int64 aaa, aba, aab, abb, baa, bba, bab, bbb;
        aaa = p[p[p[xi] + yi] + zi];
        aba = p[p[p[xi] + inc(yi)] + zi];
        aab = p[p[p[xi] + yi] + inc(zi)];
        abb = p[p[p[xi] + inc(yi)] + inc(zi)];
        baa = p[p[p[inc(xi)] + yi] + zi];
        bba = p[p[p[inc(xi)] + inc(yi)] + zi];
        bab = p[p[p[inc(xi)] + yi] + inc(zi)];
        bbb = p[p[p[inc(xi)] + inc(yi)] + inc(zi)];

        // The gradient function calculates the dot product between a pseudorandom
        // gradient vector and the vector from the input coordinate to the 8
        // surrounding points in its unit cube.
        // This is all then lerped together as a sort of weighted average based on the faded (u,v,w)
        // values we made earlier.

        float64 x1, x2, y1, y2;
        x1 = lerp(grad(aaa, xf, yf, zf), grad(baa, xf - 1, yf, zf), u);
        x2 = lerp(grad(aba, xf, yf - 1, zf), grad(bba, xf - 1, yf - 1, zf), u);
        y1 = lerp(x1, x2, v);

        x1 = lerp(grad(aab, xf, yf, zf - 1), grad(bab, xf - 1, yf, zf - 1), u);
        x2 = lerp(grad(abb, xf, yf - 1, zf - 1), grad(bbb, xf - 1, yf - 1, zf - 1), u);
        y2 = lerp(x1, x2, v);

        return (lerp(y1, y2, w) + 1.0) / 2.0;                      // For convenience we bind the result to 0 - 1 (theoretical min/max before is [-1, 1])
    }
}