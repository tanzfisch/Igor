// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iMeshRenderSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/components/iMeshRenderComponent.h>
#include <igor/entities/components/iTransformComponent.h>
#include <igor/renderer/iRenderEngine.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{
	iMeshRenderSystem::iMeshRenderSystem()
	{
		_componentMask = iEntity::calcComponentMask({typeid(iTransformComponent), typeid(iMeshRenderComponent)});
	}

	iEntitySystemStage iMeshRenderSystem::getStage() const
	{
		return iEntitySystemStage::PreRender;
	}

	void iMeshRenderSystem::onUpdate(const iEntitySceneUpdateContext &context)
	{
		auto scene = context._scene;
		if (!scene->hasOctree())
		{
			return;
		}

		auto renderEngine = context._renderEngine;
		const auto &frustum = renderEngine->getFrustum();

		// frustum culling
		auto octree = scene->getOctree();
		iOctreed::Objects objects;
		octree.query(frustum, objects);

		for (const auto &object : objects)
		{
			auto entityID = std::any_cast<iEntityID>(object->_userData);
			auto entity = scene->getEntity(entityID);
			
			const auto match = _componentMask & entity->getComponentMask();
			if (match == _componentMask)
			{
				renderEngine->addMesh(entity);
			}
		}
	}

} // igor