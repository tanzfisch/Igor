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
		auto view = scene->getEntities<iBaseEntityComponent, iTransformComponent, iSpriteRendererComponent>();

		for (auto entityID : view)
		{
			auto [base, transform, spriteRender] = view.get<iBaseEntityComponent, iTransformComponent, iSpriteRendererComponent>(entityID);

			if (!base._active)
			{
				continue;
			}

			// TODO screen culling?	need cam or view information here

			iRenderer::getInstance().drawTexturedQuad(transform._worldMatrix, spriteRender._texture, spriteRender._color, true);
		}
	}

} // igor