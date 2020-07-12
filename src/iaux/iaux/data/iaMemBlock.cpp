// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <iaux/data/iaMemBlock.h>

#include <iaux/system/iaConsole.h>

namespace iaux
{
    iaMemBlock::iaMemBlock()
    {
    }

    iaMemBlock::~iaMemBlock()
    {
        clearData();
    }

    void iaMemBlock::setData(const char *buffer, uint64 size)
    {
        clearData();

        if (buffer != nullptr)
        {
            con_assert(size != 0, "invalid input value");

            if (size > 0)
            {
                _data = new char[size];
                _dataSize = size;
                memcpy(_data, buffer, size);
            }
        }
    }

    void iaMemBlock::getData(char *buffer, uint64 size) const
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

    char *iaMemBlock::getDataPointer() const
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

    uint64 iaMemBlock::getDataSize()
    {
        return _dataSize;
    }

} // namespace iaux