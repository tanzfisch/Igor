// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/physics/iPhysicsCollisionConfig.h>

#include <igor/physics/iPhysics.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{
    iaIDGenerator64 iPhysicsCollisionConfig::_idGenerator;

    iPhysicsCollisionConfig::iPhysicsCollisionConfig()
    {
        _id = _idGenerator.getNextID();
    }

    iPhysicsCollisionConfig::iPhysicsCollisionConfig(const iPhysicsCollisionConfig *physicsCollisionConfig)
    {
        _id = _idGenerator.getNextID();
        set(physicsCollisionConfig);
    }

    void iPhysicsCollisionConfig::set(const iPhysicsCollisionConfig *physicsCollisionConfig)
    {
        con_assert_sticky(physicsCollisionConfig != nullptr, "zero pointer");

        _boxes = physicsCollisionConfig->_boxes;
        _spheres = physicsCollisionConfig->_spheres;
        _cones = physicsCollisionConfig->_cones;
        _capsules = physicsCollisionConfig->_capsules;
        _cylinders = physicsCollisionConfig->_cylinders;
        _meshs = physicsCollisionConfig->_meshs;
    }

    uint64 iPhysicsCollisionConfig::getID()
    {
        return _id;
    }

    bool iPhysicsCollisionConfig::isFinalized()
    {
        return _finalized;
    }

    void iPhysicsCollisionConfig::addBox(float32 width, float32 height, float32 depth, const iaMatrixd &offset)
    {
        if (_finalized)
        {
            con_err("finalized already");
            return;
        }

        Box box;
        box._depth = depth;
        box._height = height;
        box._offset = offset;
        box._width = width;
        _boxes.push_back(box);
    }

    void iPhysicsCollisionConfig::addSphere(float32 radius, const iaMatrixd &offset)
    {
        if (_finalized)
        {
            con_err("finalized already");
            return;
        }

        Sphere sphere;
        sphere._offset = offset;
        sphere._radius = radius;
        _spheres.push_back(sphere);
    }

    void iPhysicsCollisionConfig::addCone(float32 radius, float32 height, const iaMatrixd &offset)
    {
        if (_finalized)
        {
            con_err("finalized already");
            return;
        }

        Cone cone;
        cone._height = height;
        cone._offset = offset;
        cone._radius = radius;
        _cones.push_back(cone);
    }

    void iPhysicsCollisionConfig::addCapsule(float32 radius0, float32 radius1, float32 height, const iaMatrixd &offset)
    {
        if (_finalized)
        {
            con_err("finalized already");
            return;
        }

        Capsule capsule;
        capsule._height = height;
        capsule._offset = offset;
        capsule._radius0 = radius0;
        capsule._radius1 = radius1;
        _capsules.push_back(capsule);
    }

    void iPhysicsCollisionConfig::addCylinder(float32 radius0, float32 radius1, float32 height, const iaMatrixd &offset)
    {
        if (_finalized)
        {
            con_err("finalized already");
            return;
        }

        Cylinder cylinder;
        cylinder._height = height;
        cylinder._offset = offset;
        cylinder._radius0 = radius0;
        cylinder._radius1 = radius1;
        _cylinders.push_back(cylinder);
    }

    void iPhysicsCollisionConfig::addMesh(iMeshPtr mesh, int64 faceAttribute, const iaMatrixd &offset)
    {
        if (_finalized)
        {
            con_err("finalized already");
            return;
        }

        if (!mesh->hasRawData())
        {
            con_err("no mesh data");
            return;
        }

        Mesh newMesh;
        newMesh._offset = offset;
        newMesh._faceAttribute = faceAttribute;
        newMesh._mesh = mesh;
        _meshs.push_back(newMesh);
    }

    void iPhysicsCollisionConfig::finalize(uint64 worldID)
    {
        if (_finalized)
        {
            con_err("finalized already");
            return;
        }

        iPhysicsCollision *resultingCollision = nullptr;
        std::vector<iPhysicsCollision *> collisions;

        for (auto box : _boxes)
        {
            collisions.push_back(iPhysics::getInstance().createBox(box._width, box._height, box._depth, box._offset, worldID));
        }

        for (auto sphere : _spheres)
        {
            collisions.push_back(iPhysics::getInstance().createSphere(sphere._radius, sphere._offset, worldID));
        }

        for (auto cone : _cones)
        {
            collisions.push_back(iPhysics::getInstance().createCone(cone._radius, cone._height, cone._offset, worldID));
        }

        for (auto capsule : _capsules)
        {
            collisions.push_back(iPhysics::getInstance().createCapsule(capsule._radius0, capsule._radius1, capsule._height, capsule._offset, worldID));
        }

        for (auto cylinder : _cylinders)
        {
            collisions.push_back(iPhysics::getInstance().createCylinder(cylinder._radius0, cylinder._radius1, cylinder._height, cylinder._offset, worldID));
        }

        for (auto mesh : _meshs)
        {
            iPhysicsCollision *collision = iPhysics::getInstance().createMesh(mesh._mesh, mesh._faceAttribute, mesh._offset, worldID);
            if (collision != nullptr)
            {
                collisions.push_back(collision);
            }
        }

        if (collisions.size() > 0)
        {
            if (collisions.size() > 1)
            {
                resultingCollision = iPhysics::getInstance().createCompound(collisions, worldID);
            }
            else
            {
                resultingCollision = collisions[0];
            }

            _collisionID = resultingCollision->getID();
        }
        else
        {
            con_err("no collision objects defined");
        }

        _finalized = true;
    }

    uint64 iPhysicsCollisionConfig::getCollisionID()
    {
        return _collisionID;
    }

}; // namespace igor
