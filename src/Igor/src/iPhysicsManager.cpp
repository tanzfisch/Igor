// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysicsManager.h>

#include <iNodeCamera.h>
#include <iScene.h>
#include <iOctree.h>
#include <iNodePhysicsMesh.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    void iPhysicsManager::setScene(iScene* scene)
    {
        _scene = scene;
    }

    iScene* iPhysicsManager::getScene()
    {
        return _scene;
    }

    void iPhysicsManager::update()
    {
        if (_scene)
        {
            iNodeCamera* camera = _scene->getCamera();
            if (camera)
            {
                iaMatrixf camMatrix;
                iSpheref camSphere;
                camera->getWorldMatrix(camMatrix);
                camSphere._center = camMatrix._pos;
                camSphere._radius = 50.0f; // something is completely wrong with the box sizes of volume nodes

                cullScene(camSphere);
            }
            else
            {
                con_warn("no camera specified");
            }
        }
    }

    void iPhysicsManager::cullScene(const iSpheref& sphere)
    {
        _scene->getOctree()->resetFilter();
        _scene->getOctree()->filter(sphere);
        _scene->getOctree()->getResult(_cullResult);

        for (auto node : _cullResult)
        {
            if (node->getType() == iNodeType::iNodePhysicsMesh)
            {
                iNodePhysicsMesh* physicsMesh = static_cast<iNodePhysicsMesh*>(node);
                physicsMesh->setupPhysics();
            }
        }
    }

}