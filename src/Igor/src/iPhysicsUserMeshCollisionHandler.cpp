// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysicsUserMeshCollisionHandler.h>

#include <iPhysics.h>
#include <iIntersection.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iPhysicsUserMeshCollisionHandler::iPhysicsUserMeshCollisionHandler()
    {
    }

    void iPhysicsUserMeshCollisionHandler::setAABB(const iaVector3f& min, const iaVector3f& max)
    {
        _bb.set(min, max);
    }

    void iPhysicsUserMeshCollisionHandler::collideCallback(void* const collideDesc, const void* const continueCollisionHandle)
    {
        con_trace();
    }

    float32 iPhysicsUserMeshCollisionHandler::rayHitCallback(void* const rayDesc)
    {
        con_trace();
        return 0;
    }

    void iPhysicsUserMeshCollisionHandler::destroyCallback()
    {
        con_trace();
    }

    void iPhysicsUserMeshCollisionHandler::getCollisionInfo(void* const infoRecord)
    {
        con_trace();
    }

    int iPhysicsUserMeshCollisionHandler::testOverlapAABB(const float32* const box0, const float32* const box1)
    {
        iAABoxf bb;
        bb.set(iaVector3f(box0[0], box0[1], box0[2]), iaVector3f(box1[0], box1[1], box1[2]));
        return iIntersection::intersects(bb, _bb) ? 1 : 0;
    }

    int iPhysicsUserMeshCollisionHandler::getFacesInAABB(const float32* p0, const float32* p1, const float32** vertexArray, int* vertexCount, int* vertexStrideInBytes, const int* indexList, int maxIndexCount, const int* userDataList)
    {
        con_trace();
        return 0;
    }


}