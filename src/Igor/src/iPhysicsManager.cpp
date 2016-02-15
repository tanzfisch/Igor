// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysicsManager.h>

#include <iNodeCamera.h>
#include <iNodePhysics.h>
#include <iScene.h>
#include <iOctree.h>
#include <iPhysicsManagerTask.h>
#include <iTaskManager.h>
#include <iNodeFactory.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iPhysicsManager::iPhysicsManager()
    {
        iTaskManager::getInstance().registerTaskFinishedDelegate(iTaskFinishedDelegate(this, &iPhysicsManager::onTaskFinished));
    }

    iPhysicsManager::~iPhysicsManager()
    {
        iTaskManager::getInstance().unregisterTaskFinishedDelegate(iTaskFinishedDelegate(this, &iPhysicsManager::onTaskFinished));
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
                camSphere._radius = 50.0;

                cullScene(camSphere); // BIG TODO
            }
            else
            {
                con_warn("no camera specified");
            }
        }
    }

    void iPhysicsManager::onTaskFinished(uint64 taskID)
    {
        auto iter = _inWork.begin();
        while (iter != _inWork.end())
        {
            if ((*iter).second == taskID)
            {
                _inWork.erase(iter);
                return;
            }
            iter++;
        }
    }

    void iPhysicsManager::cullScene(const iSphered& sphere)
    {
        vector<uint32> cullResult;
        vector<uint32> updateList;

        _scene->getOctree()->resetFilter();
        _scene->getOctree()->filter(sphere);
        _scene->getOctree()->getResult(cullResult);

        for (auto id : cullResult)
        {
            iNode* node = iNodeFactory::getInstance().getNode(id);
            if (node != nullptr &&
                node->getKind() == iNodeKind::Physics)
            {
                auto workingIter = _inWork.find(id);
                if (workingIter == _inWork.end())
                {
                    iNodePhysics* nodePhysics = static_cast<iNodePhysics*>(node);
                    if (!nodePhysics->isInitialized())
                    {
                        _inWork[id] = 0;
                        updateList.push_back(id);
                    }
                }
            }
        }

        for (auto id : updateList)
        {
            iTask* task = new iPhysicsManagerTask(id);
            _inWork[id] = task->getID();
            iTaskManager::getInstance().addTask(task);
        }
    }


}