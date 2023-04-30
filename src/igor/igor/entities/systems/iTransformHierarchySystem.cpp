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
		auto &registry = scene->getRegistry();
		auto view = scene->getEntities<iTransformComponent>();

		for (auto entityID : view)
		{
			auto &transform = view.get<iTransformComponent>(entityID);

			if(transform._childCount != 0)
			{
				continue;
			}

			iEntityID parentID = transform._parent;
			int32 generation = 0;
			transform._generation = generation;

			while (registry.valid(parentID))
			{
				generation++;

				auto &parentTransform = view.get<iTransformComponent>(parentID);

				if(parentTransform._generation < generation)
				{
					parentTransform._generation = generation;
				}

				parentID = parentTransform._parent;
			};
		}
		
		registry.sort<iTransformComponent>([&registry](const entt::entity lhs, const entt::entity rhs)
										   {
											   const auto &clhs = registry.get<iTransformComponent>(lhs);
											   const auto &crhs = registry.get<iTransformComponent>(rhs);

											   return clhs._generation > crhs._generation;
										   });

		for (auto entityID : view)
		{
			auto &transform = view.get<iTransformComponent>(entityID);

			transform._worldMatrix.identity();
			transform._worldMatrix.translate(transform._position);
			transform._worldMatrix.rotate(transform._orientation);
			transform._worldMatrix.scale(transform._scale);

			if (registry.valid(transform._parent))
			{
				iTransformComponent parentTransform = registry.get<iTransformComponent>(transform._parent);
				transform._worldMatrix = parentTransform._worldMatrix * transform._worldMatrix;
			}
		}
	}

} // igor