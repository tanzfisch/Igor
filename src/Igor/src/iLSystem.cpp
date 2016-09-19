// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iLSystem.h>

namespace Igor
{

    void iLSystem::addRule(wchar_t in, iaString out)
    {
        _rules[in] = out;
    }

    iaString iLSystem::generate(iaString in, int iterations)
    {
        iaString sentence = in;
        iaString next;

        for (int i = 0; i < iterations; ++i)
        {
            next.clear();

            for (int c = 0; c < sentence.getSize(); ++c)
            {
                next += _rules[sentence[c]];
            }

            sentence = next;
        }

        return sentence;
    }
}