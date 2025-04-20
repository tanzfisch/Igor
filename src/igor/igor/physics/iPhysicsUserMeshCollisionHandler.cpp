// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/physics/iPhysicsUserMeshCollisionHandler.h>

#include <igor/physics/iPhysics.h>
#include <igor/data/iIntersection.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iPhysicsUserMeshCollisionHandler::iPhysicsUserMeshCollisionHandler()
    {
    }

    void iPhysicsUserMeshCollisionHandler::setAABB(const iaVector3f &min, const iaVector3f &max)
    {
        _bb.setMinMax(min, max);
    }

    void iPhysicsUserMeshCollisionHandler::collideCallback(void *const collideDesc, const void *const continueCollisionHandle)
    {
    }

    float32 iPhysicsUserMeshCollisionHandler::rayHitCallback(void *const rayDesc)
    {
        return 0;
    }

    void iPhysicsUserMeshCollisionHandler::destroyCallback()
    {
    }

    void iPhysicsUserMeshCollisionHandler::getCollisionInfo(void *const infoRecord)
    {
    }

    int iPhysicsUserMeshCollisionHandler::testOverlapAABB(const float64 *const box0, const float64 *const box1)
    {
        iAABoxf bb;
        bb.setMinMax(iaVector3f(box0[0], box0[1], box0[2]), iaVector3f(box1[0], box1[1], box1[2]));
        return iIntersection::intersects(bb, _bb) ? 1 : 0;
    }

    int iPhysicsUserMeshCollisionHandler::getFacesInAABB(const float64 *p0, const float64 *p1, const float64 **vertexArray, int *vertexCount, int *vertexStrideInBytes, const int *indexList, int maxIndexCount, const int *userDataList)
    {
        return 0;
    }

} // namespace igor