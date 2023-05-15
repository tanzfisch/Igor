// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/simulation/iParticleSystem.h>
#include <iaux/system/iaConsole.h>

#include <igor/resources/material/iMaterialResourceFactory.h>
#include <iaux/data/iaGradient.h>
#include <igor/system/iTimer.h>
#include <igor/simulation/iParticleEmitter.h>

#include <iaux/data/iaConvert.h>
using namespace iaux;

#include <iostream>

namespace igor
{
    float32 iParticleSystem::_simulationRate = 60.0f;

    iParticleSystem::iParticleSystem()
    {
        _rand.setSeed(static_cast<uint32>(iaTime::getNow().getMicroseconds()));
        initDefaultGradients();
    }

    iParticleSystem::~iParticleSystem()
    {
    }

    const iaSphered &iParticleSystem::getBoundingSphere() const
    {
        return _boundingSphere;
    }

    const iAABoxd &iParticleSystem::getBoundingBox() const
    {
        return _boundingBox;
    }

    void iParticleSystem::initDefaultGradients()
    {
        _startAgeGradient.setValue(0.0f, iaVector2f(2.5f, 3.5f));

        _colorGradient.setValue(0.0f, iaColor4f(1.0f, 1.0f, 1.0f, 0.0f));
        _colorGradient.setValue(0.2f, iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
        _colorGradient.setValue(0.5f, iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
        _colorGradient.setValue(1.0f, iaColor4f(1.0f, 1.0f, 1.0f, 0.0f));

        _emissionRateGradient.setValue(0.0f, 20.0f / iParticleSystem::_simulationRate);

        _sizeScaleGradient.setValue(0.0f, 1.0f);

        _startSizeGradient.setValue(0.0f, iaVector2f(0.1f, 0.3f));

        _startVelocityGradient.setValue(0.0f, iaVector2f(0.01f, 0.02f));

        _startLiftGradient.setValue(0.0f, iaVector2f(0.0f, 0.0f));

        _startOrientationGradient.setValue(0.0f, iaVector2f(0.0f, 0.0f));

        _startOrientationRateGradient.setValue(0.0f, iaVector2f(0.0f, 0.0f));

        // internal gradient
        _torqueFactorGradient.setValue(0.0f, 0.0f);
        _torqueFactorGradient.setValue(0.1f, 1.0f);
        _torqueFactorGradient.setValue(0.9f, 1.0f);
        _torqueFactorGradient.setValue(1.0f, 0.0f);
    }

    void iParticleSystem::setVelocityOriented(bool velocityOriented)
    {
        _velocityOriented = velocityOriented;
    }

    bool iParticleSystem::getVelocityOriented() const
    {
        return _velocityOriented;
    }

    void iParticleSystem::setColorGradient(const iaGradientColor4f &colorGradient)
    {
        _colorGradient = colorGradient;
    }

    void iParticleSystem::getColorGradient(iaGradientColor4f &colorGradient) const
    {
        colorGradient = _colorGradient;
    }

    const iaGradientColor4f &iParticleSystem::getColorGradient() const
    {
        return _colorGradient;
    }

    void iParticleSystem::setEmissionGradient(const iaGradientf &emissionGradient)
    {
        _emissionRateGradient = emissionGradient;
    }

    void iParticleSystem::getEmissionGradient(iaGradientf &emissionGradient) const
    {
        emissionGradient = _emissionRateGradient;
    }

    void iParticleSystem::setTextureTiling(uint8 columns, uint8 rows)
    {
        con_assert(columns > 0 && rows > 0, "out of range");

        if (columns > 0 && rows > 0)
        {
            _firstTextureColumns = columns;
            _firstTextureRows = rows;
        }
    }

    uint8 iParticleSystem::getTextureColumns() const
    {
        return _firstTextureColumns;
    }

    uint8 iParticleSystem::getTextureRows() const
    {
        return _firstTextureRows;
    }

    void iParticleSystem::setSecondTextureRotation(float32 angle)
    {
        _octave1Rotation = angle;
    }

    void iParticleSystem::setThirdTextureRotation(float32 angle)
    {
        _octave2Rotation = angle;
    }

    float32 iParticleSystem::getSecondTextureRotation()
    {
        return _octave1Rotation;
    }

    float32 iParticleSystem::getThirdTextureRotation()
    {
        return _octave2Rotation;
    }

    float32 iParticleSystem::getVorticityConfinement()
    {
        return _vorticityConfinement;
    }

    float32 iParticleSystem::getVortexTorqueMin()
    {
        return _minVortexTorque;
    }

    float32 iParticleSystem::getVortexTorqueMax()
    {
        return _maxVortexTorque;
    }

    float32 iParticleSystem::getVortexRangeMin()
    {
        return _minVortexRange;
    }

    float32 iParticleSystem::getVortexRangeMax()
    {
        return _maxVortexRange;
    }

    uint8 iParticleSystem::getVortexCheckRange()
    {
        return _vortexCheckRange;
    }

    void iParticleSystem::setVortexCheckRange(uint8 checkRange)
    {
        _vortexCheckRange = checkRange;
    }

    void iParticleSystem::start()
    {
        _startTime = iTimer::getInstance().getTime();
        _playbackTime = _startTime;
        _running = true;
    }

    void iParticleSystem::stop()
    {
        _running = false;
    }

    bool iParticleSystem::isRunning()
    {
        return _running;
    }

    void iParticleSystem::reset()
    {
        _particlePool.resize(_maxParticleCount);
        _particlePoolIndex = _particlePool.size() - 1;

        _mustReset = false;
        _particleCounter = 0;

        start();
    }

    void iParticleSystem::setMaxParticleCount(uint16 max)
    {
        con_assert(max > 0, "invalid particle count");

        if (_maxParticleCount == max)
        {
            return;
        }

        _maxParticleCount = max;
        _createBuffers = true;
        _mustReset = true;
    }

    uint16 iParticleSystem::getMaxParticleCount() const
    {
        return _maxParticleCount;
    }

    void iParticleSystem::setPeriodTime(float32 periodTime)
    {
        _particleSystemPeriodTime = iaTime::fromSeconds(periodTime);
        _mustReset = true;
    }

    float32 iParticleSystem::getPeriodTime() const
    {
        return _particleSystemPeriodTime.getSeconds();
    }

    void iParticleSystem::setVortexTorque(float32 min, float32 max)
    {
        _minVortexTorque = min;
        _maxVortexTorque = max;
        _mustReset = true;
    }

    void iParticleSystem::setVortexRange(float32 min, float32 max)
    {
        _minVortexRange = min;
        _maxVortexRange = max;
        _mustReset = true;
    }

    void iParticleSystem::resetParticle(iParticle &particle, iParticleEmitter &emitter, float32 particleSystemTime)
    {
        iaVector2f minMax;
        iaVector3d position;
        iaVector3d velocity;
        emitter.calcRandomStart(position, velocity);

        position = _particleSystemInvWorldMatrix * position;

        velocity = _particleSystemInvWorldMatrix * velocity;
        velocity -= _particleSystemInvWorldMatrix._pos;

        float32 randomFactor = (_rand.getNext() % 1000 / 1000.0f);

        float32 vel = 0;
        _startVelocityGradient.getValue(particleSystemTime, minMax);
        vel = minMax._x + randomFactor * (minMax._y - minMax._x);
        velocity *= vel;

        particle._position.set(position._x, position._y, position._z);
        particle._velocity.set(velocity._x, velocity._y, velocity._z);

        _startLiftGradient.getValue(particleSystemTime, minMax);
        particle._lift = minMax._x + (1 - randomFactor) * (minMax._y - minMax._x);

        _startSizeGradient.getValue(particleSystemTime, minMax);
        particle._size = minMax._x + randomFactor * (minMax._y - minMax._x);

        _startAgeGradient.getValue(particleSystemTime, minMax);
        particle._maxAge = minMax._x + randomFactor * (minMax._y - minMax._x);
        particle._lifeLeft = particle._maxAge;
        particle._visible = true;

        _startOrientationGradient.getValue(particleSystemTime, minMax);
        particle._orientation = minMax._x + randomFactor * (minMax._y - minMax._x);

        _startOrientationRateGradient.getValue(particleSystemTime, minMax);
        particle._orientationRate = minMax._x + randomFactor * (minMax._y - minMax._x);

        if (_firstTextureColumns == 1 && _firstTextureRows == 1)
        {
            particle._tilingIndex = 0.0f;
        }
        else
        {
            particle._tilingIndex = static_cast<float32>(_rand.getNext() % (_firstTextureColumns * _firstTextureRows));
        }

        if (_vortexToParticleRate != 0.0 &&
            _particleCounter == static_cast<uint64>((1.0f - _vortexToParticleRate) * 100.0f))
        {
            particle._normal.set(_rand.getNext() % 100 / 100.0f - 0.5f, _rand.getNext() % 100 / 100.0f - 0.5f, _rand.getNext() % 100 / 100.0f - 0.5f);
            particle._normal.normalize();
            particle._torque = _minVortexTorque + (_rand.getNext() % 100 / 100.0f) * (_maxVortexTorque - _minVortexTorque);
            particle._vortexRange = _minVortexRange + (_rand.getNext() % 100 / 100.0f) * (_maxVortexRange - _minVortexRange);
            _particleCounter = 0;
        }
        else
        {
            particle._torque = 0.0;
            _particleCounter++;
        }
    }

    void iParticleSystem::setVortexToParticleRate(float32 rate)
    {
        _vortexToParticleRate = rate;
        _mustReset = true;
    }

    float32 iParticleSystem::getVortexToParticleRate() const
    {
        return _vortexToParticleRate;
    }

    void iParticleSystem::createParticles(uint32 particleCount, iParticleEmitter &emitter, float32 particleSystemTime)
    {
        for (uint32 i = 0; i < particleCount; ++i)
        {
            resetParticle(_particlePool[_particlePoolIndex], emitter, particleSystemTime);

            _particlePoolIndex--;
            _particlePoolIndex = (_particlePoolIndex + _particlePool.size()) % _particlePool.size();
        }
    }

    void iParticleSystem::setParticleSystemMatrix(const iaMatrixd &worldInvMatrix)
    {
        _particleSystemInvWorldMatrix = worldInvMatrix;
    }

    void iParticleSystem::setStartAgeGradient(const iaGradientVector2f &startAgeGradient)
    {
        // TODO check if max age can be 0 or less

        _startAgeGradient = startAgeGradient;
        _mustReset = true;
    }

    void iParticleSystem::getStartAgeGradient(iaGradientVector2f &startAgeGradient) const
    {
        startAgeGradient = _startAgeGradient;
    }

    void iParticleSystem::setSizeScaleGradient(const iaGradientf &sizeScaleGradient)
    {
        _sizeScaleGradient = sizeScaleGradient;
        _mustReset = true;
    }

    void iParticleSystem::getSizeScaleGradient(iaGradientf &sizeScaleGradient) const
    {
        sizeScaleGradient = _sizeScaleGradient;
    }

    bool iParticleSystem::isFinished() const
    {
        return _finished;
    }

    void iParticleSystem::setLoop(bool loop)
    {
        _loop = loop;
        _mustReset = true;
    }

    bool iParticleSystem::isLooped() const
    {
        return _loop;
    }

    void iParticleSystem::iterateFrame()
    {
        int32 index = 0;
        float32 sizeScale = 0;
        float32 torqueFactor = 0;

        for (int index = 0; index < _particlePool.size(); ++index)
        {
            auto &particle = _particlePool[index];
            if (particle._lifeLeft <= 0)
            {
                particle._visible = false;
                continue;
            }

            particle._tilingIndex += _tileIncrement;

            const float32 normalizedAge = particle._lifeLeft / particle._maxAge;

            _sizeScaleGradient.getValue(normalizedAge, sizeScale);

            particle._velocity[1] += particle._lift;
            particle._velocity *= _airDrag;
            particle._orientation += particle._orientationRate;
            particle._sizeScale = sizeScale;

            if (particle._torque != 0.0)
            {
                _torqueFactorGradient.getValue(normalizedAge, torqueFactor);

                const int32 startIndex = index - _vortexCheckRange;
                const int32 endIndex = index + _vortexCheckRange;

                for (int32 i = startIndex; i < endIndex; ++i)
                {
                    int32 torqueIndex = (i + _particlePool.size()) % _particlePool.size();

                    if (index == torqueIndex) // ignore your self
                    {
                        continue;
                    }

                    iaVector3f vortexAxis = particle._position - _particlePool[torqueIndex]._position;
                    if (vortexAxis.length() > particle._vortexRange)
                    {
                        continue;
                    }

                    iaVector3f vortexTangent = vortexAxis % particle._normal;
                    vortexTangent.normalize();
                    vortexTangent *= (particle._vortexRange - vortexAxis.length()) / particle._vortexRange;
                    vortexTangent *= particle._torque * torqueFactor;
                    vortexAxis.normalize();
                    vortexAxis *= _vorticityConfinement;
                    vortexTangent += vortexAxis;

                    _particlePool[torqueIndex]._velocity += vortexTangent * 0.001f; // TODO 0.001 ???
                }
            }

            particle._position += particle._velocity;
            particle._lifeLeft -= 1.0f / _simulationRate;
        }
    }

    void iParticleSystem::onUpdate(iParticleEmitter &emitter)
    {
        if (_mustReset)
        {
            reset();
        }

        if (_running)
        {
            const iaTime frameTime = iTimer::getInstance().getTime();
            const iaTime frameTick = iaTime::fromMilliseconds(1000.0 / _simulationRate);
            
            // ignore hickups
            if (frameTime - _playbackTime > iaTime::fromMilliseconds(100))
            {
                _playbackTime = frameTime;
            }

            iaTime particleSystemTime = _playbackTime - _startTime;

            while (_playbackTime <= frameTime)
            {
                iterateFrame();

                float32 emissionRate = 0.0f;
                _emissionRateGradient.getValue(particleSystemTime.getSeconds(), emissionRate);
                _emissionImpulseStack += emissionRate;
                int32 createCount = static_cast<int32>(_emissionImpulseStack);
                _emissionImpulseStack -= static_cast<float32>(createCount);
                createParticles(createCount, emitter, particleSystemTime.getSeconds());

                _playbackTime += frameTick;
                particleSystemTime += frameTick;
            }

            updateBuffer();
            updateBoundings();

            if (particleSystemTime >= _particleSystemPeriodTime)
            {
                if (_loop)
                {
                    start();
                }
                else
                {
                    _finished = true;
                    _running = false;
                }
            }
        }
    }

    void iParticleSystem::updateBuffer()
    {
        if (_createBuffers)
        {
            if (_vertexBufferData != nullptr)
            {
                delete[] _vertexBufferData;
            }

            _vertexBufferData = new iParticleSystem::iParticleVertex[_maxParticleCount * 4];

            _vertexBuffer = iVertexBuffer::create(_maxParticleCount * 4 * sizeof(iParticleVertex));
            _vertexBuffer->setLayout(
                std::vector<iBufferLayoutEntry>{
                    {iShaderDataType::Float3},
                    {iShaderDataType::Float3},
                    {iShaderDataType::Float4},
                    {iShaderDataType::Float4}});

            _vertexArray = iVertexArray::create();
            _vertexArray->addVertexBuffer(_vertexBuffer);

            _createBuffers = false;
        }

        // update buffer data
        iParticleVertex *vertexBufferDataPtr = _vertexBufferData;

        for (const auto &particle : _particlePool)
        {
            if (particle._lifeLeft <= 0.0)
            {
                continue;
            }

            const float32 normalizedAge = particle._lifeLeft / particle._maxAge;
            const float32 age = particle._maxAge - particle._lifeLeft;

            vertexBufferDataPtr->_position = particle._position;
            _colorGradient.getValue(normalizedAge, vertexBufferDataPtr->_color);
            vertexBufferDataPtr->_velocity = particle._velocity;
            vertexBufferDataPtr->_lifeSizeAngleTilingIndex.set(
                age,
                particle._size * particle._sizeScale,
                particle._orientation,
                particle._tilingIndex);

            vertexBufferDataPtr++;
        }

        uint32 dataSize = (uint32)((uint8 *)vertexBufferDataPtr - (uint8 *)_vertexBufferData);
        _vertexBuffer->setData(dataSize, _vertexBufferData);
    }

    iVertexArrayPtr iParticleSystem::getVertexArray() const
    {
        return _vertexArray;
    }

    void iParticleSystem::updateBoundings()
    {
        // TODO
        if (_particlePool.empty())
        {
            return;
        }

        iaVector3f minPos = _particlePool.front()._position;
        iaVector3f maxPos = _particlePool.front()._position;
        uint32 steps = std::min(_maxParticleCount / 20, 1);

        for (int i = 0; i < _particlePool.size(); i += steps)
        {
            if (_particlePool[i]._position._x < minPos._x)
            {
                minPos._x = _particlePool[i]._position._x;
            }

            if (_particlePool[i]._position._y < minPos._y)
            {
                minPos._y = _particlePool[i]._position._y;
            }

            if (_particlePool[i]._position._z < minPos._z)
            {
                minPos._z = _particlePool[i]._position._z;
            }

            if (_particlePool[i]._position._x > maxPos._x)
            {
                maxPos._x = _particlePool[i]._position._x;
            }

            if (_particlePool[i]._position._y > maxPos._y)
            {
                maxPos._y = _particlePool[i]._position._y;
            }
            if (_particlePool[i]._position._z > maxPos._z)
            {
                maxPos._z = _particlePool[i]._position._z;
            }
        }

        iaVector3d minPosd = minPos.convert<float64>();
        iaVector3d maxPosd = maxPos.convert<float64>();

        _boundingBox._center = minPosd;
        _boundingBox._center += maxPosd;
        _boundingBox._center *= 0.5;

        _boundingBox._halfWidths = maxPosd;
        _boundingBox._halfWidths -= minPosd;
        _boundingBox._halfWidths *= 0.5;

        _boundingSphere._center = _boundingBox._center;
        _boundingSphere._radius = std::max(_boundingBox._halfWidths._x, std::max(_boundingBox._halfWidths._y, _boundingBox._halfWidths._z));
    }

    float32 iParticleSystem::getSimulationRate()
    {
        return _simulationRate;
    }

    void iParticleSystem::setStartSizeGradient(const iaGradientVector2f &sizeGradient)
    {
        _startSizeGradient = sizeGradient;
        _mustReset = true;
    }

    void iParticleSystem::getStartSizeGradient(iaGradientVector2f &sizeGradient) const
    {
        sizeGradient = _startSizeGradient;
    }

    void iParticleSystem::setStartVelocityGradient(const iaGradientVector2f &velocityGradient)
    {
        _startVelocityGradient = velocityGradient;
        _mustReset = true;
    }

    void iParticleSystem::getStartVelocityGradient(iaGradientVector2f &velocityGradient) const
    {
        velocityGradient = _startVelocityGradient;
    }

    void iParticleSystem::setStartLiftGradient(const iaGradientVector2f &liftGradient)
    {
        _startLiftGradient = liftGradient;
        _mustReset = true;
    }

    void iParticleSystem::getStartLiftGradient(iaGradientVector2f &liftGradient) const
    {
        liftGradient = _startLiftGradient;
    }

    void iParticleSystem::setAirDrag(float32 airDrag)
    {
        _airDrag = airDrag;
        _mustReset = true;
    }

    float32 iParticleSystem::getAirDrag() const
    {
        return _airDrag;
    }

    void iParticleSystem::setStartOrientationGradient(const iaGradientVector2f &orientationGradient)
    {
        _startOrientationGradient = orientationGradient;
        _mustReset = true;
    }

    void iParticleSystem::getStartOrientationGradient(iaGradientVector2f &orientationGradient) const
    {
        orientationGradient = _startOrientationGradient;
    }

    void iParticleSystem::setStartOrientationRateGradient(const iaGradientVector2f &orientationRateGradient)
    {
        _startOrientationRateGradient = orientationRateGradient;
        _mustReset = true;
    }

    void iParticleSystem::getStartOrientationRateGradient(iaGradientVector2f &orientationRateGradient) const
    {
        orientationRateGradient = _startOrientationRateGradient;
    }

    void iParticleSystem::setVorticityConfinement(float32 vc)
    {
        _vorticityConfinement = vc;
        _mustReset = true;
    }

    void iParticleSystem::setTileIncrement(float32 tileIncrement)
    {
        _tileIncrement = tileIncrement;
    }

    float32 iParticleSystem::getTileIncrement() const
    {
        return _tileIncrement;
    }

}; // namespace igor
