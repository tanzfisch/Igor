// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iQuadtreeSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntity.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{
	void iQuadtreeSystem::update(iEntityScenePtr scene)
	{
		auto &registry = scene->getRegistry();
		auto &quadtree = scene->getQuadtree();

		auto viewNoCollision = registry.view<iTransformComponent, iBody2DComponent>(entt::exclude<iCircleCollision2DComponent>);

		for (auto entityID : viewNoCollision)
		{
			auto [transform, object] = viewNoCollision.get<iTransformComponent, iBody2DComponent>(entityID);

			if (object._object == nullptr ||
				object._object->_parent.expired())
			{
				continue;
			}

			const iaVector2d position(transform._position._x, transform._position._y);
			quadtree.update(object._object, position);
		}

		auto view = registry.view<iTransformComponent, iBody2DComponent, iCircleCollision2DComponent>();

		for (auto entityID : view)
		{
			auto [transform, object, circleCollision] = view.get<iTransformComponent, iBody2DComponent, iCircleCollision2DComponent>(entityID);

			if (object._object == nullptr ||
				object._object->_parent.expired())
			{
				continue;
			}

			const iaCircled circle(transform._position._x + circleCollision._offset._x,
								   transform._position._y + circleCollision._offset._y,
								   circleCollision._radius);
			quadtree.update(object._object, circle);
		}
	}

} // igor