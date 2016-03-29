// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodePhysics.h>

#include <iNodeTransform.h>
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
        _bodyID = node->getBodyID();

        _boxes = node->_boxes;
        _spheres = node->_spheres;
        _cones = node->_cones;
        _capsules = node->_capsules;
        _cylinders = node->_cylinders;
        _upVectorJoints = node->_upVectorJoints;
        _meshs = node->_meshs;
        _mass = node->_mass;
        _materialID = node->_materialID;

        if (node->_applyForceAndTorqueDelegate != nullptr)
        {
            _applyForceAndTorqueDelegate = new iApplyForceAndTorqueDelegate(*(node->_applyForceAndTorqueDelegate));
        }

        setDataDirty();
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

    uint64 iNodePhysics::getBodyID() const
    {
        return _bodyID;
    }

    void iNodePhysics::onUpdateTransform(iaMatrixf& matrix)
    {
        iNodeVolume::onUpdateTransform(matrix);
    }

    void iNodePhysics::clear()
    {
        con_assert(!isInitialized(), "already initialized");

        if (!isInitialized())
        {
            _boxes.clear();
            _spheres.clear();
            _cones.clear();
            _capsules.clear();
            _cylinders.clear();
            _upVectorJoints.clear();

            setDataDirty();
        }
        else
        {
            con_err("cant reset primitives after physics was already initialized");
        }
    }

    void iNodePhysics::addBox(float32 width, float32 height, float32 depth, const iaMatrixf& offset)
    {
        con_assert(!isInitialized(), "already initialized");

        if (!isInitialized())
        {
            Box box;
            box._depth = depth;
            box._height = height;
            box._offset = offset;
            box._width = width;
            _boxes.push_back(box);

            setDataDirty();
        }
    }

    void iNodePhysics::addSphere(float32 radius, const iaMatrixf& offset)
    {
        con_assert(!isInitialized(), "already initialized");

        if (!isInitialized())
        {
            Sphere sphere;
            sphere._offset = offset;
            sphere._radius = radius;
            _spheres.push_back(sphere);

            setDataDirty();
        }
    }

    void iNodePhysics::addCone(float32 radius, float32 height, const iaMatrixf& offset)
    {
        con_assert(!isInitialized(), "already initialized");

        if (!isInitialized())
        {
            Cone cone;
            cone._height = height;
            cone._offset = offset;
            cone._radius = radius;
            _cones.push_back(cone);

            setDataDirty();
        }
    }

    void iNodePhysics::addCapsule(float32 radius, float32 height, const iaMatrixf& offset)
    {
        con_assert(!isInitialized(), "already initialized");

        if (!isInitialized())
        {
            Capsule capsule;
            capsule._height = height;
            capsule._offset = offset;
            capsule._radius = radius;
            _capsules.push_back(capsule);

            setDataDirty();
        }
    }

    void iNodePhysics::addCylinder(float32 radius, float32 height, const iaMatrixf& offset)
    {
        con_assert(!isInitialized(), "already initialized");

        if (!isInitialized())
        {
            Cylinder cylinder;
            cylinder._height = height;
            cylinder._offset = offset;
            cylinder._radius = radius;
            _cylinders.push_back(cylinder);

            setDataDirty();
        }
    }

    void iNodePhysics::addUpVectorJoint(const iaVector3f& upVector)
    {
        con_assert(!isInitialized(), "already initialized");

        if (!isInitialized())
        {
            _upVectorJoints.push_back(upVector);

            setDataDirty();
        }
    }

    void iNodePhysics::addMesh(shared_ptr<iMesh> mesh, int64 faceAttribute, const iaMatrixf& offset)
    {
        con_assert_sticky(!isInitialized(), "already initialized");

        if (!isInitialized())
        {
            Mesh newMesh;
            newMesh._offset = offset;
            newMesh._faceAttribute = faceAttribute;
            newMesh._mesh = mesh;
            _meshs.push_back(newMesh);

            setDataDirty();
        }
    }

    void iNodePhysics::setForceAndTorqueDelegate(iApplyForceAndTorqueDelegate applyForceAndTorqueDelegate)
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

            setDataDirty();
        }
    }

    void iNodePhysics::resetForceAndTorqueDelegate()
    {
        con_assert(!isInitialized(), "already initialized");

        if (!isInitialized())
        {
            if (_applyForceAndTorqueDelegate != nullptr)
            {
                delete _applyForceAndTorqueDelegate;
                _applyForceAndTorqueDelegate = nullptr;
            }

            setDataDirty();
        }
    }

    void iNodePhysics::initPhysics()
    {
        if (_physicsInitialized)
        {
            return;
        }

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

        for (auto mesh : _meshs)
        {
            collisions.push_back(iPhysics::getInstance().createMesh(mesh._mesh, mesh._faceAttribute, mesh._offset));
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
            body->setMaterial(_materialID);

            if (_applyForceAndTorqueDelegate != nullptr)
            {
                body->registerForceAndTorqueDelegate(*_applyForceAndTorqueDelegate);
                delete _applyForceAndTorqueDelegate;
                _applyForceAndTorqueDelegate = nullptr;
            }

            _bodyID = body->getID();

            for (auto upVector : _upVectorJoints)
            {
                iPhysics::getInstance().createUpVectorJoint(body, upVector);
            }

            iPhysics::getInstance().destroyCollision(resultingCollision);

            setDataDirty();
        }
        else
        {
            con_warn("no collision objects defined");
        }

        _physicsInitialized = true;
    }

    void iNodePhysics::onPreSetScene()
    {
        iNodeVolume::onPreSetScene();
    }

    void iNodePhysics::onPostSetScene()
    {
        iNodeVolume::onPostSetScene();

        if (getScene() &&
            isDataDirty())
        {
            getScene()->addToDataUpdateQueue(this);
        }
    }

    float32 iNodePhysics::getMass() const
    {
        return _mass;
    }

    void iNodePhysics::setMass(float32 mass)
    {
        _mass = mass;

        if (_bodyID != iPhysicsBody::INVALID_BODY_ID)
        {
            iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
            if (body != nullptr)
            {
                body->setMass(_mass);
            }
        }
    }

    void iNodePhysics::setMaterial(int64 materialID)
    {
        _materialID = materialID;

        if (_bodyID != iPhysicsBody::INVALID_BODY_ID)
        {
            iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
            if (body != nullptr)
            {
                body->setMaterial(_materialID);
            }
        }
    }

    int64 iNodePhysics::getMaterial() const
    {
        return _materialID;
    }

    bool iNodePhysics::onUpdateData()
    {
        iNodeTransform* nextTransformNode = static_cast<iNodeTransform*>(_parent);
        while (nextTransformNode != nullptr && nextTransformNode->getType() != iNodeType::iNodeTransform)
        {
            nextTransformNode = static_cast<iNodeTransform*>(nextTransformNode->getParent());
        }

        if (nextTransformNode != nullptr &&
            nextTransformNode->getType() == iNodeType::iNodeTransform)
        {
            initPhysics();

            if (_bodyID != iPhysicsBody::INVALID_BODY_ID)
            {
                iPhysicsBody* body = iPhysics::getInstance().getBody(_bodyID);
                iPhysics::getInstance().bindTransformNode(body, nextTransformNode);
            }
            else
            {
                con_err("inconsistand data");
                return false;
            }
        }
        else
        {
            con_warn("need a transform node as ancester");
            return false;
        }

        setTransformationDirty();
        return true;
    }

    void iNodePhysics::draw()
    {
        // nothing to do here
    }

};
