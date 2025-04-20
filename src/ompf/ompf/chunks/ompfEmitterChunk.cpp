// OMPF 3d model file format
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <ompf/chunks/ompfEmitterChunk.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <iomanip>

namespace OMPF
{
    ompfEmitterChunk::ompfEmitterChunk()
        : ompfBaseChunk(OMPFChunkType::Emitter)
    {
    }

    void ompfEmitterChunk::setSize(float32 size)
    {
        _size = size;
    }

    const float32 ompfEmitterChunk::getSize() const
    {
        return _size;
    }

    void ompfEmitterChunk::setType(OMPFEmitterType emitterType)
    {
        _type = emitterType;
    }

    OMPFEmitterType ompfEmitterChunk::getType() const
    {
        return _type;
    }

    IGOR_DISABLE_WARNING(4100)
    uint32 ompfEmitterChunk::getSize(const ompfSettings &settings)
    {
        return 4 + 1; // size + type
    }
    IGOR_ENABLE_WARNING(4100)

    bool ompfEmitterChunk::write(std::ofstream &file, const ompfSettings &settings)
    {
        if (!ompfBaseChunk::write(file, settings))
        {
            return false;
        }

        con_trace("---------------------------------------------------");
        con_trace("write ompfEmitterChunk " << this->getName());

        if (!iaSerializable::writeFloat32(file, _size))
        {
            return false;
        }
        con_trace("size " << _size);

        if (!iaSerializable::writeUInt8(file, static_cast<uint8>(_type)))
        {
            return false;
        }
        con_trace("type " << static_cast<uint8>(_type));

        return true;
    }

    bool ompfEmitterChunk::read(std::ifstream &file, ompfSettings &settings)
    {
        if (!ompfBaseChunk::read(file, settings))
        {
            return false;
        }

        con_trace("---------------------------------------------------");
        con_trace("read ompfEmitterChunk " << this->getName());

        if (!iaSerializable::readFloat32(file, _size))
        {
            return false;
        }
        con_trace("size " << _size);

        uint8 typeValue;
        if (!iaSerializable::readUInt8(file, typeValue))
        {
            return false;
        }
        _type = static_cast<OMPFEmitterType>(typeValue);
        con_trace("type " << static_cast<uint8>(_type));

        return true;
    }
} // namespace OMPF
