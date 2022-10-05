// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/iScene.h>

#include <igor/scene/nodes/iNode.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/data/iOctree.h>
#include <igor/scene/nodes/iNodeVolume.h>
#include <igor/scene/nodes/iNodeLODSwitch.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/system/iTimer.h>
#include <igor/events/iEventScene.h>
#include <igor/system/iApplication.h>

#include <iaux/system/iaConsole.h>
#include <iaux/system/iaTime.h>
using namespace iaux;

namespace igor
{

    iScene::iScene()
    {
        _root = iNodeManager::getInstance().createNode<iNode>();
        _root->setName(L"RootNode");
        _root->setScene(this);

        _octree = new iOctree(iAACubed(iaVector3d(0, 0, 0), 10000000.0), 10.0, 8, 2);
    }

    iScene::~iScene()
    {
        if (_root != nullptr)
        {
            iNodeManager::getInstance().destroyNode(_root);
            _root = nullptr;
        }

        if (_octree != nullptr)
        {
            delete _octree;
            _octree = nullptr;
        }
    }

    void iScene::registerLODTrigger(iNodeLODTrigger *trigger)
    {
        auto iter = find(_lodTriggers.begin(), _lodTriggers.end(), trigger);
        if (iter != _lodTriggers.end())
        {
            con_err("trigger was already registered");
        }
        else
        {
            _lodTriggers.push_back(trigger);
        }
    }

    void iScene::unregisterLODTrigger(iNodeLODTrigger *trigger)
    {
        auto iter = find(_lodTriggers.begin(), _lodTriggers.end(), trigger);
        if (iter != _lodTriggers.end())
        {
            _lodTriggers.erase(iter);
            return;
        }

        con_err("trigger node was not registered");
    }

    void iScene::registerLODSwitch(iNodeLODSwitch *switchNode)
    {
        auto iter = find(_lodSwitches.begin(), _lodSwitches.end(), switchNode);
        if (iter != _lodSwitches.end())
        {
            con_err("switch node was already registered");
        }
        else
        {
            _lodSwitches.push_back(switchNode);
        }
    }

    void iScene::unregisterLODSwitch(iNodeLODSwitch *switchNode)
    {
        auto iter = find(_lodSwitches.begin(), _lodSwitches.end(), switchNode);
        if (iter != _lodSwitches.end())
        {
            _lodSwitches.erase(iter);
            return;
        }

        con_err("switch node was not registered");
    }

    void iScene::updateLOD()
    {
        for (auto switchNode : _lodSwitches)
        {
            switchNode->update();
        }
    }

    const std::vector<iNodeRenderPtr> &iScene::getRenderables() const
    {
        return _renderables;
    }

    const std::vector<iNodeLightPtr> &iScene::getLights() const
    {
        return _lights;
    }

    const iaString &iScene::getName() const
    {
        return _name;
    }

    void iScene::setName(const iaString &name)
    {
        _name = name;
    }

    iNodePtr iScene::getRoot() const
    {
        return _root;
    }

    void iScene::registerRenderable(iNodeRender *node)
    {
        auto iter = find(_renderables.begin(), _renderables.end(), node);
        if (iter != _renderables.end())
        {
            con_err("this renderable was already registered");
        }
        else
        {
            _renderables.push_back(node);
        }
    }

    void iScene::unregisterRenderable(iNodeRender *node)
    {
        auto iter = find(_renderables.begin(), _renderables.end(), node);
        if (iter != _renderables.end())
        {
            _renderables.erase(iter);
            return;
        }

        con_err("light was not registered");
    }

    void iScene::registerLight(iNodeLight *light)
    {
        auto iter = find(_lights.begin(), _lights.end(), light);
        if (iter != _lights.end())
        {
            con_err("light was already registered");
        }
        else
        {
            _lights.push_back(light);
        }
    }

    void iScene::unregisterLight(iNodeLight *light)
    {
        auto iter = find(_lights.begin(), _lights.end(), light);
        if (iter != _lights.end())
        {
            _lights.erase(iter);
            return;
        }

        con_err("light was not registered");
    }

