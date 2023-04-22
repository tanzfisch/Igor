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
		auto view = scene->getEntities<iBehaviourComponent>();
		view.each([scene](auto entityID, auto &behaviour)
				  {
			iEntity entity(entityID, scene);
			for(auto b : behaviour._behaviour)
			{
				if(b != nullptr)
				{
					b(entity);
				}
			} });

		/*for (auto entityID : view)
		{
			auto &transform = view.get<iTransformComponent>(entityID);

			transform._worldMatrix.identity();
			transform._worldMatrix.translate(transform._position);
			transform._worldMatrix.rotate(transform._orientation);
			transform._worldMatrix.scale(transform._scale);

			if (scene->getRegistry().valid(transform._parent))
			{
				iTransformComponent *parentTransform = scene->getRegistry().try_get<iTransformComponent>(transform._parent);
				transform._worldMatrix = parentTransform->_worldMatrix * transform._worldMatrix;
			}
		}*/
	}

} // igor