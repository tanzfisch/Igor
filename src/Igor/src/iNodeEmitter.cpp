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

        _emitterTriangles = node->_emitterTriangles;
    }

	iNodeEmitter::~iNodeEmitter()
	{
	}

    const iaMatrixf& iNodeEmitter::getWorldMatrix() const
    {
        return _worldMatrix;
    }
    
	void iNodeEmitter::addTriangleEmitter(const iEmitterTriangle& emitterTriangle)
	{
		_emitterTriangles.push_back(emitterTriangle);
	}

    void iNodeEmitter::onUpdateTransform(iaMatrixf& matrix)
    {
        _worldMatrix = matrix;
    }

    vector<iEmitterTriangle>* iNodeEmitter::getTriangles()
    {
        return &_emitterTriangles;
    }

};
