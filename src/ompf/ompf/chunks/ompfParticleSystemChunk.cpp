// OMPF 3d model stream format
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header stream

#include <ompf/chunks/ompfParticleSystemChunk.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <iomanip>

namespace OMPF
{
    ompfParticleSystemChunk::ompfParticleSystemChunk()
        : ompfBaseChunk(OMPFChunkType::ParticleSystem)
    {
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

    bool ompfParticleSystemChunk::isLooped() const
    {
        return _loop;
    }

    void ompfParticleSystemChunk::setTextureTiling(uint8 columns, uint8 rows)
    {
        _firstTextureTilingColumns = columns;
        _firstTextureTilingRows = rows;
    }

    uint8 ompfParticleSystemChunk::getTextureColumns() const
    {
        return _firstTextureTilingColumns;
    }

    uint8 ompfParticleSystemChunk::getTextureRows() const
    {
        return _firstTextureTilingRows;
    }

    void ompfParticleSystemChunk::setStartAgeGradient(const iaKeyFrameGraphVector2f &visibleGradient)
    {
        _startVisibleTimeGradient = visibleGradient;
    }

    void ompfParticleSystemChunk::getStartAgeGradient(iaKeyFrameGraphVector2f &visibleGradient) const
    {
        visibleGradient = _startVisibleTimeGradient;
    }

    void ompfParticleSystemChunk::setSizeScaleGradient(const iaKeyFrameGraphf &sizeScaleGradient)
    {
        _sizeScaleGradient = sizeScaleGradient;
    }

    void ompfParticleSystemChunk::getSizeScaleGradient(iaKeyFrameGraphf &sizeScaleGradient) const
    {
        sizeScaleGradient = _sizeScaleGradient;
    }

    void ompfParticleSystemChunk::setStartSizeGradient(const iaKeyFrameGraphVector2f &sizeGradient)
    {
        _sizeGradient = sizeGradient;
    }

    void ompfParticleSystemChunk::getStartSizeGradient(iaKeyFrameGraphVector2f &sizeGradient) const
    {
        sizeGradient = _sizeGradient;
    }

    void ompfParticleSystemChunk::setStartVelocityGradient(const iaKeyFrameGraphVector2f &velocityGradient)
    {
        _velocityGradient = velocityGradient;
    }

    void ompfParticleSystemChunk::getStartVelocityGradient(iaKeyFrameGraphVector2f &velocityGradient) const
    {
        velocityGradient = _velocityGradient;
    }

    void ompfParticleSystemChunk::setStartLiftGradient(const iaKeyFrameGraphVector2f &liftGradient)
    {
        _liftGradient = liftGradient;
    }

    void ompfParticleSystemChunk::getStartLiftGradient(iaKeyFrameGraphVector2f &liftGradient) const
    {
        liftGradient = _liftGradient;
    }

    void ompfParticleSystemChunk::setStartOrientationGradient(const iaKeyFrameGraphVector2f &orientationGradient)
    {
        _orientationGradient = orientationGradient;
    }

    void ompfParticleSystemChunk::getStartOrientationGradient(iaKeyFrameGraphVector2f &orientationGradient) const
    {
        orientationGradient = _orientationGradient;
    }

    void ompfParticleSystemChunk::setStartOrientationRateGradient(const iaKeyFrameGraphVector2f &orientationRateGradient)
    {
        _orientationRateGradient = orientationRateGradient;
    }

    void ompfParticleSystemChunk::getStartOrientationRateGradient(iaKeyFrameGraphVector2f &orientationRateGradient) const
    {
        orientationRateGradient = _orientationRateGradient;
    }

    void ompfParticleSystemChunk::setColorGradient(const iaKeyFrameGraphColor4f &colorGradient)
    {
        _colorGradient = colorGradient;
    }

    void ompfParticleSystemChunk::getColorGradient(iaKeyFrameGraphColor4f &colorGradient) const
    {
        colorGradient = _colorGradient;
    }

    void ompfParticleSystemChunk::setEmissionGradient(const iaKeyFrameGraphf &emissionGradient)
    {
        _emissionGradient = emissionGradient;
    }

    void ompfParticleSystemChunk::getEmissionGradient(iaKeyFrameGraphf &emissionGradient) const
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

    void ompfParticleSystemChunk::setVortexToParticleRate(float32 rate)
    {
        _vortexToParticleRate = rate;
    }

    float32 ompfParticleSystemChunk::getVortexToParticleRate() const
    {
        return _vortexToParticleRate;
    }

    void ompfParticleSystemChunk::setVorticityConfinement(float32 vorticityConfinement)
    {
        _vorticityConfinement = vorticityConfinement;
    }

    float32 ompfParticleSystemChunk::getVorticityConfinement()
    {
        return _vorticityConfinement;
    }

    void ompfParticleSystemChunk::setAirDrag(float32 airDrag)
    {
        _airDrag = airDrag;
    }

    float32 ompfParticleSystemChunk::getAirDrag() const
    {
        return _airDrag;
    }

    void ompfParticleSystemChunk::setPeriodTime(float32 periodTime)
    {
        _periodTime = periodTime;
    }

    float32 ompfParticleSystemChunk::getPeriodTime() const
    {
        return _periodTime;
    }

    void ompfParticleSystemChunk::setVelocityOriented(bool velocityOriented)
    {
        _velocityOriented = velocityOriented;
    }

    bool ompfParticleSystemChunk::getVelocityOriented() const
    {
        return _velocityOriented;
    }

    void ompfParticleSystemChunk::setMaterialChunkID(uint32 id)
    {
        _materialChunkID = id;
    }

    uint32 ompfParticleSystemChunk::getMaterialChunkID() const
    {
        return _materialChunkID;
    }

    void ompfParticleSystemChunk::setEmitterChunkID(uint32 emitterID)
    {
        _emitterID = emitterID;
    }

    uint32 ompfParticleSystemChunk::getEmitterChunkID() const
    {
        return _emitterID;
    }

    IGOR_DISABLE_WARNING(4100)
    uint32 ompfParticleSystemChunk::getSize(const ompfSettings &settings)
    {
        uint32 result = 0;
        result += 2; // max particle count
        result += 1; // loop
        result += static_cast<uint32>(_colorGradient.getValues().size()) * (sizeof(float32) * (1 + 4));
        result += static_cast<uint32>(_emissionGradient.getValues().size()) * (sizeof(float32) * (1 + 1));
        result += static_cast<uint32>(_sizeScaleGradient.getValues().size()) * (sizeof(float32) * 2);
        result += static_cast<uint32>(_orientationGradient.getValues().size()) * (sizeof(float32) * (1 + 2));
        result += static_cast<uint32>(_orientationRateGradient.getValues().size()) * (sizeof(float32) * (1 + 2));
        result += static_cast<uint32>(_liftGradient.getValues().size()) * (sizeof(float32) * 3);
        result += static_cast<uint32>(_velocityGradient.getValues().size()) * (sizeof(float32) * 3);
        result += static_cast<uint32>(_sizeGradient.getValues().size()) * (sizeof(float32) * 3);
        result += static_cast<uint32>(_startVisibleTimeGradient.getValues().size()) * (sizeof(float32) * 3);
        result += 4 + 4; //min max vortex torque
        result += 4 + 4; //min max vortex range
        result += 1;     // vortex check range
        result += 4;     // vortexToParticleRate
        result += 4;     // vorticity confinement
        result += 2;     // tiling column row
        result += 4;     // air drag
        result += 4;     // period time
        result += 1;     // velocity oriented
        result += 4;   // material chunk id
        result += 4;   // emitter chunk id
        return result; // 24
    }
    IGOR_ENABLE_WARNING(4100)

    bool ompfParticleSystemChunk::write(std::ofstream &stream, const ompfSettings &settings)
    {
        if (!ompfBaseChunk::write(stream, settings))
        {
            return false;
        }

        con_trace("---------------------------------------------------");
        con_trace("write ompfParticleSystemChunk " << this->getName());

        if (!iaSerializable::writeUInt16(stream, _maxParticleCount))
        {
            return false;
        }
        con_trace("max particle count " << _maxParticleCount);

        if (!iaSerializable::writeUInt8(stream, static_cast<uint8>(_loop ? 1 : 0)))
        {
            return false;
        }
        con_trace("loop " << (_loop ? "true" : "false"));

        if (!iaSerializable::write(stream, _colorGradient))
        {
            return false;
        }
        con_trace("color gradient entries=" << _colorGradient.getValues().size());

        if (!iaSerializable::write(stream, _emissionGradient))
        {
            return false;
        }
        con_trace("emission gradient entries=" << _emissionGradient.getValues().size());

        if (!iaSerializable::write(stream, _sizeScaleGradient))
        {
            return false;
        }
        con_trace("size scale gradient entries=" << _sizeScaleGradient.getValues().size());

        if (!iaSerializable::write(stream, _orientationGradient))
        {
            return false;
        }
        con_trace("orientation gradient entries=" << _orientationGradient.getValues().size());

        if (!iaSerializable::write(stream, _orientationRateGradient))
        {
            return false;
        }
        con_trace("orientation rate gradient entries=" << _orientationRateGradient.getValues().size());

        if (!iaSerializable::write(stream, _liftGradient))
        {
            return false;
        }
        con_trace("lift gradient entries=" << _liftGradient.getValues().size());

        if (!iaSerializable::write(stream, _velocityGradient))
        {
            return false;
        }
        con_trace("velocity gradient entries=" << _velocityGradient.getValues().size());

        if (!iaSerializable::write(stream, _sizeGradient))
        {
            return false;
        }
        con_trace("size gradient entries=" << _sizeGradient.getValues().size());

        if (!iaSerializable::write(stream, _startVisibleTimeGradient))
        {
            return false;
        }
        con_trace("start visibile gradient entries=" << _startVisibleTimeGradient.getValues().size());

        if (!iaSerializable::writeFloat32(stream, _minVortexTorque))
        {
            return false;
        }
        con_trace("min vortex torque " << _minVortexTorque);

        if (!iaSerializable::writeFloat32(stream, _maxVortexTorque))
        {
            return false;
        }
        con_trace("max vortex torque " << _maxVortexTorque);

        if (!iaSerializable::writeFloat32(stream, _minVortexRange))
        {
            return false;
        }
        con_trace("min vortex range " << _minVortexRange);

        if (!iaSerializable::writeFloat32(stream, _maxVortexRange))
        {
            return false;
        }
        con_trace("max vortex range " << _maxVortexRange);

        if (!iaSerializable::writeUInt8(stream, _vortexCheckRange))
        {
            return false;
        }
        con_trace("vortex check range " << _vortexCheckRange);

        if (!iaSerializable::writeFloat32(stream, _vortexToParticleRate))
        {
            return false;
        }
        con_trace("vortex to particle rate " << _vortexToParticleRate);

        if (!iaSerializable::writeFloat32(stream, _vorticityConfinement))
        {
            return false;
        }
        con_trace("vorticity confinement " << _vorticityConfinement);

        if (!iaSerializable::writeUInt8(stream, _firstTextureTilingColumns))
        {
            return false;
        }
        con_trace("first texture tiling columns " << _firstTextureTilingColumns);

        if (!iaSerializable::writeUInt8(stream, _firstTextureTilingRows))
        {
            return false;
        }
        con_trace("first texture tiling rows " << _firstTextureTilingRows);

        if (!iaSerializable::writeFloat32(stream, _airDrag))
        {
            return false;
        }
        con_trace("air drag " << _airDrag);

        if (!iaSerializable::writeFloat32(stream, _periodTime))
        {
            return false;
        }
        con_trace("period " << _periodTime);

        if (!iaSerializable::writeUInt8(stream, static_cast<uint8>(_velocityOriented ? 1 : 0)))
        {
            return false;
        }
        con_trace("velocity oriented " << (_velocityOriented ? "true" : "false"));

        if (!iaSerializable::writeUInt32(stream, _materialChunkID))
        {
            return false;
        }
        con_trace("material chunk id " << _materialChunkID);

        if (!iaSerializable::writeUInt32(stream, _emitterID))
        {
            return false;
        }
        con_trace("emitter chunk id " << _emitterID);

        return true;
    }

    bool ompfParticleSystemChunk::read(std::ifstream &stream, ompfSettings &settings)
    {
        con_trace("---------------------------------------------------");
        con_trace("read ompfParticleSystemChunk " << this->getName());

        if (!ompfBaseChunk::read(stream, settings))
        {
            return false;
        }

        if (!iaSerializable::readUInt16(stream, _maxParticleCount))
        {
            return false;
        }
        con_trace("max particle count " << _maxParticleCount);

        uint8 typeValue;
        if (!iaSerializable::readUInt8(stream, typeValue))
        {
            return false;
        }
        _loop = typeValue == 0 ? false : true;
        con_trace("loop " << (_loop ? "true" : "false"));

        if (!iaSerializable::read(stream, _colorGradient))
        {
            return false;
        }
        con_trace("color gradient entries=" << _colorGradient.getValues().size());

        if (!iaSerializable::read(stream, _emissionGradient))
        {
            return false;
        }
        con_trace("emission gradient entries=" << _emissionGradient.getValues().size());

        if (!iaSerializable::read(stream, _sizeScaleGradient))
        {
            return false;
        }
        con_trace("size scale gradient entries=" << _sizeScaleGradient.getValues().size());

        if (!iaSerializable::read(stream, _orientationGradient))
        {
            return false;
        }
        con_trace("orientation gradient entries=" << _orientationGradient.getValues().size());

        if (!iaSerializable::read(stream, _orientationRateGradient))
        {
            return false;
        }
        con_trace("orientation rate gradient entries=" << _orientationRateGradient.getValues().size());

        if (!iaSerializable::read(stream, _liftGradient))
        {
            return false;
        }
        con_trace("lift gradient entries=" << _liftGradient.getValues().size());

        if (!iaSerializable::read(stream, _velocityGradient))
        {
            return false;
        }
        con_trace("velocity gradient entries=" << _velocityGradient.getValues().size());

        if (!iaSerializable::read(stream, _sizeGradient))
        {
            return false;
        }
        con_trace("size gradient entries=" << _sizeGradient.getValues().size());

        if (!iaSerializable::read(stream, _startVisibleTimeGradient))
        {
            return false;
        }
        con_trace("start visibile gradient entries=" << _startVisibleTimeGradient.getValues().size());

        if (!iaSerializable::readFloat32(stream, _minVortexTorque))
        {
            return false;
        }
        con_trace("min vortex torque " << _minVortexTorque);

        if (!iaSerializable::readFloat32(stream, _maxVortexTorque))
        {
            return false;
        }
        con_trace("max vortex torque " << _maxVortexTorque);

        if (!iaSerializable::readFloat32(stream, _minVortexRange))
        {
            return false;
        }
        con_trace("min vortex range " << _minVortexRange);

        if (!iaSerializable::readFloat32(stream, _maxVortexRange))
        {
            return false;
        }
        con_trace("max vortex range " << _maxVortexRange);

        if (!iaSerializable::readUInt8(stream, _vortexCheckRange))
        {
            return false;
        }
        con_trace("vortex check range " << _vortexCheckRange);

        if (!iaSerializable::readFloat32(stream, _vortexToParticleRate))
        {
            return false;
        }
        con_trace("vortex to particle rate " << _vortexToParticleRate);

        if (!iaSerializable::readFloat32(stream, _vorticityConfinement))
        {
            return false;
        }
        con_trace("vorticity confinement " << _vorticityConfinement);

        if (!iaSerializable::readUInt8(stream, _firstTextureTilingColumns))
        {
            return false;
        }
        con_trace("first texture tiling columns " << _firstTextureTilingColumns);

        if (!iaSerializable::readUInt8(stream, _firstTextureTilingRows))
        {
            return false;
        }
        con_trace("first texture tiling rows " << _firstTextureTilingRows);

        if (!iaSerializable::readFloat32(stream, _airDrag))
        {
            return false;
        }
        con_trace("air drag " << _airDrag);

        if (!iaSerializable::readFloat32(stream, _periodTime))
        {
            return false;
        }
        con_trace("period " << _periodTime);

        uint8 velocityOriented;
        if (!iaSerializable::readUInt8(stream, velocityOriented))
        {
            return false;
        }
        _velocityOriented = velocityOriented == 0 ? false : true;
        con_trace("velocity oriented " << (_velocityOriented ? "true" : "false"));

        if (!iaSerializable::readUInt32(stream, _materialChunkID))
        {
            return false;
        }
        con_trace("materialChunkID " << _materialChunkID);

        if (!iaSerializable::readUInt32(stream, _emitterID))
        {
            return false;
        }
        con_trace("emitter chunk id " << _emitterID);

        return true;
    }
} // namespace OMPF
