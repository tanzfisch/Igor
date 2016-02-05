// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodePhysicsPrimitives.h>

#include <iScene.h>
#include <iPhysics.h>
#include <iPhysicsCollision.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iNodePhysicsPrimitives::iNodePhysicsPrimitives()
    {
        setName(L"iNodePhysicsPrimitives");
        _nodeType = iNodeType::iNodePhysicsPrimitives;
        _nodeKind = iNodeKind::Physics;

        setMaterial(iMaterial::INVALID_MATERIAL_ID); // physics should only render for debugging purposes
    }

    iNodePhysicsPrimitives::iNodePhysicsPrimitives(iNodePhysicsPrimitives* node)
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = node->_nodeType;
        _nodeKind = node->_nodeKind;

        setName(node->getName());
        setBody(node->getBody());

        _boxes = node->_boxes;
        _spheres = node->_spheres;
        _cones = node->_cones;
        _capsules = node->_capsules;
        _cylinders = node->_cylinders;
        _mass = node->_mass;

        if (node->_applyForceAndTorqueDelegate != nullptr)
        {
            _applyForceAndTorqueDelegate = new iApplyForceAndTorqueDelegate(*(node->_applyForceAndTorqueDelegate));
        }
    }

    void iNodePhysicsPrimitives::clear()
    {
        con_assert(!isInitialized(), "already initialized");

        if (!isInitialized())
        {
            _boxes.clear();
            _spheres.clear();
            _cones.clear();
            _capsules.clear();
            _cylinders.clear();
        }
        else
        {
            con_err("cant reset primitives after physics was already initialized");
        }
    }

    void iNodePhysicsPrimitives::addBox(float32 width, float32 height, float32 depth, const iaMatrixf& offset)
    {
        con_assert(!isInitialized(), "already initialized");
        Box box;
        box._depth = depth;
        box._height = height;
        box._offset = offset;
        box._width = width;
        _boxes.push_back(box);
    }

    void iNodePhysicsPrimitives::addSphere(float32 radius, const iaMatrixf& offset)
    {
        con_assert(!isInitialized(), "already initialized");

        if (!isInitialized())
        {
            Sphere sphere;
            sphere._offset = offset;
            sphere._radius = radius;
            _spheres.push_back(sphere);
        }
    }

    void iNodePhysicsPrimitives::addCone(float32 radius, float32 height, const iaMatrixf& offset)
    {
        con_assert(!isInitialized(), "already initialized");

        if (!isInitialized())
        {
            Cone cone;
            cone._height = height;
            cone._offset = offset;
            cone._radius = radius;
            _cones.push_back(cone);
        }
    }

    void iNodePhysicsPrimitives::addCapsule(float32 radius, float32 height, const iaMatrixf& offset)
    {
        con_assert(!isInitialized(), "already initialized");

        if (!isInitialized())
        {
            Capsule capsule;
            capsule._height = height;
            capsule._offset = offset;
            capsule._radius = radius;
            _capsules.push_back(capsule);
        }
    }

    void iNodePhysicsPrimitives::addCylinder(float32 radius, float32 height, const iaMatrixf& offset)
    {
        con_assert(!isInitialized(), "already initialized");

        if (!isInitialized())
        {
            Cylinder cylinder;
            cylinder._height = height;
            cylinder._offset = offset;
            cylinder._radius = radius;
            _cylinders.push_back(cylinder);
        }
    }

    void iNodePhysicsPrimitives::setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate applyForceAndTorqueDelegate)
    {
        con_assert(!isInitialized(), "already initialized");

        if (!isInitialized())
        {
            if (_applyForceAndTorqueDelegate != nullptr)
            {
                delete _applyForceAndTorqueDelegate;
                _applyForceAndTorqueDelegate = nullptr;
            }

            _applyForceAndTorqueDelegate = new iApplyForceAndTorqueDelegate(applyForceAndTorqueDelegate);
        }
    }

    void iNodePhysicsPrimitives::resetForceAndTorqueDelegate()
    {
        con_assert(!isInitialized(), "already initialized");

        if (!isInitialized())
        {
            if (_applyForceAndTorqueDelegate != nullptr)
            {
                delete _applyForceAndTorqueDelegate;
                _applyForceAndTorqueDelegate = nullptr;
            }
        }
    }

    void iNodePhysicsPrimitives::initPhysics()
    {
        int32 count = 0;

        iPhysicsCollision* resultingCollision = nullptr;
        vector<iPhysicsCollision*> collisions;

        for (auto box : _boxes)
        {
            collisions.push_back(iPhysics::getInstance().createBox(box._width, box._height, box._depth, box._offset));
        }

        for (auto sphere : _spheres)
        {
            collisions.push_back(iPhysics::getInstance().createSphere(sphere._radius, sphere._offset));
        }

        for (auto cone : _cones)
        {
            collisions.push_back(iPhysics::getInstance().createCone(cone._radius, cone._height, cone._offset));
        }

        for (auto capsule : _capsules)
        {
            collisions.push_back(iPhysics::getInstance().createCapsule(capsule._radius, capsule._height, capsule._offset));
        }

        for (auto cylinder : _cylinders)
        {
            collisions.push_back(iPhysics::getInstance().createCylinder(cylinder._radius, cylinder._height, cylinder._offset));
        }

        if (collisions.size() > 0)
        {
            if (collisions.size() > 1)
            {
                resultingCollision = iPhysics::getInstance().createCompound(collisions);
            }
            else
            {
                resultingCollision = collisions[0];
            }

            iPhysicsBody* body = iPhysics::getInstance().createBody(resultingCollision);
            body->setMass(_mass);

            if (_applyForceAndTorqueDelegate != nullptr)
            {
                body->registerForceAndTorqueDelegate(*_applyForceAndTorqueDelegate);
                delete _applyForceAndTorqueDelegate;
                _applyForceAndTorqueDelegate = nullptr;
            }

            setBody(body->getID());
            iPhysics::getInstance().destroyCollision(resultingCollision);

            if (_parent != nullptr && 
                _parent->getType() == iNodeType::iNodeTransform)
            {
                iPhysics::getInstance().bindTransformNode(body, reinterpret_cast<iNodeTransform*>(_parent));
            }
            else
            {
                con_err("need a transform node as parent");
            }
        }
        else
        {
            con_warn("no collision objects defined");
        }

        setTransformationDirty();
    }

    float32 iNodePhysicsPrimitives::getMass() const
    {
        return _mass;
    }

    void iNodePhysicsPrimitives::setMass(float32 mass)
    {
        _mass = mass;
    }

    void iNodePhysicsPrimitives::deinitPhysics()
    {
        // nothing to do here
    }

    void iNodePhysicsPrimitives::draw()
    {
        // nothing to do here
    }

};
