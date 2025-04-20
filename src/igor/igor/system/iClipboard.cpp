// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/system/iClipboard.h>

#include <igor/system/iDefinesLinux.h>
#include <igor/threading/iThread.h>

#include <codecvt>

#include <clip.h>

namespace igor
{

    static clip::format s_entityIDFormat = clip::register_format("com.igor.EntityID");

    iClipboard::iClipboard()
    {
    }

    static std::string wstringToUtf8(const std::wstring &wstr)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(wstr);
    }

    static std::wstring utf8ToWstring(const std::string &str)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(str);
    }

    void iClipboard::copyText(const iaString &text)
    {
        clip::set_text(wstringToUtf8(text.getData()));
    }

    const iaString iClipboard::pasteText() const
    {
        std::string value;
        clip::get_text(value);
        return iaString(utf8ToWstring(value).c_str());
    }

    void iClipboard::copyEntityIDs(const std::vector<iEntityID> &entityIDs)
    {
        iaString data;

        for(const auto &entityID : entityIDs)
        {
            data += entityID.toString();
            data += ",";
        }

        clip::lock l;
        l.clear();
        l.set_data(s_entityIDFormat, (const char*)data.getData(), data.getSize());
    }

    const std::vector<iEntityID> iClipboard::pasteEntityIDs() const
    {
        con_assert(has(iClipboardDataFormat::EntityID), "invalid type");

        clip::lock l;
        const auto dataSize = l.get_data_length(s_entityIDFormat);
        std::vector<char> text(dataSize);
        if(!l.get_data(s_entityIDFormat, text.data(), dataSize))
        {
            con_err("failed to read data from clipboard");
        }

        iaString data((wchar_t*)text.data());

        std::vector<iaString> tokens;
        data.split(",", tokens);

        std::vector<iEntityID> result;

        for(const auto &token : tokens)
        {
            result.push_back(iEntityID(token));
        }

        return result;
    }

    bool iClipboard::has(iClipboardDataFormat format) const
    {
        switch(format)
        {
            case iClipboardDataFormat::Text:
            return clip::has(clip::text_format());

            case iClipboardDataFormat::EntityID:
            return clip::has(s_entityIDFormat);
        };

        con_warn("unsupported format");
        return false;
    }

} // namespace igor
