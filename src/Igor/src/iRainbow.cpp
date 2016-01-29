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

	iRainbow::iRainbow()
	{
	}

	iRainbow::~iRainbow()
	{
	}

	void iRainbow::clear()
	{
		colors.clear();
	}

	__IGOR_INLINE__ bool smaller(const RainbowColor &a, const RainbowColor &b)
	{
		return (a.position < b.position);
	}

    void iRainbow::insertColor(const iaColor4f& color, float32 at)
	{
        con_assert(0 <= at && 1 >= at, "invalid value");

		RainbowColor temp;

		temp.color = color;
		temp.position = at;
		colors.push_back(temp);

		sort(colors.begin(),colors.end(),smaller);
	}

	__IGOR_INLINE__ float32 normalize(float32 from, float32 to, float32 t)
	{
		return (t-from) / (to-from);
	}

    void iRainbow::getColor(float32 at, iaColor4f& color)
	{
        color.set(1, 1, 1, 1);

		for(uint32 i=1;i<colors.size();++i)
		{
			if(colors[i].position > at)
			{
				at = normalize(colors[i-1].position, colors[i].position, at);
                iaColor4f a = colors[i - 1].color;
                a *= (1.0f - at);
                iaColor4f b = colors[i].color;
                b *= at;
                color = a;
                color += b;;
			}
		}
	}

};
