// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iBehaviourSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntity.h>

namespace igor
{
	void iBehaviourSystem::update(iEntityScenePtr scene)
	{
		auto view = scene->getEntities<iBehaviourComponent, iActiveComponent>();

		for (auto entityID : view)
		{
			const auto &behaviour = view.get<iBehaviourComponent>(entityID);

			iEntity entity(entityID, scene);

			for (auto behaviourData : behaviour._behaviour)
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