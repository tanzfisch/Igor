// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iBehaviourSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntity.h>

namespace igor
{
    iBehaviourSystem::iBehaviourSystem()
    {
		_view = createView<iBehaviourComponent>();
    }

    iEntitySystemStage iBehaviourSystem::getStage() const
    {
        return iEntitySystemStage::Update;
    }

	void iBehaviourSystem::onUpdate(const iEntitySceneUpdateContext &context)
	{
		const auto &entities = _view->getEntities();
		for (const auto entity : entities)
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