// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysicsCollisionConfig.h>

#include <iPhysics.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    void iPhysicsCollisionConfig::addBox(float32 width, float32 height, float32 depth, const iaMatrixf& offset)
    {
        con_assert(!_finalized, "already finalized");

        if (!_finalized)
        {
            Box box;
            box._depth = depth;
            box._height = height;
            box._offset = offset;
            box._width = width;
            _boxes.push_back(box);
        }
    }

    void iPhysicsCollisionConfig::addSphere(float32 radius, const iaMatrixf& offset)
    {
        con_assert(!_finalized, "already finalized");

        if (!_finalized)
        {
            Sphere sphere;
            sphere._offset = offset;
            sphere._radius = radius;
            _spheres.push_back(sphere);
        }
    }

    void iPhysicsCollisionConfig::addCone(float32 radius, float32 height, const iaMatrixf& offset)
    {
        con_assert(!_finalized, "already finalized");

        if (!_finalized)
        {
            Cone cone;
            cone._height = height;
            cone._offset = offset;
            cone._radius = radius;
            _cones.push_back(cone);
        }
    }

    void iPhysicsCollisionConfig::addCapsule(float32 radius, float32 height, const iaMatrixf& offset)
    {
        con_assert(!_finalized, "already finalized");

        if (!_finalized)
        {
            Capsule capsule;
            capsule._height = height;
            capsule._offset = offset;
            capsule._radius = radius;
            _capsules.push_back(capsule);
        }
    }

    void iPhysicsCollisionConfig::addCylinder(float32 radius, float32 height, const iaMatrixf& offset)
    {
        con_assert(!_finalized, "already finalized");

        if (!_finalized)
        {
            Cylinder cylinder;
            cylinder._height = height;
            cylinder._offset = offset;
            cylinder._radius = radius;
            _cylinders.push_back(cylinder);
        }
    }

    void iPhysicsCollisionConfig::addMesh(shared_ptr<iMesh> mesh, int64 faceAttribute, const iaMatrixf& offset)
    {
        con_assert(!_finalized, "already finalized");

        if (!_finalized)
        {
            Mesh newMesh;
            newMesh._offset = offset;
            newMesh._faceAttribute = faceAttribute;
            newMesh._mesh = mesh;
            _meshs.push_back(newMesh);
        }
    }

    void iPhysicsCollisionConfig::finalize()
    {
        con_assert(!_finalized, "already finalized");
        if (!_finalized)
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

                _collisionID = resultingCollision->getID();
                _finalized = true;
            }
            else
            {
                con_err("no collision objects defined");
            }
        }
    }

    uint64 iPhysicsCollisionConfig::getCollisionID()
    {
        return _collisionID;
    }

};