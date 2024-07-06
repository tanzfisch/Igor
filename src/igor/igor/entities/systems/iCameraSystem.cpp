// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
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

	iEntitySystemStage iCameraSystem::getStage() const
	{
		return iEntitySystemStage::Update;
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
		con_assert(_cameraView->getEntities().size() < 2, "more then one camera active. this will lead to undefined behaviour");

		auto camera = getActiveCamera();
		if (camera == nullptr)
		{
			return;
		}

		context._renderEngine->setCamera(camera);
	}

} // igor