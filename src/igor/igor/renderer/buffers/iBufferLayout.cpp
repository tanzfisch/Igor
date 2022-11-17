// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/buffers/iBufferLayout.h>

#include <igor/renderer/utils/iRendererUtils.h>

namespace igor
{

    iBufferLayout::iBufferLayout(const std::vector<iBufferLayoutEntry> &elements)
        : _elements(elements)
    {
        calcOffsetsAndStride();
    }

    uint32_t iBufferLayout::getStride() const
    {
        return _stride;
    }

    const std::vector<iBufferLayoutEntry> &iBufferLayout::getElements() const
    {
        return _elements;
    }

    void iBufferLayout::calcOffsetsAndStride()
    {
        size_t offset = 0;
        _stride = 0;
        for (auto &element : _elements)
        {
            element._offset = offset;
            offset += element._size;
            _stride += element._size;
        }
    }

    iBufferLayoutEntry::iBufferLayoutEntry(iShaderDataType type, bool normalized)
        : _type(type), _offset(0), _normalized(normalized), _size(iRendererUtils::getSize(_type))
    {
    }

    uint32_t iBufferLayoutEntry::getComponentCount() const
    {
        return iRendererUtils::getComponentCount(_type);
    }

}
