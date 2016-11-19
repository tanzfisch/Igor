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

    void ompfParticleSystemChunk::setFirstTextureTiling(uint8 columns, uint8 rows)
    {
        _firstTectureTilingColumns = columns;
        _firstTectureTilingRows = rows;
    }

    uint8 ompfParticleSystemChunk::getFirstTextureColumns() const
    {
        return _firstTectureTilingColumns;
    }

    uint8 ompfParticleSystemChunk::getFirstTextureRows() const
    {
        return _firstTectureTilingRows;
    }

    void ompfParticleSystemChunk::setStartVisibleTimeGradient(const iaGradientVector2f& visibleGradient)
    {
        _startVisibleTimeGradient = visibleGradient;
    }

    void ompfParticleSystemChunk::getStartVisibleTimeGradient(iaGradientVector2f& visibleGradient) const
    {
        visibleGradient = _startVisibleTimeGradient;
    }

    void ompfParticleSystemChunk::setSizeScaleGradient(const iaGradientf& sizeScaleGradient)
    {
        _sizeScaleGradient = sizeScaleGradient;
    }

    void ompfParticleSystemChunk::getSizeScaleGradient(iaGradientf& sizeScaleGradient) const
    {
        sizeScaleGradient = _sizeScaleGradient;
    }

    void ompfParticleSystemChunk::setStartSizeGradient(const iaGradientVector2f& sizeGradient)
    {
        _sizeGradient = sizeGradient;
    }

    void ompfParticleSystemChunk::getStartSizeGradient(iaGradientVector2f& sizeGradient) const
    {
        sizeGradient = _sizeGradient;
    }

    void ompfParticleSystemChunk::setStartVelocityGradient(const iaGradientVector2f& velocityGradient)
    {
        _velocityGradient = velocityGradient;
    }

    void ompfParticleSystemChunk::getStartVelocityGradient(iaGradientVector2f& velocityGradient) const
    {
        velocityGradient = _velocityGradient;
    }

    void ompfParticleSystemChunk::setStartLiftGradient(const iaGradientVector2f& liftGradient)
    {
        _liftGradient = liftGradient;
    }

    void ompfParticleSystemChunk::getStartLiftGradient(iaGradientVector2f& liftGradient) const
    {
        liftGradient = _liftGradient;
    }

    void ompfParticleSystemChunk::setStartOrientationGradient(const iaGradientVector2f& orientationGradient)
    {
        _orientationGradient = orientationGradient;
    }

    void ompfParticleSystemChunk::getStartOrientationGradient(iaGradientVector2f& orientationGradient) const
    {
        orientationGradient = _orientationGradient;
    }

    void ompfParticleSystemChunk::setStartOrientationRateGradient(const iaGradientVector2f& orientationRateGradient)
    {
        _orientationRateGradient = orientationRateGradient;
    }

    void ompfParticleSystemChunk::getStartOrientationRateGradient(iaGradientVector2f& orientationRateGradient) const
    {
        orientationRateGradient = _orientationRateGradient;
    }

    void ompfParticleSystemChunk::setColorGradient(const iaGradientColor4f& colorGradient)
    {
        _colorGradient = colorGradient;
    }

    void ompfParticleSystemChunk::getColorGradient(iaGradientColor4f& colorGradient) const
    {
        colorGradient = _colorGradient;
    }

    void ompfParticleSystemChunk::setEmissionGradient(const iaGradientf& emissionGradient)
    {
        _emissionGradient = emissionGradient;
    }

    void ompfParticleSystemChunk::getEmissionGradient(iaGradientf& emissionGradient) const
    {
        emissionGradient = _emissionGradient;
    }

    void ompfParticleSystemChunk::setVortexTorque(float32 min, float32 max)
    {
        _minVortexTorque = min;
        _maxVortexTorque = max;
    }

    float32 ompfParticleSystemChunk::getVortexTorqueMin()
    {
        return _minVortexTorque;
    }

    float32 ompfParticleSystemChunk::getVortexTorqueMax()
    {
        return _maxVortexTorque;
    }

    void ompfParticleSystemChunk::setVortexRange(float32 min, float32 max)
    {
        _minVortexRange = min;
        _maxVortexRange = max;
    }

    float32 ompfParticleSystemChunk::getVortexRangeMin()
    {
        return _minVortexRange;
    }

    float32 ompfParticleSystemChunk::getVortexRangeMax()
    {
        return _maxVortexRange;
    }

    void ompfParticleSystemChunk::setVortexCheckRange(uint8 particles)
    {
        _vortexCheckRange = particles;
    }

    uint8 ompfParticleSystemChunk::getVortexCheckRange()
    {
        return _vortexCheckRange;
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

        if (!iaSerializable::write(file, _emissionGradient))
        {
            return false;
        }
        con_debug_endl("emission gradient entries=" << _emissionGradient.getValues().size());



        if (!iaSerializable::writeFloat32(file, _minVortexTorque))
        {
            return false;
        }
        con_debug_endl("min vortex torque " << _minVortexTorque);

        if (!iaSerializable::writeFloat32(file, _maxVortexTorque))
        {
            return false;
        }
        con_debug_endl("max vortex torque " << _maxVortexTorque);

        if (!iaSerializable::writeFloat32(file, _minVortexRange))
        {
            return false;
        }
        con_debug_endl("min vortex range " << _minVortexRange);

        if (!iaSerializable::writeFloat32(file, _maxVortexRange))
        {
            return false;
        }
        con_debug_endl("max vortex range " << _maxVortexRange);

        if (!iaSerializable::writeUInt8(file, _vortexCheckRange))
        {
            return false;
        }
        con_debug_endl("vortex check range " << _vortexCheckRange);

        if (!iaSerializable::writeUInt8(file, _firstTectureTilingColumns))
        {
            return false;
        }
        con_debug_endl("first texture tiling columns " << _firstTectureTilingColumns);

        if (!iaSerializable::writeUInt8(file, _firstTectureTilingRows))
        {
            return false;
        }
        con_debug_endl("first texture tiling rows " << _firstTectureTilingRows);

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

        if (!iaSerializable::read(file, _emissionGradient))
        {
            return false;
        }
        con_debug_endl("emission gradient entries=" << _emissionGradient.getValues().size());




        if (!iaSerializable::readFloat32(file, _minVortexTorque))
        {
            return false;
        }
        con_debug_endl("min vortex torque " << _minVortexTorque);

        if (!iaSerializable::readFloat32(file, _maxVortexTorque))
        {
            return false;
        }
        con_debug_endl("max vortex torque " << _maxVortexTorque);

        if (!iaSerializable::readFloat32(file, _minVortexRange))
        {
            return false;
        }
        con_debug_endl("min vortex range " << _minVortexRange);

        if (!iaSerializable::readFloat32(file, _maxVortexRange))
        {
            return false;
        }
        con_debug_endl("max vortex range " << _maxVortexRange);

        if (!iaSerializable::readUInt8(file, _vortexCheckRange))
        {
            return false;
        }
        con_debug_endl("vortex check range " << _vortexCheckRange);

        if (!iaSerializable::readUInt8(file, _firstTectureTilingColumns))
        {
            return false;
        }
        con_debug_endl("first texture tiling columns " << _firstTectureTilingColumns);

        if (!iaSerializable::readUInt8(file, _firstTectureTilingRows))
        {
            return false;
        }
        con_debug_endl("first texture tiling rows " << _firstTectureTilingRows);

        return true;
    }
}

