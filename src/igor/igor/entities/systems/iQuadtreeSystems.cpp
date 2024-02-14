// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iQuadtreeSystems.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntity.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{
	iQuadtreeUpdatePositionSystem::iQuadtreeUpdatePositionSystem()
	{
		registerType<iTransformComponent>();
		registerType<iBody2DComponent>();
	}

	void iQuadtreeUpdatePositionSystem::update(const iaTime &time, iEntityScenePtr scene)
	{
		con_assert(scene->hasQuadtree(), "no quadtree");
		auto &quadtree = scene->getQuadtree();

		for (auto entity : getEntities())
		{
			iTransformComponent *transform = entity->getComponent<iTransformComponent>();
			iBody2DComponent *body = entity->getComponent<iBody2DComponent>();

			if (body->_object == nullptr ||
				body->_object->_parent.expired())
			{
				continue;
			}

			const iaVector2d position(transform->_position._x, transform->_position._y);
			quadtree.update(body->_object, position);
		}
	}

	iQuadtreeUpdateCirclesSystem::iQuadtreeUpdateCirclesSystem()
	{
		registerType<iTransformComponent>();
		registerType<iBody2DComponent>();
		registerType<iCircleCollision2DComponent>();
	}

	void iQuadtreeUpdateCirclesSystem::update(const iaTime &time, iEntityScenePtr scene)
	{
		con_assert(scene->hasQuadtree(), "no quadtree");
		auto &quadtree = scene->getQuadtree();

		for (auto entity : getEntities())
		{
			iTransformComponent *transform = entity->getComponent<iTransformComponent>();
			iBody2DComponent *body = entity->getComponent<iBody2DComponent>();
			iCircleCollision2DComponent *circleCollision = entity->getComponent<iCircleCollision2DComponent>();

			if (body->_object == nullptr ||
				body->_object->_parent.expired())
			{
				continue;
			}

			const iaCircled circle(transform->_position._x + circleCollision->_offset._x,
								   transform->_position._y + circleCollision->_offset._y,
								   circleCollision->_radius);
			quadtree.update(body->_object, circle);
		}
	}

} // igor
