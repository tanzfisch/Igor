// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include "iParticleSystem2D.h"

#include <iTimer.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iParticleSystem2D::iParticleSystem2D()
    {
        _rand.setSeed(static_cast<uint32>(iTimer::getInstance().getTime()));

        setMaxParticleCount(100);
    }

    iParticleSystem2D::~iParticleSystem2D()
    {
        if (_particles != nullptr)
        {
            delete[] _particles;
            _particles = nullptr;
        }
    }

    void iParticleSystem2D::resetParticle(iParticle2D& particle)
    {
        if (_emitterType == iEmitterType::PointEmitter)
        {
            particle._position = _emitterPos;
            particle._velocity = _initialVelocity;

            float32 spread = ((_rand.getNext() % 100 / 50.0f) - 1.0f) * _spreadFactor * 0.5f;

            particle._velocity.rotateXY(spread * 2.0 * M_PI);
        }
        else
        {
            //! \todo
        }

        particle._size = _minSize + (_rand.getNext() % 100 / 100.0f) * (_maxSize - _minSize);
        particle._angle = _minRotation + (_rand.getNext() % 100 / 100.0f) * (_maxRotation - _minRotation);
        particle._sizeDelta = _minSizeDelta + (_rand.getNext() % 100 / 100.0f) * (_maxSizeDelta - _minSizeDelta);
        particle._angleDelta = _minRotationDelta + (_rand.getNext() % 100 / 100.0f) * (_maxRotationDelta - _minRotationDelta);
        particle._life = 1.0f;
    }

    iParticle2D* iParticleSystem2D::getParticles()
    {
        return _particles;
    }

    void iParticleSystem2D::restart()
    {
        for (int i = 0; i < _particleCount; ++i)
        {
            resetParticle(_particles[i]);
        }
    }

    void iParticleSystem2D::handle()
    {
        iParticle2D *newparticle;

        if (_particleCount + _emitRate <= _maxParticleCount)
        {
            int startIndex = _particleCount;
            _particleCount += _emitRate;
            
            for (int i = startIndex; i < _particleCount; ++i)
            {
                resetParticle(_particles[i]);
            }
        }

        if (!_rebirth)
        {
            _done = true;
        }

        for (int i = 0; i < _particleCount; ++i)
        {
            if (0 < _particles[i]._life)
            {
                _done = false;

                // Geschwindigkeiten der Partikel neu berechnen
                _particles[i]._velocity += _externalForce;	// Einfluss der Gravitation
                _particles[i]._velocity *= _airDrag;		// Einfluss Luftwiederstand

                // Positionen der Partikel neu berechnen
                _particles[i]._position += _particles[i]._velocity;

                // rotation, skalierung etc.
                _particles[i]._angle += _particles[i]._angleDelta;
                _particles[i]._size += _particles[i]._sizeDelta;

                // lebenszyklus überwachen
                _particles[i]._life -= _lifeDelta;
                if (_particles[i]._life <= 0.0f)
                {
                    if (_rebirth)
                    {
                        resetParticle(_particles[i]);
                    }
                }
            }
        }
    }

    bool iParticleSystem2D::finished()
    {
        return _done;
    }

    void iParticleSystem2D::translateEmitter(iaVector2f v)
    {
        _emitterPos += v;
    }

    void iParticleSystem2D::setEmitterPos(iaVector2f v)
    {
        _emitterPos = v;
    }

    void iParticleSystem2D::setSpreadFactor(float32 spreadFactor)
    {
        _spreadFactor = spreadFactor;
    }

    void iParticleSystem2D::setParticleSizeDelta(float32 min, float32 max)
    {
        _minSizeDelta = min;
        _maxSizeDelta = max;
    }

    void iParticleSystem2D::setParticleSize(float32 min, float32 max)
    {
        _minSize = min;
        _maxSize = max;
    }

    void iParticleSystem2D::setParticleRotationDelta(float32 min, float32 max)
    {
        _minRotationDelta = min;
        _maxRotationDelta = max;
    }

    void iParticleSystem2D::setParticleRotation(float32 min, float32 max)
    {
        _minRotation = min;
        _maxRotation = max;
    }

    void iParticleSystem2D::setParticleLifetime(int32 iterations)
    {
        _particleLifeTime = iterations;
        _lifeDelta = 1.0f / static_cast<float32>(iterations);
    }

    void iParticleSystem2D::setMaxParticleCount(int32 count)
    {
        con_assert(count > 1, "out of bounds");

        if (_maxParticleCount != count)
        {
            _maxParticleCount = count;

            if (_particles != nullptr)
            {
                delete[] _particles;
            }

            _particles = new iParticle2D[_maxParticleCount];
            _particleCount = 0;
        }
    }

    int32 iParticleSystem2D::getMaxParticleCount()
    {
        return _maxParticleCount;
    }

    /*! \returns current in use particle count
    */
    int32 iParticleSystem2D::getParticleCount()
    {
        return _particleCount;
    }

    void iParticleSystem2D::setExternalForce(const iaVector2f& force)
    {
        _externalForce = force;
    }

    void iParticleSystem2D::setInitialVelocity(const iaVector2f& initialVelocity)
    {
        _initialVelocity = initialVelocity;
    }

    void iParticleSystem2D::setAirDrag(float32 airDrag)
    {
        con_assert(airDrag >= 0.0 && airDrag <= 1.0, "value out of range");

        if (airDrag >= 0.0 && airDrag <= 1.0)
        {
            _airDrag = 1.0 - airDrag;
        }
    }

    int32 iParticleSystem2D::getEmitRate() const
    {
        return _emitRate;
    }

    int32 iParticleSystem2D::getParticleLifetime() const
    {
        return _particleLifeTime;
    }

    void iParticleSystem2D::setLoopable(bool loop)
    {
        _rebirth = loop;
    }

    void iParticleSystem2D::setEmitRate(int32 emitRate)
    {
        _emitRate = emitRate;
    }

};

