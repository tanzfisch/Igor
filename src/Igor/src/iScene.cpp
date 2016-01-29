// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iScene.h>

#include <iNode.h>
#include <iNodeModel.h>
#include <iNodeFactory.h>
#include <iOctree.h>
#include <iNodeVolume.h>
#include <iNodeLODSwitch.h>

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
		_octree = new iOctree(iAACubef(iaVector3f(0,0,0), 20000.0f), 50.0f, 8, 4);
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
			_octree->insert(volume->getID());
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
		_octree->update(volume->getID());
	}

	void iScene::registerCamera(iNodeCamera* camera)
	{
        auto iter = find(_cameras.begin(), _cameras.end(), camera);
        if(iter != _cameras.end())
		{
            con_err("camera was already registered");
		}
		else
		{
            _cameras.push_back(camera);
		}
	}

	void iScene::unregisterCamera(iNodeCamera* camera)
	{
        auto iter = find(_cameras.begin(), _cameras.end(), camera);
        if (iter != _cameras.end())
		{
            _cameras.erase(iter);
			return;
		}

		con_err("camera was not registered");
	}

	void iScene::setCamera(iNodeCamera* camera)
	{
        auto iter = find(_cameras.begin(), _cameras.end(), camera);
        if(iter != _cameras.end())
		{
			_camera = camera;
			return;
		}

		con_err("camera is not registered");
	}

	void iScene::handle()
	{
        // todo can't not stay here. need to reduce update effort per frame. event based would be nice
        updateLOD();

        // todo not doing much. so for now it can stay here but ther is no need to do that every frame
        iNodeModel::processModelNodes(); 

		_updateTransformVisitor.traverseTree(_root);
	}

    iOctree* iScene::getOctree()
    {
        return _octree;
    }

    iNodeCamera* iScene::getCamera()
    {
        return _camera;
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
