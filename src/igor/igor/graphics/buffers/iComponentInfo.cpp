
// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/graphics/buffers/iComponentInfo.h>

namespace igor
{

    iComponentInfo::iComponentInfo(std::vector<iComponentEntry> components)
        : _components(components)
    {
        calcOffsetsAndStride();
    }

    uint32_t iComponentInfo::getStride() const
    {
        return _stride;
    }

    const std::vector<iComponentEntry> &iComponentInfo::getComponents() const
    {
        return _components;
    }

    void iComponentInfo::calcOffsetsAndStride()
    {
        size_t offset = 0;
        _stride = 0;
        for (auto &element : _components)
        {
            element._offset = offset;
            offset += element._size;
            _stride += element._size;
        }
    }

}
