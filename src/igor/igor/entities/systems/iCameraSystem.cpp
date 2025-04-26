// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iCameraSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/components/iTransformComponent.h>
#include <igor/entities/components/iCameraComponent.h>
#include <igor/renderer/iRenderEngine.h>

namespace igor
{
	iCameraSystem::iCameraSystem()
	{
		_cameraView = createView<iCameraComponent, iTransformComponent>();
	}

    iEntitySystemPtr iCameraSystem::createInstance()
    {
        return new iCameraSystem();
    }

    const iaString &iCameraSystem::getTypeName()
    {
        static const iaString typeName("igor_camera_system");
        return typeName;
    }	

	iEntitySystemStage iCameraSystem::getStage() const
	{
		return iEntitySystemStage::PreRender;
	}

	iEntityPtr iCameraSystem::getActiveCamera() const
	{
		if (_cameraView->getEntities().empty())
		{
			return nullptr;
		}

		return _cameraView->getEntities().front();
	}

	std::vector<iEntityPtr> iCameraSystem::getCameras() const
	{
		std::vector<iEntityPtr> result;

		for (auto entity : _cameraView->getEntities())
		{
			result.push_back(entity);
		}

		for (auto entity : _cameraView->getInactiveEntities())
		{
			result.push_back(entity);
		}

		return result;
	}

	void iCameraSystem::onUpdate(const iEntitySceneUpdateContext &context)
	{
		auto camera = getActiveCamera();
		if (camera == nullptr)
		{
			return;
		}

		context._renderEngine->setCamera(camera->getID());
	}

} // igor