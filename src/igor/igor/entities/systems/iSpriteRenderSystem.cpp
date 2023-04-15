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
	}

	iSpriteRenderSystem::~iSpriteRenderSystem()
	{
	}

	void iSpriteRenderSystem::update(iEntityScenePtr scene)
	{
		auto view = scene->getEntities<iBaseEntityComponent, iTransformComponent2D, iSpriteRendererComponent>();

		for (auto entityID : view)
		{
			auto [base, transform, spriteRender] = view.get<iBaseEntityComponent, iTransformComponent2D, iSpriteRendererComponent>(entityID);

			if (!base._active)
			{
				continue;
			}

			// TODO screen culling?	need cam or view information here

			iaMatrixf matrix;
			matrix.translate(transform._position._x, transform._position._y, 0.0);
			matrix.rotate(transform._orientation, iaAxis::Z);
			matrix.scale(transform._scale._x, transform._scale._y, 1.0);

			iRenderer::getInstance().drawTexturedQuad(matrix, spriteRender._texture, spriteRender._color, true);
		}
	}

} // igor