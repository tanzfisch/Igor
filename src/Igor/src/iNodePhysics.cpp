// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodePhysics.h>

#include <iScene.h>
#include <iPhysics.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iNodePhysics::iNodePhysics()
        : iNodeVolume()
    {
        setName(L"iNodePhysics");
        _nodeType = iNodeType::iNodePhysics;
        _nodeKind = iNodeKind::Physics;

        setMaterial(iMaterial::INVALID_MATERIAL_ID); // physics should only render for debugging purposes
    }

    iNodePhysics::iNodePhysics(iNodePhysics* node)
        : iNodeVolume()
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = node->_nodeType;
        _nodeKind = node->_nodeKind;

        setName(node->getName());
        setBody(node->getBody());
    }

    iNodePhysics::~iNodePhysics()
    {
        if (_bodyID != iPhysicsBody::INVALID_BODY_ID)
        {
            iPhysics::getInstance().destroyBody(_bodyID);
        }
    }

    bool iNodePhysics::isInitialized()
    {
        return _physicsInitialized;
    }

    void iNodePhysics::setBody(uint64 bodyID)
    {
        _bodyID = bodyID;
    }

    uint64 iNodePhysics::getBody() const
    {
        return _bodyID;
    }

    void iNodePhysics::updatePhysics()
    {
        if (!_physicsInitialized)
        {
            initPhysics();
            _physicsInitialized = true;
        }
    }

    void iNodePhysics::onUpdateTransform(iaMatrixf& matrix)
    {
        iNodeVolume::onUpdateTransform(matrix);
    }

};
