// OMPF 3d model file format
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <ompfParticleSystemChunk.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <iomanip>
using namespace std;

namespace OMPF
{
    ompfParticleSystemChunk::ompfParticleSystemChunk()
        : ompfBaseChunk(OMPFChunkType::ParticleSystem)
    {
    }

    uint32 ompfParticleSystemChunk::getSize(const ompfSettings& settings)
    {
        return 0;
    }

    bool ompfParticleSystemChunk::write(ofstream& file, const ompfSettings& settings)
    {
        if (!ompfBaseChunk::write(file, settings))
        {
            return false;
        }

  /*      con_debug_endl("---------------------------------------------------");
        con_debug_endl("write ompfParticleSystemChunk " << this->getName());

        if (!iaSerializable::writeFloat32(file, _size))
        {
            return false;
        }
        con_debug_endl("size " << _size);

        if (!iaSerializable::writeUInt8(file, static_cast<uint8>(_type)))
        {
            return false;
        }
        con_debug_endl("type " << static_cast<uint8>(_type));*/

        return true;
    }
    
    bool ompfParticleSystemChunk::read(ifstream& file, ompfSettings& settings)
    {
        if (!ompfBaseChunk::read(file, settings))
        {
            return false;
        }

  /*      if (!iaSerializable::readFloat32(file, _size))
        {
            return false;
        }
        con_debug_endl("size " << _size);

        uint8 typeValue;
        if (!iaSerializable::readUInt8(file, typeValue))
        {
            return false;
        }
        _type = static_cast<OMPFEmitterType>(typeValue);
        con_debug_endl("type " << static_cast<uint8>(_type));*/

        return true;
    }
}

