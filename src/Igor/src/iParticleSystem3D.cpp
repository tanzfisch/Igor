// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iParticleSystem3D.h>
#include <iaConsole.h>
#include <iRenderer.h>
#include <iTextureResourceFactory.h>
#include <iMaterialResourceFactory.h>
#include <iGradient.h>
#include <iTimer.h>
#include <iParticleEmitter.h>

#include <iostream>
using namespace std;

namespace Igor
{

    iParticleSystem3D::iParticleSystem3D()
    {
        initDefaultGradients();
    }

    iParticleSystem3D::~iParticleSystem3D()
    {
    }

    void iParticleSystem3D::initDefaultGradients()
    {
        _startVisibleTimeGradient.insertValue(0.0, iaVector2f(2.5, 3.5));

        _colorGradient.insertValue(0.0, iaColor4f(1, 1, 1, 1));

        _emissionRateGradient.insertValue(0.0, 2);

        _sizeScaleGradient.insertValue(0.0, 1.0);

        _startSizeGradient.insertValue(0.0, iaVector2f(1.0, 1.2));

        _startVelocityGradient.insertValue(0.0, iaVector2f(0.0, 0.0));

        _torqueFactorGradient.insertValue(0.0, 0.0);
        _torqueFactorGradient.insertValue(0.1, 1.0);
        _torqueFactorGradient.insertValue(0.9, 1.0);
        _torqueFactorGradient.insertValue(1.0, 0.0);

        _startLiftGradient.insertValue(0.0, iaVector2f(0.0, 0.0));
    }

    void iParticleSystem3D::setColorGradient(const iGradientColor4f& colorGradient)
    {
        _colorGradient = colorGradient;
    }

    void iParticleSystem3D::getColorGradient(iGradientColor4f& colorGradient) const
    {
        colorGradient = _colorGradient;
    }

    const iGradientColor4f& iParticleSystem3D::getColorGradient() const
    {
        return _colorGradient;
    }

    void iParticleSystem3D::setEmissionGradient(const iGradientui& emissionGradient)
    {
        _emissionRateGradient = emissionGradient;
    }

