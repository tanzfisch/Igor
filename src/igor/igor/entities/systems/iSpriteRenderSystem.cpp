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
			return spriteA->getZIndex() < spriteB->getZIndex(); });

		for (auto entity : entities)
		{
			auto spriteRender = entity->getComponent<iSpriteRenderComponent>();
			auto transformComponent = entity->getComponent<iTransformComponent>();

			const auto &worldMatrix = transformComponent->getWorldMatrix();

			switch (spriteRender->getRenderMode())
			{
			case iSpriteRenderComponent::iRenderMode::Tiled:
			{
				const auto &size = spriteRender->getSize();
				iRenderer::getInstance().drawTexturedQuad(worldMatrix._pos,
														  worldMatrix._right * size._x * 0.5,
														  worldMatrix._top * -size._y * 0.5,
														  spriteRender->getSprite()->getTexture(), spriteRender->getColor(), true, size);
				break;
			}

			case iSpriteRenderComponent::iRenderMode::Simple:
			default:
				iRenderer::getInstance().drawSprite(worldMatrix,
													spriteRender->getSprite(), spriteRender->getFrameIndex(), spriteRender->getSize(), spriteRender->getColor(), true);
				break;
			}
		}
	}

} // igor