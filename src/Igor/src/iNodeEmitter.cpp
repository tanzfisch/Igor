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
        setName(L"iNodeEmitter");
        _nodeType = iNodeType::iNodeEmitter;
        _nodeKind = iNodeKind::Node;
	}

    iNodeEmitter::iNodeEmitter(iNodeEmitter* node)
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = node->_nodeType;
        _nodeKind = node->_nodeKind;

        setName(node->getName());

        _emitter.setType(node->_emitter.getType());
    }

    void iNodeEmitter::setType(iEmitterType emitterType)
    {
        _emitter.setType(emitterType);
    }

    iEmitterType iNodeEmitter::getType() const
    {
        return _emitter.getType();
    }

    void iNodeEmitter::setVelocity(float32 velocity)
    {
        _emitter.setVelocity(velocity);
    }

    float32 iNodeEmitter::getVelocity() const
    {
        return _emitter.getVelocity();
    }

    const iaMatrixf& iNodeEmitter::getWorldMatrix() const
    {
        return _worldMatrix;
    }
    
	void iNodeEmitter::addTriangle(const iEmitterTriangle& triangle)
    {
        _emitter.addTriangle(triangle);
	}

    void iNodeEmitter::clearTriangles()
    {
        _emitter.clearTriangles();
    }

    void iNodeEmitter::startEmitting()
    {
        _emitter.startEmitting();
    }

    void iNodeEmitter::stopEmitting()
    {
        _emitter.stopEmitting();
    }

    bool iNodeEmitter::isEmitting() const
    {
        return _emitter.isEmitting();
    }

    void iNodeEmitter::onUpdateTransform(iaMatrixf& matrix)
    {
        _worldMatrix = matrix;
        _emitter.setMatrix(matrix);
    }

    const iParticleEmitter& iNodeEmitter::getParticleEmitter() const
    {
        return _emitter;
    }

    void iNodeEmitter::setSize(float32 size)
    {
        _emitter.setSize(size);
    }

    float32 iNodeEmitter::getSize() const
    {
        return _emitter.getSize();
    }

};
