// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/data/iMimeData.h>

namespace igor
{

    static const iaString s_mimeTypeTextPlain = "text/plain";
    static const iaString s_mimeTypeIgorWidgetID = "application/vnd.igor.widget-id";

    void iMimeData::setData(const iaString &mimeType, const uint8 *data, uint32 dataSize)
    {
        _data.emplace(std::make_pair(mimeType, iMimeDataBuffer(data, dataSize)));
    }

    void iMimeData::getData(const iaString &mimeType, uint8 **data, uint32 &dataSize) const
    {
        auto iter = _data.find(mimeType);
        if (iter == _data.end())
        {
            con_err("no data found for mime type \"" << mimeType << "\"");
            return;
        }

        *data = iter->second._data;
        dataSize = iter->second._dataSize;
    }

    bool iMimeData::hasType(const iaString &mimeType) const
    {
        auto iter = _data.find(mimeType);
        return iter != _data.end();
    }

    void iMimeData::setText(const iaString &text)
    {
        const uint8 *data = reinterpret_cast<const uint8*>(text.getData());
        setData(s_mimeTypeTextPlain, data, text.getSize());
    }

    const iaString iMimeData::getText() const
    {
        uint8 *data = nullptr;
        uint32 dataSize = 0;
        getData(s_mimeTypeTextPlain, &data, dataSize);

        if(data == nullptr)
        {
            return iaString();
        }

        return iaString(reinterpret_cast<wchar_t *>(data));
    }

    bool iMimeData::hasText() const
    {
        return _data.find(s_mimeTypeTextPlain) != _data.end();
    }

    void iMimeData::setWidgetID(iWidgetID widgetID)
    {
        const uint8 *data = reinterpret_cast<const uint8*>(&widgetID);
        setData(s_mimeTypeIgorWidgetID, data, sizeof(iWidgetID));        
    }

    iWidgetID iMimeData::getWidgetID() const
    {
        uint8 *data = nullptr;
        uint32 dataSize = 0;
        getData(s_mimeTypeIgorWidgetID, &data, dataSize);

        if(data == nullptr)
        {
            return iWidget::INVALID_WIDGET_ID;
        }

        return *reinterpret_cast<iWidgetID*>(data);
    }

    bool iMimeData::hasWidgetID() const
    {
        return _data.find(s_mimeTypeIgorWidgetID) != _data.end();
    }
}
