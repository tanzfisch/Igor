// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iTransformHierarchySystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntity.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{

    iTransformHierarchySystem::iTransformHierarchySystem()
    {
        _transformView = createView<iTransformComponent>();
        _hierarchyView = createView<iHierarchyComponent, iTransformComponent>();
    }

    iEntitySystemStage iTransformHierarchySystem::getStage() const
    {
        return iEntitySystemStage::Update;
    }

	void iTransformHierarchySystem::update(const iaTime &time, iEntityScenePtr scene)
	{
		// update matrices
		for (auto entity : _transformView->getEntities())
		{
			auto transform = entity->getComponent<iTransformComponent>();

			transform->_worldMatrix.identity();
			transform->_worldMatrix.translate(transform->_position);
			transform->_worldMatrix.rotate(transform->_orientation);
			transform->_worldMatrix.scale(transform->_scale);
		}		

		// update hierarchy generations
		for (auto entity : _hierarchyView->getEntities())
		{
			auto hierarchy = entity->getComponent<iHierarchyComponent>();

			const iEntityID &parentID = hierarchy->_parent;
			if(!parentID.isValid())
			{
				continue;
			}

			auto parent = scene->getEntity(parentID);
			auto parentTransform = parent->getComponent<iTransformComponent>();

			if(parentTransform == nullptr)
			{
				continue;
			}

			auto transform = entity->getComponent<iTransformComponent>();
			transform->_worldMatrix = parentTransform->_worldMatrix * transform->_worldMatrix;
		}
	}

} // igor