// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iTransformHierarchySystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntity.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

#include <entt.h>

namespace igor
{
	iTransformSystem::iTransformSystem()
	{
		registerType<iTransformComponent>();
	}

	void iTransformSystem::update(const iaTime &time, iEntityScenePtr scene)
	{
		for (auto entity : getEntities())
		{
			auto transform = entity->getComponent<iTransformComponent>();

			transform->_worldMatrix.identity();
			transform->_worldMatrix.translate(transform._position);
			transform->_worldMatrix.rotate(transform._orientation);
			transform->_worldMatrix.scale(transform._scale);
		}
	}

} // igor