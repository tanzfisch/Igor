// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iLightSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/components/iLightComponent.h>
#include <igor/entities/components/iTransformComponent.h>
#include <igor/renderer/iRenderer.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{
	iLightSystem::iLightSystem()
	{
		_view = createView<iTransformComponent, iLightComponent>();
	}

    iEntitySystemPtr iLightSystem::createInstance()
    {
        return new iLightSystem();
    }

    const iaString &iLightSystem::getTypeName()
    {
        static const iaString typeName("igor_light_system");
        return typeName;
    }	

	iEntitySystemStage iLightSystem::getStage() const
	{
		return iEntitySystemStage::Render;
	}

	void iLightSystem::onUpdate(const iEntitySceneUpdateContext &context)
	{
		auto &entities = _view->getEntities();

		int lightIndex = 0;

		for (auto entity : entities)
		{
			auto lightComponent = entity->getComponent<iLightComponent>();
			auto transformComponent = entity->getComponent<iTransformComponent>();

			const auto &worldMatrix = transformComponent->getWorldMatrix();

			iRenderer::getInstance().setLightAmbient(lightIndex, lightComponent->getAmbient());
			iRenderer::getInstance().setLightDiffuse(lightIndex, lightComponent->getDiffuse());
			iRenderer::getInstance().setLightSpecular(lightIndex, lightComponent->getSpecular());
			iRenderer::getInstance().setLightPosition(lightIndex, worldMatrix._pos);

			lightIndex++;

			break; // currently only one light supported
		}
	}

} // igor