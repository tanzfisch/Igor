// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iTransformHierarchySystem.h>

#include <igor/entities/iEntityScene.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{
	void iTransformHierarchySystem::update(iEntityScenePtr scene)
	{
		/*registry.sort<relationship>([&registry](const entt::entity lhs, const entt::entity rhs) {
			const auto &clhs = registry.get<relationship>(lhs);
			const auto &crhs = registry.get<relationship>(rhs);
			return crhs.parent == lhs || clhs.next == rhs
				|| (!(clhs.parent == rhs || crhs.next == lhs) && (clhs.parent < crhs.parent || (clhs.parent == crhs.parent && &clhs < &crhs)));
		});*/



		auto view = scene->getEntities<iTransformComponent>();

		/*auto &registry = scene->getRegistry();
		auto group = registry.group<iTransformComponent>();
		
		registry.sort<iTransformComponent>([&registry](const entt::entity lhs, const entt::entity rhs) {
			const auto &clhs = registry.get<iTransformComponent>(lhs);
			const auto &crhs = registry.get<iTransformComponent>(rhs);
			return crhs._parent == lhs || clhs.next == rhs
				|| (!(clhs._parent == rhs || crhs.next == lhs) && (clhs._parent < crhs._parent || (clhs._parent == crhs._parent && &clhs < &crhs)));
		});		*/

		for (auto entityID : view)
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
		}
	}

} // igor