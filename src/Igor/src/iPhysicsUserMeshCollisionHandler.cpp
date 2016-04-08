// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysicsUserMeshCollisionHandler.h>

#include <iPhysics.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iPhysicsUserMeshCollisionHandler::iPhysicsUserMeshCollisionHandler()
    {
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
        con_trace();
        return 0;
    }

    int iPhysicsUserMeshCollisionHandler::getFacesInAABB(const float32* p0, const float32* p1, const float32** vertexArray, int* vertexCount, int* vertexStrideInBytes, const int* indexList, int maxIndexCount, const int* userDataList)
    {
        con_trace();
        return 0;
    }


}