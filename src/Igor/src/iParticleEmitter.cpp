// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iParticleEmitter.h>

namespace Igor
{

    iParticleEmitter::iParticleEmitter()
    {
    }

	iParticleEmitter::~iParticleEmitter()
	{
	}
    
    void iParticleEmitter::setMatrix(const iaMatrixf& matrix)
    {
        _matrix = matrix;
    }

    const iaMatrixf& iParticleEmitter::getMatrix() const
    {
        return _matrix;
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
        }
    }

    void iParticleEmitter::calcRandomStartFromPoint(iaVector3f& position, iaVector3f& velocity) const
    {
        position = _matrix._pos;
        velocity.set(0, 0, 0);
    }

    void iParticleEmitter::calcRandomStartFromMesh(iaVector3f& position, iaVector3f& velocity) const
    {
        if (_emitterTriangles != nullptr && 
            !_emitterTriangles->empty())
        {
            iEmitterTriangle &emitter = (*_emitterTriangles)[rand() % _emitterTriangles->size()];
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
            position = _matrix * posOnEmitter;

            // TODO refactor!!!!

            ab = emitter.vel[0] * (1.0f - u) + emitter.vel[1] * u;
            ac = emitter.vel[2] * (1.0f - u) + emitter.vel[1] * u;

            velocity.set(0, 0, 0);
           /* iaVector3f velOnEmitter = ab*(1.0f - v) + ac*v;
            velocity = _birthTransformationMatrix * velOnEmitter;
            velocity -= _birthTransformationMatrix._pos;*/
        }
    }
};
