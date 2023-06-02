// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iSpriteRenderSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/renderer/iRenderer.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

#include <entt.h>

namespace igor
{
	void iSpriteRenderSystem::render(iEntityScenePtr scene)
	{
		auto *registry = static_cast<entt::registry *>(scene->getRegistry());
		registry->sort<iSpriteRendererComponent>([registry](const entt::entity lhs, const entt::entity rhs)
												 {
			const auto &clhs = registry->get<iSpriteRendererComponent>(lhs);
			const auto &crhs = registry->get<iSpriteRendererComponent>(rhs);
			return clhs._zIndex < crhs._zIndex; });

		auto view = registry->view<iSpriteRendererComponent, iTransformComponent, iActiveComponent>();

		for (auto entityID : view)
		{
			auto [spriteRender, transform] = view.get<iSpriteRendererComponent, iTransformComponent>(entityID);

			switch (spriteRender._renderMode)
			{
			case iSpriteRenderMode::Tiled:
				iRenderer::getInstance().drawTexturedQuad(transform._worldMatrix._pos,
														  transform._worldMatrix._right * spriteRender._size._x * 0.5,
														  transform._worldMatrix._top * -spriteRender._size._y * 0.5,
														  spriteRender._sprite->getTexture(), spriteRender._color, true, spriteRender._size);
				break;

			case iSpriteRenderMode::Simple:
			default:
				iRenderer::getInstance().drawSprite(transform._worldMatrix,
													spriteRender._sprite, spriteRender._frameIndex, spriteRender._size,
													spriteRender._color, true);
				break;
			}
		}
	}

} // igor