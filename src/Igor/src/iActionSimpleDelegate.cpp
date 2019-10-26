// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iActionSimpleDelegate.h>

namespace Igor
{
    iActionSimpleDelegate::iActionSimpleDelegate(iSimpleDelegate actionDelegate)
        : _actionDelegate(actionDelegate)
    {

    }

    void iActionSimpleDelegate::execute()
    {
        _actionDelegate();
    }
}