    void iParticleSystem3D::getEmissionGradient(iGradientui& emissionGradient) const
    {
        emissionGradient = _emissionRateGradient;
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

    uint32 iParticleSystem3D::getVortexCheckRange()
    {
        return _vortexCheckRange;
    }

    void iParticleSystem3D::setVortexCheckRange(uint32 checkRange)
    {
        _vortexCheckRange = checkRange;
    }

    void iParticleSystem3D::start()
    {
        _startTime = iTimer::getInstance().getMilliSeconds();
        _time = _startTime;
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
        _particles.clear();

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

    void iParticleSystem3D::resetParticle(iParticle &particle, const iParticleEmitter& emitter, float32 particleSystemTime)
    {
        iaVector2f minMax;
        iaVector3f position;
        iaVector3f velocity;
        emitter.calcRandomStart(position, velocity);

        float32 randomFactor = (rand() % 1000 / 1000.0f);

        float32 vel = 0;
        _startVelocityGradient.getValue(particleSystemTime, minMax);
        vel = minMax._x + randomFactor * (minMax._y - minMax._x);
        velocity *= vel;

        particle._position = position;
        particle._velocity = velocity;

        _startLiftGradient.getValue(particleSystemTime, minMax);
        particle._lift = minMax._x + (1-randomFactor) * (minMax._y - minMax._x);

        _startSizeGradient.getValue(particleSystemTime, minMax);
        particle._size = minMax._x + randomFactor * (minMax._y - minMax._x);

        particle._life = _lifeTime;

        particle._visibleTime = 0.0;

        _startVisibleTimeGradient.getValue(particleSystemTime, minMax);
        float32 visibleTime = minMax._x + randomFactor * (minMax._y - minMax._x);
        visibleTime /= 0.02; // 50Hz
        particle._visibleTimeIncrease = 1.0 / visibleTime;
        particle._visible = true;

        particle._phase0.set(rand() % 100 / 100.0f, rand() % 100 / 100.0f);
        particle._phase1.set(rand() % 100 / 100.0f, rand() % 100 / 100.0f);
    }

    void iParticleSystem3D::setVortexParticleLikeliness(uint64 likeliness)
    {
        con_assert(likeliness >= 0 && likeliness <= 100, "out of range");

        _vortexLikeliness = likeliness;
        _mustReset = true;
    }

    float32 iParticleSystem3D::getVortexParticleLikeliness() const
    {
        return _vortexLikeliness;
    }

    void iParticleSystem3D::createParticles(uint32 particleCount, const iParticleEmitter& emitter, float32 particleSystemTime)
    {
        for (uint32 i = 0; i < particleCount; ++i)
        {
            iParticle particle;

            if (_vortexLikeliness != 0 && (_particleCounter % (long)(100.0f / static_cast<float32>(_vortexLikeliness))) == 0)
            {
                particle._normal.set(rand() % 100 / 100.0f - 0.5f, rand() % 100 / 100.0f - 0.5f, rand() % 100 / 100.0f - 0.5f);
                particle._normal.normalize();
                particle._torque = _minVortexTorque + (rand() % 100 / 100.0f) * (_maxVortexTorque - _minVortexTorque);
                particle._vortexRange = _minVortexRange + (rand() % 100 / 100.0f) * (_maxVortexRange - _minVortexRange);
            }
            else
            {
                particle._torque = 0.0;
            }

            resetParticle(particle, emitter, particleSystemTime);
            _particles.push_back(particle);

            _particleCounter++;
        }
    }

    const deque<iParticle>& iParticleSystem3D::getCurrentFrame() const
    {
        return _particles;
    }

    void iParticleSystem3D::setParticleSystemMatrix(const iaMatrixf& worldInvMatrix)
    {
        _particleSystemInvWorldMatrix = worldInvMatrix;
    }

    void iParticleSystem3D::setStartVisibleTimeGradient(const iGradientVector2f& startVisibleTimeGradient)
    {
        _startVisibleTimeGradient = startVisibleTimeGradient;
        _mustReset = true;
    }

    void iParticleSystem3D::getStartVisibleTimeGradient(iGradientVector2f& startVisibleTimeGradient) const
    {
        startVisibleTimeGradient = _startVisibleTimeGradient;
    }

    void iParticleSystem3D::setSizeScaleGradient(const iGradientf& sizeScaleGradient)
    {
        _sizeScaleGradient = sizeScaleGradient;
        _mustReset = true;
    }

    void iParticleSystem3D::getSizeScaleGradient(iGradientf& sizeScaleGradient) const
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

    void iParticleSystem3D::calcNextFrame(const iParticleEmitter& emitter)
    {
        if (_mustReset)
        {
            reset();
        }

        if (_running)
        {
            _birthTransformationMatrix = emitter.getWorldMatrix();
            _birthTransformationMatrix *= _particleSystemInvWorldMatrix;

            iaVector3f a, b;

            uint32 startIndex;
            uint32 endIndex;

            float64 frameTime = iTimer::getInstance().getMilliSeconds();
            float64 particleSystemTime = _time - _startTime;

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

            while (_time <= frameTime)
            {
                float32 sizeScale = 0;
                uint64 index = 0;
                float32 torqueFactor = 0;

                auto particle = _particles.begin();
                while (particle != _particles.end())
                {
                    _sizeScaleGradient.getValue((*particle)._visibleTime, sizeScale);

                    (*particle)._velocity[1] += (*particle)._lift;

                    (*particle)._velocity *= _airDrag;

                    (*particle)._position += (*particle)._velocity;

                    (*particle)._sizeScale = sizeScale;

                    (*particle)._phase0.rotateXY(_octave1Rotation);
                    (*particle)._phase1.rotateXY(_octave2Rotation);

                    (*particle)._visibleTime += (*particle)._visibleTimeIncrease;
                    if ((*particle)._visibleTime > 1.0f)
                    {
                        (*particle)._visible = false;
                    }

                    if ((*particle)._torque != 0.0)
                    {
                        _torqueFactorGradient.getValue((*particle)._visibleTime, torqueFactor);

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

                        for (uint32 i = startIndex; i < endIndex; ++i)
                        {
                            if (index == i) // ignore your self
                            {
                                continue;
                            }

                            a = (*particle)._position - _particles[i]._position;
                            if (a.length() > (*particle)._vortexRange)
                            {
                                continue;
                            }

                            b = a % (*particle)._normal;
                            b.normalize();
                            b *= ((*particle)._vortexRange - a.length()) / (*particle)._vortexRange;
                            b *= (*particle)._torque * torqueFactor;
                            a.normalize();
                            a *= _vorticityConfinement;
                            b += a;

                            _particles[i]._position += b * 0.1f; // TODO 0.1 ???
                        }
                    }

                    (*particle)._life -= 0.02; // 50Hz
                    if ((*particle)._life <= 0)
                    {
                        particle = _particles.erase(particle);
                    }
                    else
                    {
                        particle++;
                        index++;
                    }
                }

                uint32 emissionRate = 0;
                _emissionRateGradient.getValue(particleSystemTime / __IGOR_SECOND__, emissionRate); // TODO work with fractions
                createParticles(emissionRate, emitter, particleSystemTime / __IGOR_SECOND__);

                _time += 20.0; // particle system runs in 50Hz
                particleSystemTime += 20; // TODO redundant
            }
        }
    }

    void iParticleSystem3D::setStartSizeGradient(const iGradientVector2f& sizeGradient)
    {
        _startSizeGradient = sizeGradient;
        _mustReset = true;
    }

    void iParticleSystem3D::getStartSizeGradient(iGradientVector2f& sizeGradient) const
    {
        sizeGradient = _startSizeGradient;
    }

    void iParticleSystem3D::setStartVelocityGradient(const iGradientVector2f& velocityGradient)
    {
        _startVelocityGradient = velocityGradient;
        _mustReset = true;
    }

    void iParticleSystem3D::getStartVelocityGradient(iGradientVector2f& velocityGradient) const
    {
        velocityGradient = _startVelocityGradient;
    }

    void iParticleSystem3D::setStartLiftGradient(const iGradientVector2f& liftGradient)
    {
        _startLiftGradient = liftGradient;
        _mustReset = true;
    }

    void iParticleSystem3D::getStartLiftGradient(iGradientVector2f& liftGradient) const
    {
        liftGradient = _startLiftGradient;
    }

    void iParticleSystem3D::setAirDrag(float32 airDrag)
    {
        _airDrag = airDrag;
    }

    float32 iParticleSystem3D::getAirDrag() const
    {
        return _airDrag;
    }

    void iParticleSystem3D::setVorticityConfinement(float32 vc)
    {
        _vorticityConfinement = vc;
        _mustReset = true;
    }

};
