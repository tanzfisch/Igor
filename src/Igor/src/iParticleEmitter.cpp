// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iParticleEmitter.h>

#include <iaVector2.h>
using namespace IgorAux;

namespace Igor
{

    void iParticleEmitter::setWorldMatrix(const iaMatrixf& matrix)
    {
        _worldMatrix = matrix;
    }

    const iaMatrixf& iParticleEmitter::getWorldMatrix() const
    {
        return _worldMatrix;
    }

    void iParticleEmitter::setType(iEmitterType emitterType)
    {
        _type = emitterType;
    }

    iEmitterType iParticleEmitter::getType() const
    {
        return _type;
    }

    void iParticleEmitter::calcRandomStart(iaVector3f& position, iaVector3f& velocity) const
    {
        switch (_type)
        {
            case iEmitterType::Point:
                calcRandomStartFromPoint(position, velocity);
                break;

            case iEmitterType::Mesh:
                calcRandomStartFromMesh(position, velocity);
                break;

            case iEmitterType::Circle:
                calcRandomStartFromCircle(position, velocity);
                break;

            case iEmitterType::Sphere:
                calcRandomStartFromSphere(position, velocity);
                break;

            case iEmitterType::Disc:
                calcRandomStartFromDisc(position, velocity);
                break;

            case iEmitterType::Square:
                calcRandomStartFromSquare(position, velocity);
                break;

            case iEmitterType::Cube:
                calcRandomStartFromCube(position, velocity);
                break;
        }
    }
      
    void iParticleEmitter::setSize(float32 size)
    {
        _size = size;
    }

    float32 iParticleEmitter::getSize() const
    {
        return _size;
    }

    void iParticleEmitter::calcRandomStartFromCube(iaVector3f& position, iaVector3f& velocity) const
    {
        float32 x = ((rand() % 10000) / 10000.0f) * _size * 2 - _size;
        float32 y = ((rand() % 10000) / 10000.0f) * _size * 2 - _size;
        float32 z = ((rand() % 10000) / 10000.0f) * _size * 2 - _size;
        iaVector3f posOnEmitter(x, y, z);
        position = _worldMatrix * posOnEmitter;

        posOnEmitter.normalize();
        velocity = posOnEmitter;
    }

    void iParticleEmitter::calcRandomStartFromSquare(iaVector3f& position, iaVector3f& velocity) const
    {
        float32 x = ((rand() % 10000) / 10000.0f) * _size * 2 - _size;
        float32 z = ((rand() % 10000) / 10000.0f) * _size * 2 - _size;
        iaVector3f posOnEmitter(x, 0, z);
        position = _worldMatrix * posOnEmitter;

        velocity.set(0, 1, 0);
        velocity = _worldMatrix * velocity;
        velocity -= _worldMatrix._pos;
    }

    void iParticleEmitter::calcRandomStartFromSphere(iaVector3f& position, iaVector3f& velocity) const
    {
        // TODO optimize?
        float32 x, y, z;
        iaVector3f posOnEmitter;

        do
        {
            x = ((rand() % 20000) / 10000.0f) - 1;
            y = ((rand() % 20000) / 10000.0f) - 1;
            z = ((rand() % 20000) / 10000.0f) - 1;
            posOnEmitter.set(x, y, z);

        } while (posOnEmitter.length2() > 1);

        posOnEmitter *= _size;
        position = _worldMatrix * posOnEmitter;

        posOnEmitter.normalize();
        velocity = posOnEmitter;
    }

    void iParticleEmitter::calcRandomStartFromDisc(iaVector3f& position, iaVector3f& velocity) const
    {
        float32 angle = ((rand() % 10000) / 10000.0f) * M_PI * 2;
        float32 distance = ((rand() % 10000) / 10000.0f) * _size;
        iaVector2f temp(distance, 0);
        temp.rotateXY(angle);
        iaVector3f posOnEmitter(temp._x, 0, temp._y);
        position = _worldMatrix * posOnEmitter;

        velocity.set(0, 1, 0);
        velocity = _worldMatrix * velocity;
        velocity -= _worldMatrix._pos;
    }

    void iParticleEmitter::calcRandomStartFromCircle(iaVector3f& position, iaVector3f& velocity) const
    {
        float32 angle = ((rand() % 10000) / 10000.0f) * M_PI * 2;
        iaVector2f temp(_size, 0);
        temp.rotateXY(angle);
        iaVector3f posOnEmitter(temp._x, 0, temp._y);
        position = _worldMatrix * posOnEmitter;

        velocity.set(0, 1, 0);
        velocity = _worldMatrix * velocity;
        velocity -= _worldMatrix._pos;
    }

    void iParticleEmitter::calcRandomStartFromPoint(iaVector3f& position, iaVector3f& velocity) const
    {
        position = _worldMatrix._pos;

        float32 x = ((rand() % 20000) / 10000.0f) - 1;
        float32 y = ((rand() % 20000) / 10000.0f) - 1;
        float32 z = ((rand() % 20000) / 10000.0f) - 1;
        velocity.set(x,y,z);
        velocity.normalize();
    }

    void iParticleEmitter::addTriangle(const iEmitterTriangle& triangle)
    {
        _emitterTriangles.push_back(triangle);
    }

    void iParticleEmitter::clearTriangles()
    {
        _emitterTriangles.clear();
    }

    void iParticleEmitter::calcRandomStartFromMesh(iaVector3f& position, iaVector3f& velocity) const
    {
        if (!_emitterTriangles.empty())
        {
            const iEmitterTriangle &emitter = _emitterTriangles[rand() % _emitterTriangles.size()];
            float32 u;
            float32 v;

            do
            {
                u = rand() % 100 / 100.0f;
                v = rand() % 100 / 100.0f;
            } while (u + v > 1.0f);

            iaVector3f ab = emitter.pos[1] - emitter.pos[0];
            iaVector3f ac = emitter.pos[2] - emitter.pos[0];

            iaVector3f posOnEmitter = (ab*u) + (ac*v) + emitter.pos[0];
            position = _worldMatrix * posOnEmitter;

            // TODO refactor!!!!

            ab = emitter.vel[0] * (1.0f - u) + emitter.vel[1] * u;
            ac = emitter.vel[2] * (1.0f - u) + emitter.vel[1] * u;

            velocity.set(0, 0, 0);
            iaVector3f velOnEmitter = ab*(1.0f - v) + ac*v;
            velocity = _worldMatrix * velOnEmitter;
            velocity -= _worldMatrix._pos;
        }
    }
};