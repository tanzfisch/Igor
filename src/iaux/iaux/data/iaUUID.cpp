// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/data/iaUUID.h>

#include <iaux/math/iaRandom.h>

#include <sstream>
#include <regex>

namespace iaux
{
    iaUUID::iaUUID()
    {
        // prevent zero
        while (_value == IGOR_INVALID_ID)
        {
            _value = iaRandom::getNext();
        }
    }

    iaUUID::iaUUID(const iaUUID &other)
        : _value(other._value)
    {
    }

    iaUUID::iaUUID(uint64 value)
    {
        _value = value;
    }

    iaUUID::iaUUID(const iaString &text)
    {
        if (isUUID(text))
        {
            if (text.getLength() == 18)
            {
                iaString number = text.getSubString(2, 16);
                _value = iaString::toUInt(number, 16);
            }
            else
            {
                _value = iaString::toUInt(text, 16);
            }
        }
        else
        {
            _value = text.getHashValue();
        }
    }

    bool iaUUID::isUUID(const iaString &text)
    {
        if(text.isEmpty())
        {
            return false;
        }

        std::wregex hexPattern(L"(0[xX][0-9a-fA-F]{1,16})|([0-9a-fA-F]{1,16})");        
        return std::regex_match(text.getData(), hexPattern);
    }

    iaUUID iaUUID::getInvalid()
    {
        return iaUUID(IGOR_INVALID_ID);
    }

    void iaUUID::reset()
    {
        _value = IGOR_INVALID_ID;
    }

    bool iaUUID::operator==(const iaUUID &other) const
    {
        return _value == other._value;
    }

    bool iaUUID::operator!=(const iaUUID &other)
    {
        return _value != other._value;
    }

    iaUUID iaUUID::operator=(const iaUUID &other)
    {
        _value = other._value;
        return *this;
    }

    iaUUID iaUUID::operator=(uint64 value)
    {
        _value = value;
        return *this;
    }

    const iaString iaUUID::toString() const
    {
        std::wstringstream stream;
        stream << "0x" << std::setfill(L'0') << std::setw(16) << std::hex << _value << std::dec;
        return iaString(stream.str().c_str());
    }

    iaUUID::operator uint64() const
    {
        return _value;
    }

    std::wostream &operator<<(std::wostream &stream, const iaUUID &uuid)
    {
        stream << uuid.toString();
        return stream;
    }

    std::wostream &operator<<(std::wostream &stream, const std::vector<iaUUID> &uuids)
    {
        bool first = true;
        for(const auto &uuid : uuids)
        {
            if(!first)
            {
                stream << ",";
            }
            stream << uuid.toString();

            first = false;
        }
        return stream;
    }

    bool iaUUID::isValid() const
    {
        return _value != IGOR_INVALID_ID;
    }
}