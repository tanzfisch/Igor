// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntity.h>

#include <iPhysicsBody.h>
#include <iPhysics.h>
#include <iNodeTransform.h>
using namespace Igor;

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    uint32 iEntity::getID()
    {
        return _id;
    }

    void iEntity::setBody(iPhysicsBody* body)
    {
        _body = body;

        body->setEntity(this);
        iPhysics::getInstance().bindEntity(body, this);
    }

    void iEntity::setTransformNode(iNodeTransform* node)
    {
        _node = node;
    }

    void iEntity::onHandle()
    {
        updatePhysics();
        updateScene();
    }

    iNodeTransform* iEntity::getTransformNode()
    {
        return _node;
    }

    void iEntity::updatePhysics()
    {
        if (_dirtyMatrix)
        {
            if (nullptr != _body)
            {
                _body->updateMatrix(_matrix);
            }

            _dirtyMatrix = false;
        }
    }

    void iEntity::updateScene()
    {
        if (nullptr != _node)
        {
            if (_dirtyPhysics)
            {
                _node->setMatrix(_matrix);
                _dirtyPhysics = false;
            }
        }
    }

    bool iEntity::hasBody()
    {
        return (nullptr != _body) ? true : false;
    }

    bool iEntity::hasNode()
    {
        return (nullptr != _node) ? true : false;
    }

    void iEntity::updateMatrix(const iaMatrixf& matrix)
    {
        _matrix = matrix;
        _dirtyPhysics = true;
    }

    iPhysicsBody* iEntity::getBody() const
    {
        return _body;
    }

    void iEntity::setBool(uint32 index, bool value)
    {
        _customBools[index] = value;
    }

    void iEntity::setFloat(uint32 index, float32 value)
    {
        _customFloats[index] = value;
    }

    void iEntity::setInt(uint32 index, int32 value)
    {
        _customInts[index] = value;
    }

    bool iEntity::getBool(uint32 index)
    {
        con_assert(_customBools.end() != _customBools.find(index), "id does not exist");
        return _customBools[index]; 
    }

    float32 iEntity::getFloat(uint32 index)
    {
        con_assert(_customFloats.end() != _customFloats.find(index), "id does not exist");
        return _customFloats[index];
    }

    int32 iEntity::getInt(uint32 index)
    {
        con_assert(_customInts.end() != _customInts.find(index), "id does not exist");
        return _customInts[index];
    }

    void iEntity::setMatrix(const iaMatrixf& matrix)
    {
        _matrix = matrix;
        _dirtyMatrix = true;
    }

    void iEntity::getMatrix(iaMatrixf& matrix)
    {
        matrix = _matrix;
    }

    void iEntity::setPosition(const iaVector3f& position)
    {
        _matrix._pos = position;
        _dirtyMatrix = true;
    }

    iaVector3f iEntity::getPosition()
    {
        return _matrix._pos;
    }

    void iEntity::setOrientation(const iaVector3f& orientation)
    {
        _matrix._depth.set(0, 0, 0);
        _matrix._top.set(0, 0, 0);
        _matrix._right.set(0, 0, 0);

        _matrix.rotate(orientation._x, iaAxis::X);
        _matrix.rotate(orientation._y, iaAxis::Y);
        _matrix.rotate(orientation._z, iaAxis::Z);

        _dirtyMatrix = true;
    }

    iaVector3f iEntity::getOrientation()
    {
        con_err("not implemented");
        return iaVector3f();
    }

    void iEntity::setVelocity(const iaVector3f& velocity)
    {
        _velocity = velocity;
        _dirtyVelocity = true;
    }

    iaVector3f iEntity::getVelocity()
    {
        return _velocity;
    }

    void iEntity::setAngularVelocity(const iaVector3f& angularVelocity)
    {
        _angularVelocity = angularVelocity;
        _dirtyAngularVelocity = true;
    }

    iaVector3f iEntity::getAngularVelocity()
    {
        return _angularVelocity;
    }

    void iEntity::setForce(const iaVector3f& force)
    {
        _force = force;
        _dirtyForce = true;
    }

    iaVector3f iEntity::getForce()
    {
        return _force;
    }

    void iEntity::setAngularForce(const iaVector3f& angularForce)
    {
        _angularForce = angularForce;
        _dirtyAngularForce = true;
    }

    iaVector3f iEntity::getAngularForce()
    {
        return _angularForce;
    }
    
    void iEntity::resetDirty()
    {
        _dirtyMatrix = false;
        _dirtyAngularVelocity = false;
        _dirtyVelocity = false;
        _dirtyForce = false;
        _dirtyAngularForce = false;
    }

    bool iEntity::isDirtyVelocity()
    {
        return _dirtyVelocity;
    }

    bool iEntity::isDirtyAngularVelocity()
    {
        return _dirtyAngularVelocity;
    }

    bool iEntity::isDirtyForce()
    {
        return _dirtyForce;
    }

    bool iEntity::isDirtyAngularForce()
    {
        return _dirtyAngularForce;
    }

    bool iEntity::isDirtyMatrix()
    {
        return _dirtyMatrix;
    }

}