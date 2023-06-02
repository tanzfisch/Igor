// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iQuadtreeSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntity.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

#include <entt.h>

namespace igor
{
	void iQuadtreeSystem::update(const iaTime &time, iEntityScenePtr scene)
	{
		auto *registry = static_cast<entt::registry*>(scene->getRegistry());
		auto &quadtree = scene->getQuadtree();

		auto viewNoCollision = registry->view<iTransformComponent, iBody2DComponent>(entt::exclude<iCircleCollision2DComponent>);

		for (auto entityID : viewNoCollision)
		{
			auto [transform, body] = viewNoCollision.get<iTransformComponent, iBody2DComponent>(entityID);

			if (body._object == nullptr ||
				body._object->_parent.expired())
			{
				continue;
			}

			const iaVector2d position(transform._position._x, transform._position._y);
			quadtree.update(body._object, position);
		}

		auto view = registry->view<iTransformComponent, iBody2DComponent, iCircleCollision2DComponent>();

		for (auto entityID : view)
		{
			auto [transform, body, circleCollision] = view.get<iTransformComponent, iBody2DComponent, iCircleCollision2DComponent>(entityID);

			if (body._object == nullptr ||
				body._object->_parent.expired())
			{
				continue;
			}

			const iaCircled circle(transform._position._x + circleCollision._offset._x,
								   transform._position._y + circleCollision._offset._y,
								   circleCollision._radius);
			quadtree.update(body._object, circle);
		}
	}

} // igor