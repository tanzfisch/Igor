// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iAnimationSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntity.h>

#include <entt.h>

namespace igor
{
	void iAnimationSystem::update(iEntityScenePtr scene)
	{
		auto *registry = static_cast<entt::registry *>(scene->getRegistry());
		auto view = registry->view<iAnimationComponent, iTransformComponent>();

		for (auto entityID : view)
		{
			auto &animation = view.get<iAnimationComponent>(entityID);

			iEntity entity(static_cast<iEntityID>(entityID), scene);

			animation._animationController->update(entity);
		}
	}

} // igor