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
	iSpriteRenderSystem::iSpriteRenderSystem()
	{
		_view = createView<iTransformComponent, iSpriteRendererComponent>();
	}

	iEntitySystemStage iSpriteRenderSystem::getStage() const
    {
        return iEntitySystemStage::Render;
    }

	void iSpriteRenderSystem::update(const iaTime &time, iEntityScenePtr scene)
	{
		auto &entities = _view->getEntities();

		std::sort(entities.begin(), entities.end(), [](iEntityPtr a, iEntityPtr b) {
			auto spriteA = a->getComponent<iSpriteRendererComponent>();
			auto spriteB = b->getComponent<iSpriteRendererComponent>();
			return spriteA->_zIndex < spriteB->_zIndex;
		});

		for (auto entity : entities)
		{
			auto spriteRender = entity->getComponent<iSpriteRendererComponent>();
			auto transform = entity->getComponent<iTransformComponent>();

			switch (spriteRender->_renderMode)
			{
			case iSpriteRendererComponent::iRenderMode::Tiled:
				iRenderer::getInstance().drawTexturedQuad(transform->_worldMatrix._pos,
														  transform->_worldMatrix._right * spriteRender->_size._x * 0.5,
														  transform->_worldMatrix._top * -spriteRender->_size._y * 0.5,
														  spriteRender->_sprite->getTexture(), spriteRender->_color, true, spriteRender->_size);
				break;

			case iSpriteRendererComponent::iRenderMode::Simple:
			default:
				iRenderer::getInstance().drawSprite(transform->_worldMatrix,
													spriteRender->_sprite, spriteRender->_frameIndex, spriteRender->_size,
													spriteRender->_color, true);
				break;
			}
		}
	}

} // igor