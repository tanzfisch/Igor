// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iScene.h>

#include <iNode.h>
#include <iNodeModel.h>
#include <iNodeFactory.h>
#include <iOctree.h>
#include <iNodeVolume.h>
#include <iNodeLODSwitch.h>
#include <iNodeCamera.h>
#include <iStatistics.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
	
	iScene::iScene()
	{
		_root = iNodeFactory::getInstance().createNode(iNodeType::iNode);
		_root->setName(L"RootNode");
		_root->setScene(this);
        
		//! \todo octree needs to be of variable size
        // maybe multiple octrees?
		_octree = new iOctree(iAACubed(iaVector3d(0,0,0), 40000.0), 50.0, 8, 4);

        _updateLODSectionID = iStatistics::getInstance().registerSection("LOD", iaColor4f(1, 0, 0, 1), 2);
        _processUpdateDataSectionID = iStatistics::getInstance().registerSection("MN", iaColor4f(0, 1, 0, 1), 2);
        _updateTransformSectionID = iStatistics::getInstance().registerSection("uTrans", iaColor4f(0, 0, 1, 1), 2);
	}

	iScene::~iScene()
	{
		if (_root != nullptr)
		{
			iNodeFactory::getInstance().destroyNode(_root);
            _root = nullptr;
		}

        if (_octree != nullptr)
        {
            delete _octree;
            _octree = nullptr;
        }
	}

    void iScene::registerLODTrigger(iNodeLODTrigger* trigger)
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

    void iScene::unregisterLODTrigger(iNodeLODTrigger* trigger)
    {
        auto iter = find(_lodTriggers.begin(), _lodTriggers.end(), trigger);
        if (iter != _lodTriggers.end())
        {
            _lodTriggers.erase(iter);
            return;
        }

        con_err("trigger node was not registered");
    }

    void iScene::registerLODSwitch(iNodeLODSwitch* switchNode)
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

    void iScene::unregisterLODSwitch(iNodeLODSwitch* switchNode)
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

    vector<iNodeRender*>& iScene::getRenderables()
    {
        return _renderables;
    }

    vector<iNodeLight*>& iScene::getLights()
    {
        return _lights;
    }

	iaString iScene::getName() const
	{
		return _name;
	}

	void iScene::setName(const iaString& name)
	{
		_name = name;
	}
		
	iNode* iScene::getRoot()
	{
		return _root;
	}

    void iScene::registerRenderable(iNodeRender* node)
    {
        auto iter = find(_renderables.begin(), _renderables.end(), node);
        if(iter != _renderables.end())
        {
            con_err("this renderable was already registered");
        }
        else
        {
            _renderables.push_back(node);
        }
    }

    void iScene::unregisterRenderable(iNodeRender* node)
    {
        auto iter = find(_renderables.begin(), _renderables.end(), node);
        if(iter != _renderables.end())
        {
            _renderables.erase(iter);
            return;
        }

        con_err("light was not registered");
    }

	void iScene::registerLight(iNodeLight* light)
	{
        auto iter = find(_lights.begin(), _lights.end(), light);
        if(iter != _lights.end())
		{
			con_err("light was already registered");
		}
		else
		{
            _lights.push_back(light);
		}
	}

	void iScene::unregisterLight(iNodeLight* light)
	{
        auto iter = find(_lights.begin(), _lights.end(), light);
        if(iter != _lights.end())
		{
            _lights.erase(iter);
			return;
		}

		con_err("light was not registered");
	}

	void iScene::registerVolume(iNodeVolume* volume)
	{
        auto iter = find(_volumes.begin(), _volumes.end(), volume);
        if(iter != _volumes.end())
		{
			con_err("volume was already registered");
		}
		else
		{
            _volumes.push_back(volume);

            iSpheref sphere;
            sphere._center = volume->getCenter();
            sphere._radius = volume->getBoundingSphere()._radius;
			_octree->insert(volume->getID(), sphere);
		}
	}

	void iScene::unregisterVolume(iNodeVolume* volume)
	{
        auto iter = find(_volumes.begin(), _volumes.end(), volume);
        if(iter != _volumes.end())
		{
			_octree->remove(volume->getID());
            _volumes.erase(iter);
			return;
		}

		con_err("volume was not registered");
	}

	void iScene::updateVolume(iNodeVolume* volume)
	{
        iSpheref sphere;
        sphere._center = volume->getCenter();
        sphere._radius = volume->getBoundingSphere()._radius;

		_octree->update(volume->getID(), sphere);
	}

	void iScene::registerCamera(iNodeCamera* camera)
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

	void iScene::unregisterCamera(iNodeCamera* camera)
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

	void iScene::setCamera(iNodeCamera* camera)
	{
        con_assert(camera != nullptr, "zero pointer");

        if (camera != nullptr)
        {
            auto iter = find(_cameras.begin(), _cameras.end(), camera->getID());
            if (iter != _cameras.end())
            {
                _cameraID = camera->getID();
                return;
            }

            con_err("camera " << camera->getName() << " is not registered in scene " << _name);
        }
	}

    void iScene::setCamera(uint32 cameraID)
    {
        auto iter = find(_cameras.begin(), _cameras.end(), cameraID);
        if (iter != _cameras.end())
        {
            _cameraID = cameraID;
        }
        else
        {
            con_err("camera " << cameraID << " is not registered in scene " << _name);
        }
    }

	void iScene::handle()
	{
        iStatistics::getInstance().beginSection(_updateLODSectionID);
        // todo can't not stay here. need to reduce update effort per frame. event based would be nice
        updateLOD();
        iStatistics::getInstance().endSection(_updateLODSectionID);

        iStatistics::getInstance().beginSection(_processUpdateDataSectionID);
        updateData(); 
        iStatistics::getInstance().endSection(_processUpdateDataSectionID);

        iStatistics::getInstance().beginSection(_updateTransformSectionID);
        _updateTransformVisitor.traverseTree(_root);
        iStatistics::getInstance().endSection(_updateTransformSectionID);
	}

    void iScene::addToDataUpdateQueue(iNode* node)
    {
        con_assert_sticky(node != nullptr, "zero pointer");

        node->_queueToDirtyData = false;
        _dataUpdateQueue.addNode(node->getID());
    }

    void iScene::updateData()
    {
        _dataUpdateQueue.process();
    }

    iOctree* iScene::getOctree()
    {
        return _octree;
    }

    uint32 iScene::getCamera()
    {
        return _cameraID;
    }

    void iScene::signalNodeAdded(uint32 nodeID)
    {
        _addedNode(nodeID);
    }

    void iScene::signalNodeRemoved(uint32 nodeID)
    {
        _removedNode(nodeID);
    }

    void iScene::registerAddedNodeDelegate(iAddedNodeDelegate addedNodeDelegate)
    {
        _addedNode.append(addedNodeDelegate);
    }

    void iScene::unregisterAddedNodeDelegate(iAddedNodeDelegate addedNodeDelegate)
    {
        _addedNode.remove(addedNodeDelegate);
    }

    void iScene::registerRemovedNodeDelegate(iRemovedNodeDelegate removedNodeDelegate)
    {
        _removedNode.append(removedNodeDelegate);
    }

    void iScene::unregisterRemovedNodeDelegate(iRemovedNodeDelegate removedNodeDelegate)
    {
        _removedNode.remove(removedNodeDelegate);
    }

};
