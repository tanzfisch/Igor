// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/data/iaKeyFrameGraph.h>

namespace iaux
{
    
 /*   template <>
    void iaKeyFrameGraph<uint32>::getValue(float64 at, uint32 &value) const
    {
        if (at <= _values.front().first)
        {
            value = _values.front().second;
            return;
        }

        if (at >= _values.back().first)
        {
            value = _values.back().second;
            return;
        }

        for (int i = 0; i < _values.size(); ++i)
        {
            const float64 &right = _values[i].first;
            if (at > right)
            {
                continue;
            }

            value = _values[i].second;
            return;
        }
    }*/

}