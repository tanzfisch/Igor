// OMPF 3d model file format
// (c) Copyright 2012-2020 by Martin Loga
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

    __IGOR_DISABLE_WARNING__(4100)
    uint32 ompfEmitterChunk::getSize(const ompfSettings &settings)
    {
        return 4 + 1; // size + type
    }
    __IGOR_ENABLE_WARNING__(4100)

    bool ompfEmitterChunk::write(std::ofstream &file, const ompfSettings &settings)
    {
        if (!ompfBaseChunk::write(file, settings))
        {
            return false;
        }

        con_debug("---------------------------------------------------");
        con_debug("write ompfEmitterChunk " << this->getName());

        if (!iaSerializable::writeFloat32(file, _size))
        {
            return false;
        }
        con_debug("size " << _size);

        if (!iaSerializable::writeUInt8(file, static_cast<uint8>(_type)))
        {
            return false;
        }
        con_debug("type " << static_cast<uint8>(_type));

        return true;
    }

    bool ompfEmitterChunk::read(std::ifstream &file, ompfSettings &settings)
    {
        if (!ompfBaseChunk::read(file, settings))
        {
            return false;
        }

        con_debug("---------------------------------------------------");
        con_debug("read ompfEmitterChunk " << this->getName());

        if (!iaSerializable::readFloat32(file, _size))
        {
            return false;
        }
        con_debug("size " << _size);

        uint8 typeValue;
        if (!iaSerializable::readUInt8(file, typeValue))
        {
            return false;
        }
        _type = static_cast<OMPFEmitterType>(typeValue);
        con_debug("type " << static_cast<uint8>(_type));

        return true;
    }
} // namespace OMPF
