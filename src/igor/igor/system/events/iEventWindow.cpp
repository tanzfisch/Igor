// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/system/events/iEventWindow.h>

namespace igor
{

    iEventKindMask iWindowCloseEvent_TMP::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Window;
    }

    iWindowResizeEvent_TMP::iWindowResizeEvent_TMP(int32 width, int32 height)
        : _width(width), _height(height)
    {
    }

    iEventKindMask iWindowResizeEvent_TMP::getEventKindMask() const
    {
        return (iEventKindMask)iEventKind::Window;
    }

    int32 iWindowResizeEvent_TMP::getWidth() const
    {
        return _width;
    }

    int32 iWindowResizeEvent_TMP::getHeight() const
    {
        return _height;
    }

} // namespace igor