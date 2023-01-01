// OMPF 3d model file format
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <ompf/chunks/ompfResourceSearchPathChunk.h>

namespace OMPF
{

    ompfResourceSearchPathChunk::ompfResourceSearchPathChunk()
        : ompfBaseChunk(OMPFChunkType::ResourceSearchPath)
    {
    }

    void ompfResourceSearchPathChunk::setPath(iaString path)
    {
        _path = path;
    }

    iaString ompfResourceSearchPathChunk::getPath() const
    {
        return _path;
    }

    uint32 ompfResourceSearchPathChunk::getSize(const ompfSettings &settings)
    {
        return static_cast<uint32>(_path.getUTF8Size() + 2) + static_cast<uint32>(ompfBaseChunk::getSize(settings));
    }

    void ompfResourceSearchPathChunk::setPathType(OMPFPathType pathType)
    {
        _pathType = pathType;
    }

    OMPFPathType ompfResourceSearchPathChunk::getPathType() const
    {
        return _pathType;
    }

    bool ompfResourceSearchPathChunk::write(std::ofstream &file, const ompfSettings &settings)
    {
        con_assert(_pathType != OMPFPathType::Undefined, "path type can't be undefined");

        if (!ompfBaseChunk::write(file, settings))
        {
            return false;
        }

        con_trace("search path: " << _path << " type: " << static_cast<uint8>(_pathType));

        uint8 value = static_cast<uint8>(_pathType);
        if (!iaSerializable::writeUInt8(file, value))
        {
            return false;
        }

        if (!iaSerializable::writeUTF8(file, _path))
        {
            return false;
        }

        return true;
    }

    bool ompfResourceSearchPathChunk::read(std::ifstream &file, ompfSettings &settings)
    {
        if (!ompfBaseChunk::read(file, settings))
        {
            return false;
        }

        uint8 value = 0;
        if (!iaSerializable::readUInt8(file, value))
        {
            return false;
        }
        _pathType = static_cast<OMPFPathType>(value);

        if (!iaSerializable::readUTF8(file, _path))
        {
            return false;
        }

        con_trace("search path: " << _path << " type: " << static_cast<uint8>(_pathType));

        return true;
    }

} // namespace OMPF
