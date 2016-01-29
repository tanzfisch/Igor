// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file


__IGOR_INLINE__ float64 iPerlinNoise::lerp(float64 a, float64 b, float64 x)
{
    return a + x * (b - a);
}

__IGOR_INLINE__ int64 iPerlinNoise::inc(int64 num)
{
    num++;
    num %= iPerlinNoise::RANDOM_NUMBERS_COUNT;

    return num;
}

__IGOR_INLINE__ float64 iPerlinNoise::fade(float64 t)
{
    // Fade function as defined by Ken Perlin.  This eases coordinate values
    // so that they will ease towards integral values.  This ends up smoothing
    // the final output.
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);         // 6t^5 - 15t^4 + 10t^3
}

__IGOR_INLINE__ float64 iPerlinNoise::grad(int64 hash, float64 x, float64 y, float64 z)
{
    int64 hashValue = hash & 0xF;

    if (hashValue < 0x8)
    {
        if (hashValue < 0x4)
        {
            if (hashValue < 0x2)
            {
                if (hashValue == 0x0)
                {
                    return  x + y;
                }
                else
                {
                    return -x + y;
                }
            }
            else
            {
                if (hashValue == 0x2)
                {
                    return  x - y;
                }
                else
                {
                    return -x - y;
                }
            }
        }
        else
        {
            if (hashValue < 0x6)
            {
                if (hashValue == 0x4)
                {
                    return  x + z;
                }
                else
                {
                    return -x + z;
                }
            }
            else
            {
                if (hashValue == 0x6)
                {
                    return  x - z;
                }
                else
                {
                    return -x - z;
                }
            }
        }
    }
    else
    {
        if (hashValue < 0xC)
        {
            if (hashValue < 0xA)
            {
                if (hashValue == 0x8)
                {
                    return  y + z;
                }
                else
                {
                    return -y + z;
                }
            }
            else
            {
                if (hashValue == 0xA)
                {
                    return  y - z;
                }
                else
                {
                    return -y - z;
                }
            }
        }
        else
        {
            if (hashValue < 0xE)
            {
                if (hashValue == 0xC)
                {
                    return  y + x;
                }
                else
                {
                    return -y + z;
                }
            }
            else
            {
                if (hashValue == 0xE)
                {
                    return  y - x;
                }
                else
                {
                    return -y - z;
                }
            }
        }
    }
}
