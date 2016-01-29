// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iaMemBlock.h>

#include <iaConsole.h>

namespace IgorAux
{

    iaMemBlock::~iaMemBlock()
    {
        clearData();
    }

    void iaMemBlock::setData(const char* buffer, uint32 size)
    {
        clearData();

        if (buffer != nullptr)
        {
            con_assert(size != 0, "inconsistent input");

            if (size > 0)
            {
                _data = new char[size];
                _dataSize = size;
                memcpy(_data, buffer, size);
            }
            else
            {
                con_err("invalid input data");
            }
        }
    }

    void iaMemBlock::getData(char* buffer, uint32 size) const
    {
        con_assert(buffer != nullptr, "zero pointer");
        con_assert(_data != nullptr, "zero pointer");
        con_assert(_dataSize != 0, "no data");
        con_assert(size != 0, "invalid size");
        con_assert(size <= _dataSize, "size out of range");

        if (buffer != nullptr &&
            _data != nullptr &&
            _dataSize != 0 &&
            size != 0 &&
            size <= _dataSize)
        {
            memcpy(buffer, _data, size);
        }
        else
        {
            con_err("can't retrive data");
        }
    }

    char* iaMemBlock::getDataPointer() const
    {
        return _data;
    }

    void iaMemBlock::clearData()
    {
        if (_data != nullptr)
        {
            con_assert(_dataSize != 0, "inconsistant data");
            delete[] _data;
            _data = nullptr;
            _dataSize = 0;
        }
        else
        {
            con_assert(_dataSize == 0, "inconsistant data");
        }
    }

    bool iaMemBlock::hasData()
    {
        if (_data != nullptr && _dataSize != 0)
        {
            con_assert(_data != nullptr, "inconsistant data");
            con_assert(_dataSize != 0, "inconsistant data");
            return true;
        }
        else
        {
            con_assert(_data == nullptr, "inconsistant data");
            con_assert(_dataSize == 0, "inconsistant data");
            return false;
        }
    }

    uint32 iaMemBlock::getDataSize()
    {
        return _dataSize;
    }

}