// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iBehaviourSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntity.h>

#include <entt.h>

namespace igor
{
    iBehaviourSystem::iBehaviourSystem()
    {
        registerType<iBehaviourComponent>();
    }

	void iBehaviourSystem::update(const iaTime &time, iEntityScenePtr scene)
	{
		for (auto entity : getEntities())
		{
			auto behaviour = entity->getComponent<iBehaviourComponent>();

			for (auto &behaviourData : behaviour->_behaviors)
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