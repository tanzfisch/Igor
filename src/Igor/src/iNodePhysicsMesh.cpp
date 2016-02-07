// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodePhysicsMesh.h>

#include <iScene.h>
#include <iPhysics.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iNodePhysicsMesh::iNodePhysicsMesh()
    {
        setName(L"iNodePhysicsMesh");
        _nodeType = iNodeType::iNodePhysicsMesh;
        _nodeKind = iNodeKind::Physics;

        setMaterial(iMaterial::INVALID_MATERIAL_ID); // physics should only render for debugging purposes
    }

    iNodePhysicsMesh::iNodePhysicsMesh(iNodePhysicsMesh* node)
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = node->_nodeType;
        _nodeKind = node->_nodeKind;

        setName(node->getName());
        setBody(node->getBody());

        _mesh = node->_mesh;
        _faceAttribute = node->_faceAttribute;
        _offset = node->_offset;
    }

    void iNodePhysicsMesh::setMesh(shared_ptr<iMesh> mesh, int64 faceAttribute, const iaMatrixf& offset)
    {
        _offset = offset;
        _mesh = mesh;
        _faceAttribute = faceAttribute;
    }

    void iNodePhysicsMesh::initPhysics()
    {
        if (_mesh != nullptr)
        {
            iPhysicsCollision* collision = iPhysics::getInstance().createMesh(_mesh, _faceAttribute, _offset);
            setBody(iPhysics::getInstance().createBody(collision)->getID());
            iPhysics::getInstance().destroyCollision(collision);
            _mesh = nullptr;

            setTransformationDirty();
        }
    }

    void iNodePhysicsMesh::draw()
    {

    }

    void iNodePhysicsMesh::onUpdateTransform(iaMatrixf& matrix)
    {
        iNodeVolume::onUpdateTransform(matrix);

        if (getBody() != iPhysicsBody::INVALID_BODY_ID)
        {
            iPhysicsBody* body = iPhysics::getInstance().getBody(getBody());
            if (body != nullptr)
            {
                body->updateMatrix(_worldMatrix);
            }
            else
            {
                con_warn("lost body");
                setBody(iPhysicsBody::INVALID_BODY_ID);
            }
        }
    }

};
