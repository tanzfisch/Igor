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
                iSphered camSphere;
                camera->getWorldMatrix(camMatrix);
                camSphere._center.set(camMatrix._pos._x, camMatrix._pos._y, camMatrix._pos._z);
                camSphere._radius = 50.0; // something is completely wrong with the box sizes of volume nodes

                cullScene(camSphere);
            }
            else
            {
                con_warn("no camera specified");
            }
        }
    }

    void iPhysicsManager::cullScene(const iSphered& sphere)
    {
        _scene->getOctree()->resetFilter();
        _scene->getOctree()->filter(sphere, iNodeKind::Physics);
        _scene->getOctree()->getResult(_cullResult);

        for (auto node : _cullResult)
        {
            iNodePhysics* physicsNode = static_cast<iNodePhysics*>(node);
            physicsNode->updatePhysics();
        }
    }

}