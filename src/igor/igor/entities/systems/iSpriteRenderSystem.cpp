// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iSpriteRenderSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/renderer/iRenderer.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{
	void iSpriteRenderSystem::update(iEntityScenePtr scene)
	{
		auto &registry = scene->getRegistry();
		registry.sort<iSpriteRendererComponent>([&registry](const entt::entity lhs, const entt::entity rhs) 
		{
			const auto &clhs = registry.get<iSpriteRendererComponent>(lhs);
			const auto &crhs = registry.get<iSpriteRendererComponent>(rhs);
			return clhs._zIndex < crhs._zIndex; 
		});

		auto view = registry.view<iSpriteRendererComponent, iTransformComponent, iActiveComponent>();

		for (auto entityID : view)
		{
			auto [spriteRender, transform] = view.get<iSpriteRendererComponent, iTransformComponent>(entityID);
			iRenderer::getInstance().drawTexturedQuad(transform._worldMatrix, spriteRender._texture, spriteRender._color, true);
		}
	}

} // igor