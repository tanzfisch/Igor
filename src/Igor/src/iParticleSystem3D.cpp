// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iParticleSystem3D.h>
#include <iaConsole.h>
#include <iRenderer.h>
#include <iTextureResourceFactory.h>
#include <iMaterialResourceFactory.h>
#include <iGradient.h>
#include <iParticleEmitter.h>

#include <iostream>
using namespace std;

namespace Igor
{

    iParticleSystem3D::iParticleSystem3D()
    {
        _lifeTimeStep = 1.0f / _lifeTime;
    }

    iParticleSystem3D::~iParticleSystem3D()
    {
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

    float32 iParticleSystem3D::getSizeMin()
    {
        return _minSize;
    }

    float32 iParticleSystem3D::getSizeMax()
    {
        return _maxSize;
    }

    float32 iParticleSystem3D::getSizeIncrease()
    {
        return _sizeIncreaseStep;
    }

    uint32 iParticleSystem3D::getParticleCount()
    {
        return _particleCount;
    }

    float32 iParticleSystem3D::getVorticityConfinement()
    {
        return _vorticityConfinement;
    }

    uint32 iParticleSystem3D::getVortexParticleCount()
    {
        return _vortexCount;
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

    void iParticleSystem3D::setVortexCheckRange(uint32 _particles)
    {
        _vortexCheckRange = _particles;
    }

    void iParticleSystem3D::setSizeIncrease(float32 si)
    {
        _sizeIncreaseStep = si;
        _mustReset = true;
    }

    void iParticleSystem3D::setParticleCount(uint32 count)
    {
        _particleCount = count;
        _mustReset = true;
    }

    void iParticleSystem3D::setParticleLifeTime(uint32 frames)
    {
        con_assert(frames != 0, "can't set zero lifetime");

        if (frames != 0)
        {
            _lifeTime = frames;
            _lifeTimeStep = 1.0f / _lifeTime;
            _mustReset = true;
        }
        else
        {
            con_err("can't set zero lifetime");
        }
    }

    uint32 iParticleSystem3D::getParticleLifeTime()
    {
        return _lifeTime;
    }

    void iParticleSystem3D::setLiftDecrease(float32 ld)
    {
        _reduceLiftStep = ld;
        _mustReset = true;
    }

    void iParticleSystem3D::setVortexParticleCount(uint32 count)
    {
        _vortexCount = count;
        _mustReset = true;
    }

    void iParticleSystem3D::reset(const iParticleEmitter& emitter)
    {
        con_assert_sticky(_vortexCount <= _particleCount, "can't have more vortex particles than particles");

        _initFrame = _lifeTime;

        calcBirth(emitter);
        _mustReset = false;
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

    void iParticleSystem3D::setSize(float32 min, float32 max)
    {
        _minSize = min;
        _maxSize = max;
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

    void iParticleSystem3D::resetParticle(iParticle *particle, const iParticleEmitter& emitter)
    {
        iaVector3f position;
        iaVector3f velocity;
        emitter.calcRandomStart(position, velocity);

        particle->_position = position;
        particle->_velocity = velocity;

        particle->_lift = _minLift + (rand() % 100 / 100.0f) * (_maxLift - _minLift);
        particle->_weight = _minWeight + (rand() % 100 / 100.0f) * (_maxWeight - _minWeight);
        particle->_size = _minSize + (rand() % 100 / 100.0f) * (_maxSize - _minSize);

        particle->_life = 1.0f;
        particle->_visibleTime = 1.0f;
        particle->_visibleTimeStep = (particle->_visibleTime / _lifeTime) * (0.9 + rand() % 100 / 1000.0f);
        particle->_visible = true;

        particle->_phase0.set(rand() % 100 / 100.0f, rand() % 100 / 100.0f);
        particle->_phase1.set(rand() % 100 / 100.0f, rand() % 100 / 100.0f);
    }

    void iParticleSystem3D::calcBirth(const iParticleEmitter& emitter)
    {
        iParticle *particle;
        iVortexParticle *vortexparticle;

        iaMatrixf modelview;

        _particles.clear();

        for (uint32 i = 0; i < _particleCount; ++i)
        {
            if ((i % (long)((float32)_particleCount / (float32)_vortexCount)) == 0)
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

            resetParticle(particle, emitter);
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

    void iParticleSystem3D::calcNextFrame(const iParticleEmitter& emitter)
    {
        _birthTransformationMatrix = emitter.getWorldMatrix();
        _birthTransformationMatrix *= _particleSystemInvWorldMatrix;

        if (_mustReset)
        {
            reset(emitter);
            if (_mustReset)
            {
                return;
            }
        }

        uint32 simulate = _particles.size();
        uint32 simulatevortex = _vortexParticles.size();

        if (_initFrame > 0)
        {
            _initFrame--;
            simulate = (uint32)((float32)_particles.size() / (float32)_lifeTime * ((float32)_lifeTime - (float32)_initFrame));
            simulatevortex = (uint32)((float32)_vortexParticles.size() / (float32)_lifeTime * ((float32)_lifeTime - (float32)_initFrame));
        }

        iaVector3f a, b;

        uint32 start, end;
        float32 vortexTorque;

        for (uint32 j = 0; j < simulatevortex; ++j)
        {
            if (_vortexParticles[j]->_life<0.1f)
            {
                vortexTorque = _vortexParticles[j]->_torque * _vortexParticles[j]->_life * 10.0f;
            }
            else if (_vortexParticles[j]->_life > 0.9f)
            {
                vortexTorque = _vortexParticles[j]->_torque * (1 - _vortexParticles[j]->_life) * 10.0f;
            }
            else
            {
                vortexTorque = _vortexParticles[j]->_torque;
            }

            start = _vortexParticles[j]->_particleid - _vortexCheckRange;
            if (start > 0)
            {
                start = 0;
            }

            end = _vortexParticles[j]->_particleid + _vortexCheckRange;
            if (end > simulate)
            {
                end = simulate;
            }

            for (uint32 i = start; i < end; ++i)
            {
                if (_particles[i]->_imune)
                {
                    continue;
                }

                a = _vortexParticles[j]->_position - _particles[i]->_position;
                if (a.length() > _vortexParticles[j]->_range)
                {
                    continue;
                }

                b = a % _vortexParticles[j]->_normal;
                b.normalize();
                b *= (_vortexParticles[j]->_range - a.length()) / _vortexParticles[j]->_range;
                b *= vortexTorque;
                a.normalize();
                a *= _vorticityConfinement;
                b += a;

                _particles[i]->_position += b * 0.1f; // TODO 0.1 ???
            }
        }

        iaVector3f gravity(0, -1, 0);

        for (uint32 i = 0; i < simulate; ++i)
        {
            _particles[i]->_velocity[1] += _particles[i]->_lift;
            _particles[i]->_velocity[1] -= _particles[i]->_weight;

            _particles[i]->_position += _particles[i]->_velocity;

            _particles[i]->_lift -= _reduceLiftStep;
            _particles[i]->_size += _sizeIncreaseStep;

            _particles[i]->_visibleTime -= _particles[i]->_visibleTimeStep;
            if (_particles[i]->_visibleTime <= 0.0f)
            {
                _particles[i]->_visible = false;
            }

            _particles[i]->_phase0.rotateXY(_octave1Rotation);
            _particles[i]->_phase1.rotateXY(_octave2Rotation);

            _particles[i]->_life -= _lifeTimeStep;

            if (_particles[i]->_life <= 0.0f)
            {
                resetParticle(_particles[i], emitter);

                _particles[i]->_visible = true;
            }
        }
    }

    void iParticleSystem3D::setVorticityConfinement(float32 vc)
    {
        _vorticityConfinement = vc;
        _mustReset = true;
    }

};
