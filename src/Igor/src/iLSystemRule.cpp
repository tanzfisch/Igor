// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iLSystemRule.h>

namespace Igor
{

    iLSystemRule::iLSystemRule(wchar_t in, iaString out)
    {
        _in = in;
        _out = out;
    }

    wchar_t iLSystemRule::getIn() const
    {
        return _in;
    }

    iaString iLSystemRule::getOut() const
    {
        return _out;
    }

}