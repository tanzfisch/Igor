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
		auto quadtreeView = scene->getEntitiesV2<iTransformComponent, iQuadtreeComponent>();
		auto view = scene->getEntities<iTransformComponent, iQuadtreeComponent>();

		for (auto entityID : quadtreeView)
		{
			auto [transform, object] = view.get<iTransformComponent, iQuadtreeComponent>(entityID);
			const iaVector2d position(transform._position._x, transform._position._y);

			if (object._object == nullptr)
			{
				continue;
			}

			if (object._object->_parent.expired())
			{
				continue;
			}

			scene->getQuadtree().update(object._object, position);
		}
	}

} // igor