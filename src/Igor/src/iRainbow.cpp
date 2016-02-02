// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iRainbow.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <algorithm>
using namespace std;

namespace Igor
{

	void iRainbow::clear()
	{
		colors.clear();
	}

	__IGOR_INLINE__ bool smaller(const RainbowColor &a, const RainbowColor &b)
	{
		return (a._position < b._position);
	}

    void iRainbow::insertColor(const iaColor4f& color, float32 at)
	{
        con_assert(0 <= at && 1 >= at, "invalid value");

        if (0 <= at && 1 >= at)
        {
            RainbowColor temp;

            temp._color = color;
            temp._position = at;
            colors.push_back(temp);

            sort(colors.begin(), colors.end(), smaller);
        }
	}

	__IGOR_INLINE__ float32 normalize(float32 from, float32 to, float32 t)
	{
		return (t-from) / (to-from);
	}

    void iRainbow::getColor(float32 at, iaColor4f& color)
	{
        con_assert(colors.size() > 1, "not enough data");

        if (colors.size() > 1)
        {
            color = colors[0]._color;

            for (uint32 i = 1; i < colors.size(); ++i)
            {
                if (colors[i]._position > at)
                {
                    float32 t = normalize(colors[i - 1]._position, colors[i]._position, at);
                    color = colors[i - 1]._color;
                    color *= (1.0f - t);
                    iaColor4f b = colors[i]._color;
                    b *= t;
                    color += b;

                    break;
                }
            }
        }
	}

};
