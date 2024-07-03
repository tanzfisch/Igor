// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iQuadtreeSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/components/iBody2DComponent.h>
#include <igor/entities/components/iTransformComponent.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{
	iQuadtreeSystem::iQuadtreeSystem()
	{
		_positionView = createView<iTransformComponent, iBody2DComponent>();
		_circleView = createView<iTransformComponent, iBody2DComponent, iCircleCollision2DComponent>();
	}

    iEntitySystemStage iQuadtreeSystem::getStage() const
    {
        return iEntitySystemStage::Update;
    }

	void iQuadtreeSystem::onUpdate(const iEntitySceneUpdateContext &context)
	{
		iEntityScenePtr scene = context._scene;
		
		if(!scene->hasQuadtree())
		{
			return;
		}

		auto &quadtree = scene->getQuadtree();

		for (auto entity : _positionView->getEntities())
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

		for (auto entity : _circleView->getEntities())
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
