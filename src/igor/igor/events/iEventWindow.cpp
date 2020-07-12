// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/events/iEventWindow.h>

#include <sstream>

namespace igor
{

    iEventWindowClose::iEventWindowClose(iWindow *window)
        : iEvent(window)
    {
    }

    iEventWindowOpen::iEventWindowOpen(iWindow *window)
        : iEvent(window)
    {
    }

    iEventWindowResize::iEventWindowResize(iWindow *window, int32 width, int32 height)
        : iEvent(window), _width(width), _height(height)
    {
    }

    const iaString iEventWindowResize::getInfo() const
    {
        std::wstringstream stream;

        stream << getName() << "[" << _width << ", " << _height << "]";

        return stream.str().c_str();
    }

    int32 iEventWindowResize::getWidth() const
    {
        return _width;
    }

    int32 iEventWindowResize::getHeight() const
    {
        return _height;
    }

} // namespace igor