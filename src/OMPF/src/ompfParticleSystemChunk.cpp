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
        uint32 result = 0;
        result += 2; // max particle count
        result += 1; // loop
        return result;
    }

    void ompfParticleSystemChunk::setMaxParticleCount(uint16 max)
    {
        _maxParticleCount = max;
    }

    uint16 ompfParticleSystemChunk::getMaxParticleCount() const
    {
        return _maxParticleCount;
    }

    void ompfParticleSystemChunk::setLoop(bool loop)
    {
        _loop = loop;
    }

    bool ompfParticleSystemChunk::getLoop() const
    {
        return _loop;
    }

    void ompfParticleSystemChunk::setColorGradient(const iaGradientColor4f& colorGradient)
    {
        _colorGradient = colorGradient;
    }

    void ompfParticleSystemChunk::getColorGradient(iaGradientColor4f& colorGradient) const
    {
        colorGradient = _colorGradient;
    }

    bool ompfParticleSystemChunk::write(ofstream& file, const ompfSettings& settings)
    {
        if (!ompfBaseChunk::write(file, settings))
        {
            return false;
        }

        con_debug_endl("---------------------------------------------------");
        con_debug_endl("write ompfParticleSystemChunk " << this->getName());

        if (!iaSerializable::writeUInt16(file, _maxParticleCount))
        {
            return false;
        }
        con_debug_endl("max particle count " << _maxParticleCount);

        if (!iaSerializable::writeUInt8(file, static_cast<uint8>(_loop ? 1 : 0)))
        {
            return false;
        }
        con_debug_endl("loop " << (_loop ? "true" : "false"));

        if (!iaSerializable::write(file, _colorGradient))
        {
            return false;
        }
        con_debug_endl("color gradient entries=" << _colorGradient.getValues().size());

        return true;
    }
    
    bool ompfParticleSystemChunk::read(ifstream& file, ompfSettings& settings)
    {
        if (!ompfBaseChunk::read(file, settings))
        {
            return false;
        }

        if (!iaSerializable::readUInt16(file, _maxParticleCount))
        {
            return false;
        }
        con_debug_endl("max particle count " << _maxParticleCount);

        uint8 typeValue;
        if (!iaSerializable::readUInt8(file, typeValue))
        {
            return false;
        }
        _loop = typeValue == 0 ? false : true;
        con_debug_endl("loop " << (_loop ? "true" : "false"));

        if (!iaSerializable::read(file, _colorGradient))
        {
            return false;
        }
        con_debug_endl("color gradient entries=" << _colorGradient.getValues().size());

        return true;
    }
}

