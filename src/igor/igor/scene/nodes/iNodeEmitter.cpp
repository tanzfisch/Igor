// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeEmitter.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iNodeEmitter::iNodeEmitter()
    {
        setName(L"iNodeEmitter");
        _nodeType = iNodeType::iNodeEmitter;
        _nodeKind = iNodeKind::Node;
    }

    iNodeEmitter::iNodeEmitter(iNodeEmitter *node)
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = node->_nodeType;
        _nodeKind = node->_nodeKind;

        setName(node->getName());

        _emitter.setType(node->_emitter.getType());
        _emitter.setSize(node->_emitter.getSize());
    }

    void iNodeEmitter::setEmitterType(iEmitterType emitterType)
    {
        _emitter.setType(emitterType);
    }

    iEmitterType iNodeEmitter::getEmitterType() const
    {
        return _emitter.getType();
    }

    const iaMatrixd &iNodeEmitter::getWorldMatrix() const
    {
        return _worldMatrix;
    }

    void iNodeEmitter::addTriangle(const iEmitterTriangle &triangle)
    {
        _emitter.addTriangle(triangle);
    }

    void iNodeEmitter::clearTriangles()
    {
        _emitter.clearTriangles();
    }

    void iNodeEmitter::onUpdateTransform(iaMatrixd &matrix)
    {
        if (_worldMatrix != matrix)
        {
            _worldMatrix = matrix;
            _emitter.setWorldMatrix(matrix);
        }
    }

    iParticleEmitter &iNodeEmitter::getParticleEmitter()
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

}; // namespace igor
