// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iActionDelegate.h>

namespace Igor
{
    iActionDelegate::iActionDelegate(iSimpleDelegate actionDelegate)
        : _actionDelegate(actionDelegate)
    {

    }

    void iActionDelegate::execute()
    {
        _actionDelegate();
    }
}
