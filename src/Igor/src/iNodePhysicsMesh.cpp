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
        : iNodeVolume()
    {
        setName(L"iNodePhysicsMesh");
        _nodeType = iNodeType::iNodePhysicsMesh;
        _nodeKind = iNodeKind::Volume;

        setMaterial(iMaterial::INVALID_MATERIAL_ID); // physics should only render for debugging purposes
    }

    iNodePhysicsMesh::iNodePhysicsMesh(iNodePhysicsMesh* node)
        : iNodeVolume()
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

    iNodePhysicsMesh::~iNodePhysicsMesh()
    {
        if (_bodyID != 0)
        {
            iPhysics::getInstance().destroyBody(_bodyID);
        }
    }

    void iNodePhysicsMesh::setBody(uint64 bodyID)
    {
        _bodyID = bodyID;
    }

    uint64 iNodePhysicsMesh::getBody() const
    {
        return _bodyID;
    }

    void iNodePhysicsMesh::setMesh(shared_ptr<iMesh> mesh, int64 faceAttribute, const iaMatrixf& offset)
    {
        _offset = offset;
        _mesh = mesh;
        _faceAttribute = faceAttribute;

        setTransformationDirty();
    }

    void iNodePhysicsMesh::setupPhysics()
    {
        if (_mesh != nullptr)
        {
            iPhysicsCollision* collision = iPhysics::getInstance().createMesh(_mesh, _faceAttribute, _offset);
            _bodyID = iPhysics::getInstance().createBody(collision)->getID();
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

        if (_bodyID != 0)
        {
            iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
            if (body != nullptr)
            {
                body->updateMatrix(_worldMatrix);
            }
            else
            {
                con_warn("lost body");
                _bodyID = 0;
            }
        }
    }

};
