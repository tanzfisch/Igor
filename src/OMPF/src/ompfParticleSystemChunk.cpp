// OMPF 3d model stream format
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header stream

#include <ompfParticleSystemChunk.h>

#include <iaConsole.h>
using namespace IgorAux;

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

    void ompfParticleSystemChunk::setTextureA(const iaString& filename)
    {
        _textureA = filename;
    }

    void ompfParticleSystemChunk::setTextureB(const iaString& filename)
    {
        _textureB = filename;
    }

    void ompfParticleSystemChunk::setTextureC(const iaString& filename)
    {
        _textureC = filename;
    }

    iaString ompfParticleSystemChunk::getTextureA() const
    {
        return _textureA;
    }

    iaString ompfParticleSystemChunk::getTextureB() const
    {
        return _textureB;
    }

    iaString ompfParticleSystemChunk::getTextureC() const
    {
        return _textureC;
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

	__IGOR_DISABLE_WARNING__(4100)
    uint32 ompfParticleSystemChunk::getSize(const ompfSettings& settings)
    {
        uint32 result = 0;
        result += 2; // max particle count
        result += 1; // loop
        result += static_cast<uint32>(_colorGradient.getValues().size()) * (sizeof(float32) * (1+4));
        result += static_cast<uint32>(_emissionGradient.getValues().size()) * (sizeof(float32) * (1+1));
        result += static_cast<uint32>(_sizeScaleGradient.getValues().size()) * (sizeof(float32) * 2);
        result += static_cast<uint32>(_orientationGradient.getValues().size()) * (sizeof(float32) * (1+2));
        result += static_cast<uint32>(_orientationRateGradient.getValues().size()) * (sizeof(float32) * (1+2));
        result += static_cast<uint32>(_liftGradient.getValues().size()) * (sizeof(float32) * 3);
        result += static_cast<uint32>(_velocityGradient.getValues().size()) * (sizeof(float32) * 3);
        result += static_cast<uint32>(_sizeGradient.getValues().size()) * (sizeof(float32) * 3);
        result += static_cast<uint32>(_startVisibleTimeGradient.getValues().size()) * (sizeof(float32) * 3);
        result += 4 + 4; //min max vortex torque
        result += 4 + 4; //min max vortex range
        result += 1; // vortex check range
        result += 4; // vortexToParticleRate
        result += 4; // vorticity confinement
        result += 2; // tiling column row
        result += 4; // air drag
        result += 4; // period time
        result += 1; // velocity oriented
        result += static_cast<uint32>(_textureA.getSize()) + 2;
        result += static_cast<uint32>(_textureB.getSize()) + 2;
        result += static_cast<uint32>(_textureC.getSize()) + 2;
        result += 4; // material chunk id
        result += 4; // emitter chunk id
        return result; // 24
    }
	__IGOR_ENABLE_WARNING__(4100)

    bool ompfParticleSystemChunk::write(std::ofstream& stream, const ompfSettings& settings)
    {
        if (!ompfBaseChunk::write(stream, settings))
        {
            return false;
        }

        con_debug_endl("---------------------------------------------------");
        con_debug_endl("write ompfParticleSystemChunk " << this->getName());

        if (!iaSerializable::writeUInt16(stream, _maxParticleCount))
        {
            return false;
        }
        con_debug_endl("max particle count " << _maxParticleCount);

        if (!iaSerializable::writeUInt8(stream, static_cast<uint8>(_loop ? 1 : 0)))
        {
            return false;
        }
        con_debug_endl("loop " << (_loop ? "true" : "false"));

        if (!iaSerializable::write(stream, _colorGradient))
        {
            return false;
        }
        con_debug_endl("color gradient entries=" << _colorGradient.getValues().size());

        if (!iaSerializable::write(stream, _emissionGradient))
        {
            return false;
        }
        con_debug_endl("emission gradient entries=" << _emissionGradient.getValues().size());

        if (!iaSerializable::write(stream, _sizeScaleGradient))
        {
            return false;
        }
        con_debug_endl("size scale gradient entries=" << _sizeScaleGradient.getValues().size());

        if (!iaSerializable::write(stream, _orientationGradient))
        {
            return false;
        }
        con_debug_endl("orientation gradient entries=" << _orientationGradient.getValues().size());

        if (!iaSerializable::write(stream, _orientationRateGradient))
        {
        return false;
        }
        con_debug_endl("orientation rate gradient entries=" << _orientationRateGradient.getValues().size());

        if (!iaSerializable::write(stream, _liftGradient))
        {
        return false;
        }
        con_debug_endl("lift gradient entries=" << _liftGradient.getValues().size());

        if (!iaSerializable::write(stream, _velocityGradient))
        {
        return false;
        }
        con_debug_endl("velocity gradient entries=" << _velocityGradient.getValues().size());

        if (!iaSerializable::write(stream, _sizeGradient))
        {
        return false;
        }
        con_debug_endl("size gradient entries=" << _sizeGradient.getValues().size());

        if (!iaSerializable::write(stream, _startVisibleTimeGradient))
        {
        return false;
        }
        con_debug_endl("start visibile gradient entries=" << _startVisibleTimeGradient.getValues().size());

        if (!iaSerializable::writeFloat32(stream, _minVortexTorque))
        {
            return false;
        }
        con_debug_endl("min vortex torque " << _minVortexTorque);

        if (!iaSerializable::writeFloat32(stream, _maxVortexTorque))
        {
            return false;
        }
        con_debug_endl("max vortex torque " << _maxVortexTorque);

        if (!iaSerializable::writeFloat32(stream, _minVortexRange))
        {
            return false;
        }
        con_debug_endl("min vortex range " << _minVortexRange);

        if (!iaSerializable::writeFloat32(stream, _maxVortexRange))
        {
            return false;
        }
        con_debug_endl("max vortex range " << _maxVortexRange);

        if (!iaSerializable::writeUInt8(stream, _vortexCheckRange))
        {
            return false;
        }
        con_debug_endl("vortex check range " << _vortexCheckRange);

        if (!iaSerializable::writeFloat32(stream, _vortexToParticleRate))
        {
            return false;
        }
        con_debug_endl("vortex to particle rate " << _vortexToParticleRate);

        if (!iaSerializable::writeFloat32(stream, _vorticityConfinement))
        {
            return false;
        }
        con_debug_endl("vorticity confinement " << _vorticityConfinement);

        if (!iaSerializable::writeUInt8(stream, _firstTectureTilingColumns))
        {
            return false;
        }
        con_debug_endl("first texture tiling columns " << _firstTectureTilingColumns);

        if (!iaSerializable::writeUInt8(stream, _firstTectureTilingRows))
        {
            return false;
        }
        con_debug_endl("first texture tiling rows " << _firstTectureTilingRows);

        if (!iaSerializable::writeFloat32(stream, _airDrag))
        {
            return false;
        }
        con_debug_endl("air drag " << _airDrag);

        if (!iaSerializable::writeFloat32(stream, _periodTime))
        {
            return false;
        }
        con_debug_endl("period " << _periodTime);

        if (!iaSerializable::writeUInt8(stream, static_cast<uint8>(_velocityOriented ? 1 : 0)))
        {
            return false;
        }
        con_debug_endl("velocity oriented " << (_velocityOriented ? "true" : "false"));

        if (!iaSerializable::writeUTF8(stream, _textureA))
        {
            return false;
        }
        con_debug_endl("texture A " << _textureA);

        if (!iaSerializable::writeUTF8(stream, _textureB))
        {
            return false;
        }
        con_debug_endl("texture B " << _textureB);

        if (!iaSerializable::writeUTF8(stream, _textureC))
        {
            return false;
        }
        con_debug_endl("texture C " << _textureC);

        if (!iaSerializable::writeUInt32(stream, _materialChunkID))
        {
            return false;
        }
        con_debug_endl("material chunk id " << _materialChunkID);

        if (!iaSerializable::writeUInt32(stream, _emitterID))
        {
            return false;
        }
        con_debug_endl("emitter chunk id " << _emitterID);

        return true;
    }
    
    bool ompfParticleSystemChunk::read(std::ifstream& stream, ompfSettings& settings)
    {
        con_debug_endl("---------------------------------------------------");
        con_debug_endl("read ompfParticleSystemChunk " << this->getName());

        if (!ompfBaseChunk::read(stream, settings))
        {
            return false;
        }

        if (!iaSerializable::readUInt16(stream, _maxParticleCount))
        {
            return false;
        }
        con_debug_endl("max particle count " << _maxParticleCount);

        uint8 typeValue;
        if (!iaSerializable::readUInt8(stream, typeValue))
        {
            return false;
        }
        _loop = typeValue == 0 ? false : true;
        con_debug_endl("loop " << (_loop ? "true" : "false"));

        if (!iaSerializable::read(stream, _colorGradient))
        {
            return false;
        }
        con_debug_endl("color gradient entries=" << _colorGradient.getValues().size());

        if (!iaSerializable::read(stream, _emissionGradient))
        {
            return false;
        }
        con_debug_endl("emission gradient entries=" << _emissionGradient.getValues().size());

        if (!iaSerializable::read(stream, _sizeScaleGradient))
        {
            return false;
        }
        con_debug_endl("size scale gradient entries=" << _sizeScaleGradient.getValues().size());

        if (!iaSerializable::read(stream, _orientationGradient))
        {
            return false;
        }
        con_debug_endl("orientation gradient entries=" << _orientationGradient.getValues().size());

        if (!iaSerializable::read(stream, _orientationRateGradient))
        {
        return false;
        }
        con_debug_endl("orientation rate gradient entries=" << _orientationRateGradient.getValues().size());

        if (!iaSerializable::read(stream, _liftGradient))
        {
        return false;
        }
        con_debug_endl("lift gradient entries=" << _liftGradient.getValues().size());

        if (!iaSerializable::read(stream, _velocityGradient))
        {
        return false;
        }
        con_debug_endl("velocity gradient entries=" << _velocityGradient.getValues().size());

        if (!iaSerializable::read(stream, _sizeGradient))
        {
        return false;
        }
        con_debug_endl("size gradient entries=" << _sizeGradient.getValues().size());

        if (!iaSerializable::read(stream, _startVisibleTimeGradient))
        {
        return false;
        }
        con_debug_endl("start visibile gradient entries=" << _startVisibleTimeGradient.getValues().size());

        if (!iaSerializable::readFloat32(stream, _minVortexTorque))
        {
            return false;
        }
        con_debug_endl("min vortex torque " << _minVortexTorque);

        if (!iaSerializable::readFloat32(stream, _maxVortexTorque))
        {
            return false;
        }
        con_debug_endl("max vortex torque " << _maxVortexTorque);

        if (!iaSerializable::readFloat32(stream, _minVortexRange))
        {
            return false;
        }
        con_debug_endl("min vortex range " << _minVortexRange);

        if (!iaSerializable::readFloat32(stream, _maxVortexRange))
        {
            return false;
        }
        con_debug_endl("max vortex range " << _maxVortexRange);

        if (!iaSerializable::readUInt8(stream, _vortexCheckRange))
        {
            return false;
        }
        con_debug_endl("vortex check range " << _vortexCheckRange);

        if (!iaSerializable::readFloat32(stream, _vortexToParticleRate))
        {
            return false;
        }
        con_debug_endl("vortex to particle rate " << _vortexToParticleRate);

        if (!iaSerializable::readFloat32(stream, _vorticityConfinement))
        {
            return false;
        }
        con_debug_endl("vorticity confinement " << _vorticityConfinement);

        if (!iaSerializable::readUInt8(stream, _firstTectureTilingColumns))
        {
            return false;
        }
        con_debug_endl("first texture tiling columns " << _firstTectureTilingColumns);

        if (!iaSerializable::readUInt8(stream, _firstTectureTilingRows))
        {
            return false;
        }
        con_debug_endl("first texture tiling rows " << _firstTectureTilingRows);

        if (!iaSerializable::readFloat32(stream, _airDrag))
        {
            return false;
        }
        con_debug_endl("air drag " << _airDrag);

        if (!iaSerializable::readFloat32(stream, _periodTime))
        {
            return false;
        }
        con_debug_endl("period " << _periodTime);

        uint8 velocityOriented;
        if (!iaSerializable::readUInt8(stream, velocityOriented))
        {
            return false;
        }
        _velocityOriented = velocityOriented == 0 ? false : true;
        con_debug_endl("velocity oriented " << (_velocityOriented ? "true" : "false"));

        if (!iaSerializable::readUTF8(stream, _textureA))
        {
            return false;
        }
        con_debug_endl("texture A " << _textureA);

        if (!iaSerializable::readUTF8(stream, _textureB))
        {
            return false;
        }
        con_debug_endl("texture B " << _textureB);

        if (!iaSerializable::readUTF8(stream, _textureC))
        {
            return false;
        }
        con_debug_endl("texture C " << _textureC);

        if (!iaSerializable::readUInt32(stream, _materialChunkID))
        {
            return false;
        }
        con_debug_endl("materialChunkID " << _materialChunkID);

        if (!iaSerializable::readUInt32(stream, _emitterID))
        {
            return false;
        }
        con_debug_endl("emitter chunk id " << _emitterID);

        return true;
    }
}

