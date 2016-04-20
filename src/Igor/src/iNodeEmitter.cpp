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

	iNodeEmitter::~iNodeEmitter()
	{
	}

    void iNodeEmitter::setType(iEmitterType emitterType)
    {
        _emitter.setType(emitterType);
    }

    iEmitterType iNodeEmitter::getType() const
    {
        return _emitter.getType();
    }

    const iaMatrixf& iNodeEmitter::getWorldMatrix() const
    {
        return _worldMatrix;
    }
    
	void iNodeEmitter::addTriangleEmitter(const iEmitterTriangle& emitterTriangle)
	{
		//_emitterTriangles.push_back(emitterTriangle);
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

};
