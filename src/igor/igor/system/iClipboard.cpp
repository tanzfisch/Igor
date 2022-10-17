// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/system/iClipboard.h>

namespace igor
{

    iClipboardFormat iClipboard::getFormat() const
    {
        return _format;
    }

    bool iClipboard::hasData() const
    {
        return _format != iClipboardFormat::Empty;
    }

    void iClipboard::clear()
    {
        _data.reset();
        _format = iClipboardFormat::Empty;
    }

    void iClipboard::setData(iClipboardFormat format, const std::any &data)
    {
        _format = format;
        _data = data;
    }

    const std::any &iClipboard::getData() const
    {
        return _data;
    }

} // namespace igor