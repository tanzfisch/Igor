// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/actions/iActionSimpleDelegate.h>

namespace igor
{
    iActionSimpleDelegate::iActionSimpleDelegate(iSimpleDelegate actionDelegate)
        : _actionDelegate(actionDelegate)
    {
    }

    void iActionSimpleDelegate::execute()
    {
        _actionDelegate();
    }
} // namespace igor
