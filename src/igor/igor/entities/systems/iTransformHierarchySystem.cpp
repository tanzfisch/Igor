// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iTransformHierarchySystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntity.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{
	void iTransformHierarchySystem::update(iEntityScenePtr scene)
	{
		// TODO this does not work for more then one layer of hierarchy
		// maybe we can traverse the tree set an index on the transform and then order the transforms
		// and only traverse again if the topology changes hmm
		auto &registry = scene->getRegistry();
		registry.sort<iTransformComponent>([&registry](const entt::entity lhs, const entt::entity rhs)
										   {
											   const auto &clhs = registry.get<iTransformComponent>(lhs);
											   const auto &crhs = registry.get<iTransformComponent>(rhs);

											   return crhs._parent == lhs || (!(clhs._parent == rhs) && (clhs._parent < crhs._parent || (clhs._parent == crhs._parent && lhs < rhs)));
										   });

		auto &entities = scene->getEntitiesV2<iTransformComponent>();
		auto view = scene->getEntities<iTransformComponent>();

		for (auto entityID : entities)
		{
			auto &transform = view.get<iTransformComponent>(entityID);

			transform._worldMatrix.identity();
			transform._worldMatrix.translate(transform._position);
			transform._worldMatrix.rotate(transform._orientation);
			transform._worldMatrix.scale(transform._scale);

			if (scene->getRegistry().valid(transform._parent))
			{
				iTransformComponent *parentTransform = scene->getRegistry().try_get<iTransformComponent>(transform._parent);
				if (parentTransform != nullptr)
				{
					transform._worldMatrix = parentTransform->_worldMatrix * transform._worldMatrix;
				}
			}
		}
	}

} // igor