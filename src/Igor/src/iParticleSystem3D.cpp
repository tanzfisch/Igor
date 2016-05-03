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
        _lifeTimeStep = 1.0f / _lifeTime;

        initDefaultGradients();
    }

    iParticleSystem3D::~iParticleSystem3D()
    {
    }

    void iParticleSystem3D::initDefaultGradients()
    {
        _startVisibleTimeGradient.insertValue(0.0, 3);

        _colorGradient.insertValue(0.0, iaColor4f(1, 1, 1, 1));
        _emissionRateGradient.insertValue(0.0, 2);
        _emissionRateGradient.insertValue(1.0, 2);
        _emissionRateGradient.insertValue(1.1, 0);
        _emissionRateGradient.insertValue(2.0, 0);
        _emissionRateGradient.insertValue(2.1, 2);
        _emissionRateGradient.insertValue(3.0, 2);
        _emissionRateGradient.insertValue(3.1, 0);
        _emissionRateGradient.insertValue(4.0, 0);

        _sizeScaleGradient.insertValue(0.0, 1.0);
        _sizeScaleGradient.insertValue(1.5, 3.0);
        _sizeScaleGradient.insertValue(3.0, 0.0);

        _startSizeGradient.insertValue(0.0, iaVector2f(1.0, 1.2));

        _startVelocityGradient.insertValue(0.0, iaVector2f(0.1, 0.2));
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

    float32 iParticleSystem3D::getLiftMin()
    {
        return _minLift;
    }

    float32 iParticleSystem3D::getLiftMax()
    {
        return _maxLift;
    }

    float32 iParticleSystem3D::getLiftDecrease()
    {
        return _reduceLiftStep;
    }

    float32 iParticleSystem3D::getWeightMin()
    {
        return _minWeight;
    }

    float32 iParticleSystem3D::getWeightMax()
    {
        return _maxWeight;
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
        return _minVRange;
    }

    float32 iParticleSystem3D::getVortexRangeMax()
    {
        return _maxVRange;
    }

    uint32 iParticleSystem3D::getVortexCheckRange()
    {
        return _vortexCheckRange;
    }

    void iParticleSystem3D::setVortexCheckRange(uint32 checkRange)
    {
        _vortexCheckRange = checkRange;
    }

    void iParticleSystem3D::setLiftDecrease(float32 ld)
    {
        _reduceLiftStep = ld;
        _mustReset = true;
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
        for (auto particle : _particles)
        {
            delete particle;
        }

        _particles.clear();
        _vortexParticles.clear();

        float32 maxVisibleTime = 0;
        for (auto value : _startVisibleTimeGradient.getValues())
        {
            if (value.second > maxVisibleTime)
            {
                maxVisibleTime = value.second;
            }
        }

        _particleSystemPeriodTime = 5000.0; // todo longest gradient definition

        _lifeTime = maxVisibleTime;

        _mustReset = false;

        start();
    }

    void iParticleSystem3D::setLift(float32 min, float32 max)
    {
        _minLift = min;
        _maxLift = max;
        _mustReset = true;
    }

    void iParticleSystem3D::setWeight(float32 min, float32 max)
    {
        _minWeight = min;
        _maxWeight = max;
        _mustReset = true;
    }

    void iParticleSystem3D::setVortexTorque(float32 min, float32 max)
    {
        _minVortexTorque = min;
        _maxVortexTorque = max;
        _mustReset = true;
    }

    void iParticleSystem3D::setVortexRange(float32 min, float32 max)
    {
        _minVRange = min;
        _maxVRange = max;
        _mustReset = true;
    }

    void iParticleSystem3D::resetParticle(iParticle *particle, const iParticleEmitter& emitter, float32 particleSystemTime)
    {
        iaVector3f position;
        iaVector3f velocity;
        emitter.calcRandomStart(position, velocity);

        iaVector2f minMaxVelocity;
        float32 vel = 0;
        _startVelocityGradient.getValue(particleSystemTime, minMaxVelocity);
        vel = minMaxVelocity._x + (rand() % 100 / 100.0f) * (minMaxVelocity._y - minMaxVelocity._x);

        velocity *= vel;

        particle->_position = position;
        particle->_velocity = velocity;

        particle->_lift = _minLift + (rand() % 100 / 100.0f) * (_maxLift - _minLift);
        particle->_weight = _minWeight + (rand() % 100 / 100.0f) * (_maxWeight - _minWeight);

        iaVector2f minMaxSize;
        _startSizeGradient.getValue(particleSystemTime, minMaxSize);
        particle->_size = minMaxSize._x + (rand() % 100 / 100.0f) * (minMaxSize._y - minMaxSize._x);

        particle->_life = _lifeTime;
        particle->_visibleTime = _lifeTime * (0.9 + rand() % 100 / 1000.0f);
        particle->_visible = true;

        particle->_phase0.set(rand() % 100 / 100.0f, rand() % 100 / 100.0f);
        particle->_phase1.set(rand() % 100 / 100.0f, rand() % 100 / 100.0f);
    }

    void iParticleSystem3D::setVortexParticleLikeliness(float32 likeliness)
    {
        _vortexLikeliness = likeliness;
    }

    float32 iParticleSystem3D::getVortexParticleLikeliness() const
    {
        return _vortexLikeliness;
    }

    void iParticleSystem3D::createParticles(uint32 particleCount, const iParticleEmitter& emitter, float32 particleSystemTime)
    {
        iParticle *particle = nullptr;
        iVortexParticle *vortexparticle = nullptr;

        for (uint32 i = 0; i < particleCount; ++i)
        {
            if (rand() % 100 / 100.0f < _vortexLikeliness)
            {
                vortexparticle = new iVortexParticle();

                vortexparticle->_torque = _minVortexTorque + (rand() % 100 / 100.0f) * (_maxVortexTorque - _minVortexTorque);
                vortexparticle->_range = _minVRange + (rand() % 100 / 100.0f) * (_maxVRange - _minVRange);
                vortexparticle->_normal.set(rand() % 100 / 100.0f - 0.5f, rand() % 100 / 100.0f - 0.5f, rand() % 100 / 100.0f - 0.5f);
                vortexparticle->_normal.normalize();
                vortexparticle->_imune = true;
                vortexparticle->_particleid = i;

                _vortexParticles.push_back(vortexparticle);
                particle = (iParticle*)vortexparticle;
            }
            else
            {
                particle = new iParticle();
                particle->_imune = false;
            }

            resetParticle(particle, emitter, particleSystemTime);
            _particles.push_back(particle);
        }
    }

    vector<iParticle*> iParticleSystem3D::getCurrentFrame()
    {
        return _particles;
    }

    vector<iVortexParticle*> iParticleSystem3D::getCurrentFrameVortex()
    {
        return _vortexParticles;
    }

    void iParticleSystem3D::setParticleSystemMatrix(const iaMatrixf& worldInvMatrix)
    {
        _particleSystemInvWorldMatrix = worldInvMatrix;
    }

    void iParticleSystem3D::setStartVisibleTimeGradient(const iGradientf& startVisibleTimeGradient)
    {
        _startVisibleTimeGradient = startVisibleTimeGradient;
    }

    void iParticleSystem3D::getStartVisibleTimeGradient(iGradientf& startVisibleTimeGradient) const
    {
        startVisibleTimeGradient = _startVisibleTimeGradient;
    }

    void iParticleSystem3D::setSizeScaleGradient(const iGradientf& sizeScaleGradient)
    {
        _sizeScaleGradient = sizeScaleGradient;
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

            uint32 start, end;
            float32 vortexTorque;

            float64 frameTime = iTimer::getInstance().getMilliSeconds();
            float64 particleSystemTime = _time - _startTime;

            if (particleSystemTime >= _particleSystemPeriodTime)
            {
                if (_loop)
                {
                    _mustReset = true;
                }
                else
                {
                    _finished = true;
                    _running = false;
                }
            }

            while (_time <= frameTime)
            {
                for (auto vortexParticle : _vortexParticles)
                {
                    vortexTorque = vortexParticle->_torque;

                    start = vortexParticle->_particleid - _vortexCheckRange;
                    if (start > 0)
                    {
                        start = 0;
                    }

                    end = vortexParticle->_particleid + _vortexCheckRange;
                    if (end > _particles.size())
                    {
                        end = _particles.size();
                    }

                    for (uint32 i = start; i < end; ++i)
                    {
                    /*    if (_particles[i]->_imune)
                        {
                            continue;
                        }*/

                        a = vortexParticle->_position - _particles[i]->_position;
                        if (a.length() > vortexParticle->_range)
                        {
                            continue;
                        }

                        b = a % vortexParticle->_normal;
                        b.normalize();
                        b *= (vortexParticle->_range - a.length()) / vortexParticle->_range;
                        b *= vortexTorque;
                        a.normalize();
                        a *= _vorticityConfinement;
                        b += a;

                        _particles[i]->_position += b * 0.1f; // TODO 0.1 ???
                    }
                }

                float32 sizeScale = 0;

                for (auto particle : _particles)
                {
                    _sizeScaleGradient.getValue(particle->_visibleTime, sizeScale);

                    particle->_velocity[1] += particle->_lift;
                    particle->_velocity[1] -= particle->_weight;

                    particle->_position += particle->_velocity;

                    particle->_lift -= _reduceLiftStep;
                    particle->_sizeScale = sizeScale;

                    particle->_visibleTime -= 0.02;
                    if (particle->_visibleTime <= 0.0f)
                    {
                        particle->_visible = false;
                    }

                    particle->_phase0.rotateXY(_octave1Rotation);
                    particle->_phase1.rotateXY(_octave2Rotation);

                    particle->_life -= _lifeTimeStep;
                }

                uint32 emissionRate = 0;
                _emissionRateGradient.getValue(particleSystemTime / __IGOR_SECOND__, emissionRate); // TODO work with factions
                createParticles(emissionRate, emitter, particleSystemTime / __IGOR_SECOND__);

                _time += 20.0; // particle system runs in 50Hz
                particleSystemTime += 20; // TODO redundant
            }
        }
    }

    void iParticleSystem3D::setStartSizeGradient(const iGradientVector2f& sizeGradient)
    {
        _startSizeGradient = sizeGradient;
    }

    void iParticleSystem3D::getStartSizeGradient(iGradientVector2f& sizeGradient) const
    {
        sizeGradient = _startSizeGradient;
    }

    void iParticleSystem3D::setStartVelocityGradient(const iGradientVector2f& velocityGradient)
    {
        _startVelocityGradient = velocityGradient;
    }

    void iParticleSystem3D::getStartVelocityGradient(iGradientVector2f& velocityGradient) const
    {
        velocityGradient = _startVelocityGradient;
    }

    void iParticleSystem3D::setVorticityConfinement(float32 vc)
    {
        _vorticityConfinement = vc;
        _mustReset = true;
    }

};
