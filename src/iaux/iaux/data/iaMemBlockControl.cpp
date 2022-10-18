// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/data/iaMemBlockControl.h>
#include <iaux/system/iaConsole.h>

#include <cstring>

namespace iaux
{

    void iaMemBlockControl::setBuffer(void *data, uint64 size)
    {
        _data = data;
        _size = size;
        _pos = 0;
    }

    void *iaMemBlockControl::getData()
    {
        return _data;
    }

    uint64 iaMemBlockControl::getSize()
    {
        return _size;
    }

    uint64 iaMemBlockControl::tell()
    {
        return _pos;
    }

    bool iaMemBlockControl::eof()
    {
        return _pos >= _size;
    }

    void iaMemBlockControl::seekBeg(uint64 pos)
    {
        if (pos <= _size)
        {
            _pos = pos;
        }
        else
        {
            con_err("out of range");
        }
    }

    void iaMemBlockControl::seekEnd(uint64 pos)
    {
        if (pos <= _size)
        {
            _pos = (_size - 1) - pos;
        }
        else
        {
            con_err("out of range");
        }
    }

    void iaMemBlockControl::seekCur(uint64 pos)
    {
        if (_pos + pos <= _size)
        {
            _pos += pos;
        }
        else
        {
            con_err("out of range");
        }
    }

    uint64 iaMemBlockControl::read(char *buffer, uint64 count)
    {
        if (_pos + count - 1 >= _size)
        {
            count = _size - _pos;
        }

        memcpy(buffer, static_cast<char *>(_data) + _pos, count);
        _pos += count;

        return count;
    }

    uint8 iaMemBlockControl::readUInt8()
    {
        uint8 result = *((uint8 *)(static_cast<char *>(_data) + _pos));
        _pos++;
        return result;
    }

    int8 iaMemBlockControl::readInt8()
    {
        int8 result = *((int8 *)(static_cast<char *>(_data) + _pos));
        _pos++;
        return result;
    }

    uint16 iaMemBlockControl::readUInt16()
    {
        uint16 result = *((uint16 *)(static_cast<char *>(_data) + _pos));
        _pos += 2;
        return result;
    }

    int16 iaMemBlockControl::readInt16()
    {
        int16 result = *((int16 *)(static_cast<char *>(_data) + _pos));
        _pos += 2;
        return result;
    }

    uint32 iaMemBlockControl::readUInt32()
    {
        uint32 result = *((uint32 *)(static_cast<char *>(_data) + _pos));
        _pos += 4;
        return result;
    }

    int32 iaMemBlockControl::readInt32()
    {
        int32 result = *((int32 *)(static_cast<char *>(_data) + _pos));
        _pos += 4;
        return result;
    }

    uint64 iaMemBlockControl::readUInt64()
    {
        uint64 result = *((uint64 *)(static_cast<char *>(_data) + _pos));
        _pos += 8;
        return result;
    }

    int64 iaMemBlockControl::readInt64()
    {
        int64 result = *((int64 *)(static_cast<char *>(_data) + _pos));
        _pos += 8;
        return result;
    }

    float32 iaMemBlockControl::readFloat32()
    {
        float32 result = *((float32 *)(static_cast<char *>(_data) + _pos));
        _pos += 4;
        return result;
    }

    float64 iaMemBlockControl::readFloat64()
    {
        float64 result = *((float64 *)(static_cast<char *>(_data) + _pos));
        _pos += 8;
        return result;
    }

} // namespace iaux
