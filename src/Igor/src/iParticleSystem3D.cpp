// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iParticleSystem3D.h>
#include <iaConsole.h>
#include <iTextureResourceFactory.h>
#include <iMaterialResourceFactory.h>
#include <iaGradient.h>
#include <iTimer.h>
#include <iParticleEmitter.h>

#include <iaConvert.h>
using namespace IgorAux;

#include <iostream>


namespace Igor
{

    float32 iParticleSystem3D::_simulationRate = 60.0f;

    iParticleSystem3D::iParticleSystem3D()
    {
        _rand.setSeed(static_cast<uint32>(iTimer::getInstance().getApplicationTime()));
        initDefaultGradients();
    }

    iParticleSystem3D::~iParticleSystem3D()
    {
    }

    const iSphered& iParticleSystem3D::getBoundingSphere() const
    {
        return _boundingSphere;
    }

    const iAABoxd& iParticleSystem3D::getBoundingBox() const
    {
        return _boundingBox;
    }

    void iParticleSystem3D::initDefaultGradients()
    {
        _startVisibleTimeGradient.setValue(0.0f, iaVector2f(2.5f, 3.5f));

        _colorGradient.setValue(0.0f, iaColor4f(1.0f, 1.0f, 1.0f, 0.0f));
        _colorGradient.setValue(0.2f, iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
        _colorGradient.setValue(0.5f, iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
        _colorGradient.setValue(1.0f, iaColor4f(1.0f, 1.0f, 1.0f, 0.0f));

        _emissionRateGradient.setValue(0.0f, 20.0f / iParticleSystem3D::_simulationRate);

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

    void iParticleSystem3D::setVelocityOriented(bool velocityOriented)
    {
        _velocityOriented = velocityOriented;
    }

    bool iParticleSystem3D::getVelocityOriented() const
    {
        return _velocityOriented;
    }

    void iParticleSystem3D::setColorGradient(const iaGradientColor4f& colorGradient)
    {
        _colorGradient = colorGradient;
    }

    void iParticleSystem3D::getColorGradient(iaGradientColor4f& colorGradient) const
    {
        colorGradient = _colorGradient;
    }

    const iaGradientColor4f& iParticleSystem3D::getColorGradient() const
    {
        return _colorGradient;
    }

    void iParticleSystem3D::setEmissionGradient(const iaGradientf& emissionGradient)
    {
        _emissionRateGradient = emissionGradient;
    }

    void iParticleSystem3D::getEmissionGradient(iaGradientf& emissionGradient) const
    {
        emissionGradient = _emissionRateGradient;
    }

    void iParticleSystem3D::setFirstTextureTiling(uint8 columns, uint8 rows)
    {
        con_assert(columns > 0 && rows > 0, "out of range");

        if (columns > 0 && rows > 0)
        {
            _firstTextureColumns = columns;
            _firstTextureRows = rows;
        }
    }

    uint8 iParticleSystem3D::getFirstTextureColumns() const
    {
        return _firstTextureColumns;
    }

    uint8 iParticleSystem3D::getFirstTextureRows() const
    {
        return _firstTextureRows;
    }

    void iParticleSystem3D::setSecondTextureRotation(float32 angle)
    {
        _octave1Rotation = angle;
    }

    void iParticleSystem3D::setThirdTextureRotation(float32 angle)
    {
        _octave2Rotation = angle;
    }

    float32 iParticleSystem3D::getSecondTextureRotation()
    {
        return _octave1Rotation;
    }

    float32 iParticleSystem3D::getThirdTextureRotation()
    {
        return _octave2Rotation;
    }

    float32 iParticleSystem3D::getVorticityConfinement()
    {
        return _vorticityConfinement;
    }

    float32 iParticleSystem3D::getVortexTorqueMin()
    {
        return _minVortexTorque;
    }

    float32 iParticleSystem3D::getVortexTorqueMax()
    {
        return _maxVortexTorque;
    }

    float32 iParticleSystem3D::getVortexRangeMin()
    {
        return _minVortexRange;
    }

    float32 iParticleSystem3D::getVortexRangeMax()
    {
        return _maxVortexRange;
    }

    uint8 iParticleSystem3D::getVortexCheckRange()
    {
        return _vortexCheckRange;
    }

    void iParticleSystem3D::setVortexCheckRange(uint8 checkRange)
    {
        _vortexCheckRange = checkRange;
    }

    void iParticleSystem3D::start()
    {
        _startTime = iTimer::getInstance().getMilliSeconds();
        _playbackTime = _startTime;
        _running = true;
    }

    void iParticleSystem3D::stop()
    {
        _running = false;
    }

    bool iParticleSystem3D::isRunning()
    {
        return _running;
    }

    void iParticleSystem3D::reset()
    {
        if (!_particles.empty())
        {
            _particles.clear();
        }

        float32 maxVisibleTime = 0;
        for (auto value : _startVisibleTimeGradient.getValues())
        {
            if (value.second._y > maxVisibleTime)
            {
                maxVisibleTime = value.second._y;
            }
        }

        _lifeTime = maxVisibleTime;

        _mustReset = false;

        _particleCounter = 0;

        start();
    }

    void iParticleSystem3D::setMaxParticleCount(uint16 max)
    {
        _maxParticleCount = max;
    }

    uint16 iParticleSystem3D::getMaxParticleCount() const
    {
        return _maxParticleCount;
    }

    void iParticleSystem3D::setPeriodTime(float32 periodTime)
    {
        _particleSystemPeriodTime = periodTime * __IGOR_SECOND__;
        _mustReset = true;
    }

    float32 iParticleSystem3D::getPeriodTime() const
    {
        return _particleSystemPeriodTime / __IGOR_SECOND__;
    }

    void iParticleSystem3D::setVortexTorque(float32 min, float32 max)
    {
        _minVortexTorque = min;
        _maxVortexTorque = max;
        _mustReset = true;
    }

    void iParticleSystem3D::setVortexRange(float32 min, float32 max)
    {
        _minVortexRange = min;
        _maxVortexRange = max;
        _mustReset = true;
    }

    void iParticleSystem3D::resetParticle(iParticle &particle, iParticleEmitter& emitter, float32 particleSystemTime)
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

        particle._life = _lifeTime;

        particle._visibleTime = 0.0;

        _startVisibleTimeGradient.getValue(particleSystemTime, minMax);
        float32 visibleTime = minMax._x + randomFactor * (minMax._y - minMax._x);
        visibleTime /= 1.0f / _simulationRate;
        particle._visibleTimeIncrease = 1.0f / visibleTime;
        particle._visible = true;

        _startOrientationGradient.getValue(particleSystemTime, minMax);
        particle._orientation = minMax._x + randomFactor * (minMax._y - minMax._x);

        _startOrientationRateGradient.getValue(particleSystemTime, minMax);
        particle._orientationRate = minMax._x + randomFactor * (minMax._y - minMax._x);

        if (_firstTextureColumns == 1 && _firstTextureRows == 1)
        {
            particle._texturefrom.set(0, 0);
            particle._textureto.set(1, 1);
        }
        else
        {
            uint8 col = static_cast<uint8>(_rand.getNext()) % _firstTextureColumns;
            uint8 row = static_cast<uint8>(_rand.getNext()) % _firstTextureRows;

            float32 width = 1.0f / static_cast<float32>(_firstTextureColumns);
            float32 height = 1.0f / static_cast<float32>(_firstTextureRows);

            particle._texturefrom.set(col * static_cast<float32>(width), row * static_cast<float32>(height));
            particle._textureto._x = particle._texturefrom._x + width;
            particle._textureto._y = particle._texturefrom._y + width;
        }

        particle._phase0.set(_rand.getNext() % 100 / 100.0f, _rand.getNext() % 100 / 100.0f);
        particle._phase1.set(_rand.getNext() % 100 / 100.0f, _rand.getNext() % 100 / 100.0f);
    }

    void iParticleSystem3D::setVortexToParticleRate(float32 rate)
    {
        _vortexToParticleRate = rate;
        _mustReset = true;
    }

    float32 iParticleSystem3D::getVortexToParticleRate() const
    {
        return _vortexToParticleRate;
    }

    void iParticleSystem3D::createParticles(uint32 particleCount, iParticleEmitter& emitter, float32 particleSystemTime)
    {
        uint16 particlesToCreate = particleCount;
        if (_particles.size() + particlesToCreate > _maxParticleCount)
        {
            particlesToCreate = _maxParticleCount - static_cast<uint16>(_particles.size());
        }

        for (uint16 i = 0; i < particlesToCreate; ++i)
        {
            iParticle particle;

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

            resetParticle(particle, emitter, particleSystemTime);
            _particles.push_back(particle);
        }
    }

    const std::deque<iParticle>& iParticleSystem3D::getCurrentFrame() const
    {
        return _particles;
    }

    void iParticleSystem3D::setParticleSystemMatrix(const iaMatrixd& worldInvMatrix)
    {
        _particleSystemInvWorldMatrix = worldInvMatrix;
    }

    void iParticleSystem3D::setStartVisibleTimeGradient(const iaGradientVector2f& startVisibleTimeGradient)
    {
        _startVisibleTimeGradient = startVisibleTimeGradient;
        _mustReset = true;
    }

    void iParticleSystem3D::getStartVisibleTimeGradient(iaGradientVector2f& startVisibleTimeGradient) const
    {
        startVisibleTimeGradient = _startVisibleTimeGradient;
    }

    void iParticleSystem3D::setSizeScaleGradient(const iaGradientf& sizeScaleGradient)
    {
        _sizeScaleGradient = sizeScaleGradient;
        _mustReset = true;
    }

    void iParticleSystem3D::getSizeScaleGradient(iaGradientf& sizeScaleGradient) const
    {
        sizeScaleGradient = _sizeScaleGradient;
    }

    bool iParticleSystem3D::isFinished() const
    {
        return _finished;
    }

    void iParticleSystem3D::setLoop(bool loop)
    {
        _loop = loop;
        _mustReset = true;
    }

    bool iParticleSystem3D::getLoop() const
    {
        return _loop;
    }

    void iParticleSystem3D::calcNextFrame(iParticleEmitter& emitter)
    {
        if (_mustReset)
        {
            reset();
        }

        if (_running)
        {
            iaVector3f a, b;

            uint32 startIndex;
            uint32 endIndex;

            float64 frameTime = iTimer::getInstance().getMilliSeconds();

            // ignore hickups
            if (frameTime - _playbackTime > 100.0)
            {
                _playbackTime = frameTime;
            }

            float64 particleSystemTime = _playbackTime - _startTime;

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

            while (_playbackTime <= frameTime)
            {
                float32 sizeScale = 0;
                uint64 index = 0;
                float32 torqueFactor = 0;

                auto particleIter = _particles.begin();
                while (particleIter != _particles.end())
                {
                    iParticle &particle = (*particleIter);

                    particle._life -= 1.0f / _simulationRate;
                    if (particle._life <= 0)
                    {
                        particleIter = _particles.erase(particleIter);
                    }
                    else
                    {
                        _sizeScaleGradient.getValue(particle._visibleTime, sizeScale);

                        particle._velocity[1] += particle._lift;

                        particle._velocity *= _airDrag;

                        particle._orientation += particle._orientationRate;

                        particle._sizeScale = sizeScale;

                        particle._phase0.rotateXY(_octave1Rotation);
                        particle._phase1.rotateXY(_octave2Rotation);

                        particle._visibleTime += particle._visibleTimeIncrease;
                        if (particle._visibleTime > 1.0f)
                        {
                            particle._visible = false;
                        }

                        if (particle._torque != 0.0)
                        {
                            _torqueFactorGradient.getValue(particle._visibleTime, torqueFactor);

                            startIndex = index - _vortexCheckRange;
                            if (startIndex > 0)
                            {
                                startIndex = 0;
                            }

                            endIndex = index + _vortexCheckRange;
                            if (endIndex > _particles.size())
                            {
                                endIndex = _particles.size();
                            }

                            // TODO need overflow here

                            for (uint32 i = startIndex; i < endIndex; ++i)
                            {
                                if (index == i) // ignore your self
                                {
                                    continue;
                                }

                                a = particle._position - _particles[i]._position;
                                if (a.length() > particle._vortexRange)
                                {
                                    continue;
                                }

                                b = a % particle._normal;
                                b.normalize();
                                b *= (particle._vortexRange - a.length()) / particle._vortexRange;
                                b *= particle._torque * torqueFactor;
                                a.normalize();
                                a *= _vorticityConfinement;
                                b += a;

                                _particles[i]._velocity += b * 0.001f; // TODO 0.001 ???
                            }
                        }

                        particle._position += particle._velocity;

                        particleIter++;
                        index++;
                    }
                }

                float32 emissionRate = 0.0f;
                _emissionRateGradient.getValue(particleSystemTime / __IGOR_SECOND__, emissionRate);
                _emissionImpulseStack += emissionRate;
                int32 createCount = static_cast<int32>(_emissionImpulseStack);
                _emissionImpulseStack -= static_cast<float32>(createCount);
                createParticles(createCount, emitter, particleSystemTime / __IGOR_SECOND__);

                _playbackTime += 1000.0 / _simulationRate;
                particleSystemTime += 1000.0 / _simulationRate; // TODO redundant
            }
        }

        // TODO this seems a bit inefficient
        if (!_particles.empty())
        {
            iaVector3f minPos = _particles.front()._position;
            iaVector3f maxPos = _particles.front()._position;

            for (int i = 0; i < _particles.size(); i += 40)
            {
                if (_particles[i]._position._x < minPos._x)
                {
                    minPos._x = _particles[i]._position._x;
                }

                if (_particles[i]._position._y < minPos._y)
                {
                    minPos._y = _particles[i]._position._y;
                }

                if (_particles[i]._position._z < minPos._z)
                {
                    minPos._z = _particles[i]._position._z;
                }

                if (_particles[i]._position._x > maxPos._x)
                {
                    maxPos._x = _particles[i]._position._x;
                }

                if (_particles[i]._position._y > maxPos._y)
                {
                    maxPos._y = _particles[i]._position._y;
                }
                if (_particles[i]._position._z > maxPos._z)
                {
                    maxPos._z = _particles[i]._position._z;
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
            _boundingSphere._radius = max(_boundingBox._halfWidths._x, max(_boundingBox._halfWidths._y, _boundingBox._halfWidths._z));
        }
    }

    uint32 iParticleSystem3D::getParticleCount()
    {
        return static_cast<uint32>(_particles.size());
    }

    float32 iParticleSystem3D::getSimulationRate()
    {
        return _simulationRate;
    }

    void iParticleSystem3D::setStartSizeGradient(const iaGradientVector2f& sizeGradient)
    {
        _startSizeGradient = sizeGradient;
        _mustReset = true;
    }

    void iParticleSystem3D::getStartSizeGradient(iaGradientVector2f& sizeGradient) const
    {
        sizeGradient = _startSizeGradient;
    }

    void iParticleSystem3D::setStartVelocityGradient(const iaGradientVector2f& velocityGradient)
    {
        _startVelocityGradient = velocityGradient;
        _mustReset = true;
    }

    void iParticleSystem3D::getStartVelocityGradient(iaGradientVector2f& velocityGradient) const
    {
        velocityGradient = _startVelocityGradient;
    }

    void iParticleSystem3D::setStartLiftGradient(const iaGradientVector2f& liftGradient)
    {
        _startLiftGradient = liftGradient;
        _mustReset = true;
    }

    void iParticleSystem3D::getStartLiftGradient(iaGradientVector2f& liftGradient) const
    {
        liftGradient = _startLiftGradient;
    }

    void iParticleSystem3D::setAirDrag(float32 airDrag)
    {
        _airDrag = airDrag;
        _mustReset = true;
    }

    float32 iParticleSystem3D::getAirDrag() const
    {
        return _airDrag;
    }

    void iParticleSystem3D::setStartOrientationGradient(const iaGradientVector2f& orientationGradient)
    {
        _startOrientationGradient = orientationGradient;
        _mustReset = true;
    }

    void iParticleSystem3D::getStartOrientationGradient(iaGradientVector2f& orientationGradient) const
    {
        orientationGradient = _startOrientationGradient;
    }

    void iParticleSystem3D::setStartOrientationRateGradient(const iaGradientVector2f& orientationRateGradient)
    {
        _startOrientationRateGradient = orientationRateGradient;
        _mustReset = true;
    }

    void iParticleSystem3D::getStartOrientationRateGradient(iaGradientVector2f& orientationRateGradient) const
    {
        orientationRateGradient = _startOrientationRateGradient;
    }

    void iParticleSystem3D::setVorticityConfinement(float32 vc)
    {
        _vorticityConfinement = vc;
        _mustReset = true;
    }

};
