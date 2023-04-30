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
		// update matrices
		auto &registry = scene->getRegistry();
		auto transformOnlyView = scene->getEntities<iTransformComponent>();

		for (auto entityID : transformOnlyView)
		{
			auto &transform = transformOnlyView.get<iTransformComponent>(entityID);

			transform._worldMatrix.identity();
			transform._worldMatrix.translate(transform._position);
			transform._worldMatrix.rotate(transform._orientation);
			transform._worldMatrix.scale(transform._scale);
		}

		// update hierarchy generations
		auto hierarchyView = scene->getEntities<iHierarchyComponent>();

		for (auto entityID : hierarchyView)
		{
			auto &hierarchy = hierarchyView.get<iHierarchyComponent>(entityID);

			if(hierarchy._childCount != 0)
			{
				continue;
			}

			iEntityID parentID = hierarchy._parent;
			int32 generation = 0;
			hierarchy._generation = generation;

			while (registry.valid(parentID))
			{
				generation++;

				auto &parentHierarchy = hierarchyView.get<iHierarchyComponent>(parentID);

				if(parentHierarchy._generation < generation)
				{
					parentHierarchy._generation = generation;
				}

				parentID = parentHierarchy._parent;
			};
		}
		
		registry.sort<iHierarchyComponent>([&registry](const entt::entity lhs, const entt::entity rhs)
										   {
											   const auto &clhs = registry.get<iHierarchyComponent>(lhs);
											   const auto &crhs = registry.get<iHierarchyComponent>(rhs);

											   return clhs._generation > crhs._generation;
										   });

		registry.sort<iTransformComponent, iHierarchyComponent>();

		// update hierarchy generations
		auto transformHierarchyView = scene->getEntities<iTransformComponent, iHierarchyComponent>();

		for (auto entityID : transformHierarchyView)
		{
			auto [transform, hierarchy] = transformHierarchyView.get<iTransformComponent, iHierarchyComponent>(entityID);

			if (registry.valid(hierarchy._parent))
			{
				iTransformComponent parentTransform = registry.get<iTransformComponent>(hierarchy._parent);
				transform._worldMatrix = parentTransform._worldMatrix * transform._worldMatrix;
			}
		}
	}

} // igor