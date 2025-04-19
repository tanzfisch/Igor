// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iSpriteRenderSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/components/iSpriteRenderComponent.h>
#include <igor/entities/components/iTransformComponent.h>
#include <igor/renderer/iRenderer.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{
	iSpriteRenderSystem::iSpriteRenderSystem()
	{
		_view = createView<iTransformComponent, iSpriteRenderComponent>();
	}

    iEntitySystemPtr iSpriteRenderSystem::createInstance()
    {
        return new iSpriteRenderSystem();
    }

    const iaString &iSpriteRenderSystem::getTypeName()
    {
        static const iaString typeName("igor_sprite_render_system");
        return typeName;
    }	

	iEntitySystemStage iSpriteRenderSystem::getStage() const
	{
		return iEntitySystemStage::Render;
	}

	void iSpriteRenderSystem::onUpdate(const iEntitySceneUpdateContext &context)
	{
		auto &entities = _view->getEntities();

		std::sort(entities.begin(), entities.end(), [](iEntityPtr a, iEntityPtr b)
				  {
			auto spriteA = a->getComponent<iSpriteRenderComponent>();
			auto spriteB = b->getComponent<iSpriteRenderComponent>();
			return spriteA->_zIndex < spriteB->_zIndex; });

		for (auto entity : entities)
		{
			if(entity->getName() == "shop")
			{
				con_endl("shop");
			}
			auto spriteRender = entity->getComponent<iSpriteRenderComponent>();
			auto transformComponent = entity->getComponent<iTransformComponent>();

			const auto &worldMatrix = transformComponent->getWorldMatrix();

			switch (spriteRender->_renderMode)
			{
			case iSpriteRenderComponent::iRenderMode::Tiled:
				iRenderer::getInstance().drawTexturedQuad(worldMatrix._pos,
														  worldMatrix._right * spriteRender->_size._x * 0.5,
														  worldMatrix._top * -spriteRender->_size._y * 0.5,
														  spriteRender->_sprite->getTexture(), spriteRender->_color, true, spriteRender->_size);
				break;

			case iSpriteRenderComponent::iRenderMode::Simple:
			default:
				iRenderer::getInstance().drawSprite(worldMatrix,
													spriteRender->_sprite, spriteRender->_frameIndex, spriteRender->_size,
													spriteRender->_color, true);
				break;
			}
		}
	}

} // igor