    void iScene::registerVolume(iNodeVolume *volume)
    {
        auto iter = find(_volumes.begin(), _volumes.end(), volume);
        if (iter != _volumes.end())
        {
            con_err("volume was already registered");
        }
        else
        {
            _volumes.push_back(volume);

            iSphered sphere;
            sphere._center._x = volume->getCenter()._x;
            sphere._center._y = volume->getCenter()._y;
            sphere._center._z = volume->getCenter()._z;
            sphere._radius = volume->getBoundingSphere()._radius;
            _octree->insert(volume, sphere);
        }
    }

    void iScene::unregisterVolume(iNodeVolume *volume)
    {
        auto iter = find(_volumes.begin(), _volumes.end(), volume);
        if (iter != _volumes.end())
        {
            _octree->remove(volume);
            _volumes.erase(iter);
            return;
        }

        con_err("volume was not registered");
    }

    void iScene::updateVolume(iNodeVolume *volume)
    {
        iSphered sphere;
        sphere._center._x = volume->getCenter()._x;
        sphere._center._y = volume->getCenter()._y;
        sphere._center._z = volume->getCenter()._z;
        sphere._radius = volume->getBoundingSphere()._radius;

        _octree->update(volume, sphere);
    }

    void iScene::registerCamera(iNodeCamera *camera)
    {
        con_assert(camera != nullptr, "zero pointer");

        if (camera != nullptr)
        {
            auto iter = find(_cameras.begin(), _cameras.end(), camera->getID());
            if (iter != _cameras.end())
            {
                con_err("camera was already registered in scene " << _name);
            }
            else
            {
                _cameras.push_back(camera->getID());
            }
        }
    }

    void iScene::unregisterCamera(iNodeCamera *camera)
    {
        con_assert(camera != nullptr, "zero pointer");

        if (camera != nullptr)
        {
            auto iter = find(_cameras.begin(), _cameras.end(), camera->getID());
            if (iter != _cameras.end())
            {
                _cameras.erase(iter);
                return;
            }

            con_err("camera " << camera->getName() << " was not registered in scene " << _name);
        }
    }

    void iScene::handle()
    {
        // todo can not stay here. need to reduce update effort per frame. event based would be nice
        updateLOD();
        updateData();
        _updateTransformVisitor.traverseTree(_root);
    }

    void iScene::addToDataUpdateQueue(iNodePtr node)
    {
        con_assert_sticky(node != nullptr, "zero pointer");

        node->_queueToDirtyData = false;

        _mutex.lock();
        _loadingQueue.push_back(node->getID());
        _mutex.unlock();
    }

    void iScene::updateData()
    {
        _mutex.lock();
        _processingQueue.insert(_processingQueue.end(), _loadingQueue.begin(), _loadingQueue.end());
        _loadingQueue.clear();
        _mutex.unlock();

        // stop after 50ms to keep the front end responsive
        iaTime endTime = iaTime::getNow();
        endTime += iaTime::fromMilliseconds(50);

        auto iterP = _processingQueue.begin();
        while (iterP != _processingQueue.end())
        {
            iNodePtr node = iNodeManager::getInstance().getNode((*iterP));
            if (node != nullptr)
            {
                if (node->onUpdateData())
                {
                    iterP = _processingQueue.erase(iterP);
                }
                else
                {
                    iterP++;
                }
            }
            else
            {
                // node was destroyed in the mean time
                iterP = _processingQueue.erase(iterP);
            }

            if (iaTime::getNow() > endTime)
            {
                break;
            }
        }
    }

    iOctreePtr iScene::getOctree() const
    {
        return _octree;
    }

    void iScene::signalNodeAdded(iNodePtr node)
    {
        iApplication::getInstance().onEvent(iEventPtr(new iEventNodeAddedToScene(this, node->getID())));
    }

    void iScene::signalNodeRemoved(iNodePtr node)
    {
        iApplication::getInstance().onEvent(iEventPtr(new iEventNodeRemovedFromScene(this, node->getID())));
    }

    const std::vector<iNodeID> &iScene::getSelection() const
    {
        return _selectedNodes;
    }

    void iScene::setSelection(const std::vector<iNodeID> &selection)
    {
        if (_selectedNodes != selection)
        {
            _selectedNodes = selection;
            iApplication::getInstance().onEvent(iEventPtr(new iEventSceneSelectionChanged(this)));
        }
    }

    void iScene::clearSelection()
    {
        if (!_selectedNodes.empty())
        {
            _selectedNodes.clear();
            iApplication::getInstance().onEvent(iEventPtr(new iEventSceneSelectionChanged(this)));
        }
    }

}; // namespace igor
