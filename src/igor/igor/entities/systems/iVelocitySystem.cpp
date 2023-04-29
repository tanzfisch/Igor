// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iVelocitySystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntity.h>

namespace igor
{
	void iVelocitySystem::update(iEntityScenePtr scene)
	{
		auto &entities = scene->getEntitiesV2<iVelocityComponent, iTransformComponent, iActiveComponent>();
		auto view = scene->getEntities<iVelocityComponent, iTransformComponent, iActiveComponent>();

		for (auto entityID : entities)
		{
			auto [velocity, transform] = view.get<iVelocityComponent, iTransformComponent>(entityID);

			transform._position += velocity._velocity;
			transform._orientation += velocity._angularVelocity;
		}
	}

} // igor