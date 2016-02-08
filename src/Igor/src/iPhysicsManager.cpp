// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysicsManager.h>

#include <iNodeCamera.h>
#include <iScene.h>
#include <iOctree.h>
#include <iPhysicsManagerTask.h>
#include <iTaskManager.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iPhysicsManager::iPhysicsManager()
    {
#if UPDATE_ASYNC == 1
        iTaskManager::getInstance().registerTaskFinishedDelegate(iTaskFinishedDelegate(this, &iPhysicsManager::onTaskFinished));
#endif
    }

    iPhysicsManager::~iPhysicsManager()
    {
#if UPDATE_ASYNC == 1
        iTaskManager::getInstance().unregisterTaskFinishedDelegate(iTaskFinishedDelegate(this, &iPhysicsManager::onTaskFinished));
#endif
    }

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
        if (_scene != nullptr)
        {
            iNodeCamera* camera = _scene->getCamera();

            if (camera != nullptr)
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

    void iPhysicsManager::onTaskFinished(uint64 taskID)
    {
        if (_taskID != taskID)
        {
            _taskID = 0;
        }
    }

    void iPhysicsManager::cullScene(const iSphered& sphere)
    {
        if (_taskID == 0)
        {
            vector<uint32> cullResult;

            _scene->getOctree()->resetFilter();
            _scene->getOctree()->filter(sphere, iNodeKind::Physics);
            _scene->getOctree()->getResult(cullResult);

#if UPDATE_ASYNC == 1
            iTask* task = new iPhysicsManagerTask(cullResult);
            _taskID = task->getID();
            iTaskManager::getInstance().addTask(task);
#else
            for (auto node : cullResult)
            {
                iNodePhysics* physicsNode = static_cast<iNodePhysics*>(node);
                physicsNode->updatePhysics();
            }
#endif
        }
    }

}