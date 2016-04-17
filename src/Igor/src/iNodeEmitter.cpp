// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeEmitter.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

	iNodeEmitter::iNodeEmitter()
	{
	}

	iNodeEmitter::~iNodeEmitter()
	{
	}
    
	void iNodeEmitter::addTriangleEmitter(const iEmitterTriangle& emitterTriangle)
	{
		_emitterTriangles.push_back(emitterTriangle);
	}

    void iNodeEmitter::onUpdateTransform(iaMatrixf& matrix)
    {
        _worldMatrixInv = matrix;
        _worldMatrixInv.invert();
    }

	void iNodeEmitter::calcRandomStart(iaVector3f& position, iaVector3f& velocity)
	{
		con_assert(!_emitterTriangles.empty(), "no data");

        if (!_emitterTriangles.empty())
        {
            iEmitterTriangle &emitter = _emitterTriangles[rand() % _emitterTriangles.size()];
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
            position = _worldMatrixInv * posOnEmitter;

            // TODO refactor!!!!

            ab = emitter.vel[0] * (1.0f - u) + emitter.vel[1] * u;
            ac = emitter.vel[2] * (1.0f - u) + emitter.vel[1] * u;

            iaVector3f velOnEmitter = ab*(1.0f - v) + ac*v;
            velocity = _worldMatrixInv * velOnEmitter;
            velocity -= _worldMatrixInv._pos;
        }
	}

};
