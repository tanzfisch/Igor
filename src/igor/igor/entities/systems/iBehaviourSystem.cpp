// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iBehaviourSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntity.h>

#include <entt.h>

namespace igor
{
	void iBehaviourSystem::update(const iaTime &time, iEntityScenePtr scene)
	{
		auto *registry = static_cast<entt::registry *>(scene->getRegistry());
		auto view = registry->view<iBehaviourComponent, iActiveComponent>();

		for (auto entityID : view)
		{
			auto &behaviour = view.get<iBehaviourComponent>(entityID);

			iEntity entity(static_cast<iEntityID>(entityID), scene);

			for (auto &behaviourData : behaviour._behaviour)
			{
				if (!behaviourData._delegate.isValid())
				{
					continue;
				}

				behaviourData._delegate(entity, behaviourData._userData);
			}
		}
	}

} // igor