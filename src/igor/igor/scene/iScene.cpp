// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/iScene.h>

#include <igor/scene/nodes/iNode.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/octree/iOctree.h>
#include <igor/scene/nodes/iNodeVolume.h>
#include <igor/scene/nodes/iNodeLODSwitch.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/resources/profiler/iProfiler.h>
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

		//! \todo octree needs to be of variable size
		// maybe multiple octrees?
		_octree = new iOctree(iAACubed(iaVector3d(0, 0, 0), 10000000.0), 50.0, 8, 4);

		//! \todo how do we handle this when we have more than one scene?
#ifdef USE_VERBOSE_STATISTICS
		_sceneHandleSectionID = iProfiler::getInstance().registerSection("scene:handle", 2);
		_updateLODSectionID = iProfiler::getInstance().registerSection("scene:LOD", 2);
		_processUpdateDataSectionID = iProfiler::getInstance().registerSection("scene:updateData", 2);
		_updateTransformSectionID = iProfiler::getInstance().registerSection("scene:traverse", 2);
#endif
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

	std::vector<iNodeRender *> &iScene::getRenderables()
	{
		return _renderables;
	}

	std::vector<iNodeLight *> &iScene::getLights()
	{
		return _lights;
	}

	iaString iScene::getName() const
	{
		return _name;
	}

	void iScene::setName(const iaString &name)
	{
		_name = name;
	}

	iNodePtr iScene::getRoot()
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
			_octree->insert(volume->getID(), sphere);
		}
	}

	void iScene::unregisterVolume(iNodeVolume *volume)
	{
		auto iter = find(_volumes.begin(), _volumes.end(), volume);
		if (iter != _volumes.end())
		{
			_octree->remove(volume->getID());
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

		_octree->update(volume->getID(), sphere);
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
#ifdef USE_VERBOSE_STATISTICS
		iProfiler::getInstance().beginSection(_sceneHandleSectionID);

		iProfiler::getInstance().beginSection(_updateLODSectionID);
#endif
		// todo can not stay here. need to reduce update effort per frame. event based would be nice
		updateLOD();
#ifdef USE_VERBOSE_STATISTICS
		iProfiler::getInstance().endSection(_updateLODSectionID);

		iProfiler::getInstance().beginSection(_processUpdateDataSectionID);
#endif
		updateData();
#ifdef USE_VERBOSE_STATISTICS
		iProfiler::getInstance().endSection(_processUpdateDataSectionID);

		iProfiler::getInstance().beginSection(_updateTransformSectionID);
#endif
		_updateTransformVisitor.traverseTree(_root);
#ifdef USE_VERBOSE_STATISTICS
		iProfiler::getInstance().endSection(_updateTransformSectionID);

		iProfiler::getInstance().endSection(_sceneHandleSectionID);
#endif
	}

	void iScene::addToDataUpdateQueue(iNodePtr node)
	{
		con_assert_sticky(node != nullptr, "zero pointer");

		node->_queueToDirtyData = false;

		_mutex.lock();
		_loadingQueue.insert(node->getID());
		_mutex.unlock();
	}

	void iScene::updateData()
	{
		_mutex.lock();
		_processingQueue.insert(_loadingQueue.begin(), _loadingQueue.end());
		_mutex.unlock();

		// stop after 50ms to keep the front end responsive
		iaTime endTime = iaTime::now();
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

			if (iaTime::now() > endTime)
			{
				break;
			}
		}
	}

	iOctree *iScene::getOctree()
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

}; // namespace igor
