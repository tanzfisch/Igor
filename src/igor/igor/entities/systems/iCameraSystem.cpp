// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iCameraSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntity.h>
#include <igor/renderer/iRenderer.h>

#include <igor/system/iApplication.h>

namespace igor
{
	iCameraSystem::iCameraSystem()
	{
		_cameraView = createView<iCameraComponent, iTransformComponent>();
	}

	iEntitySystemStage iCameraSystem::getStage() const
	{
		return iEntitySystemStage::PreRender;
	}

	void iCameraSystem::update(const iaTime &time, iEntityScenePtr scene)
	{
		// TODO iRenderDebugComponent *debug = registry.try_get<iRenderDebugComponent>(entityID);

		iWindowPtr window = iApplication::getInstance().getWindow();

		int32 clientHeight = window->getClientHeight();
		int32 clientWidth = window->getClientWidth();

		for (auto entity : _cameraView->getEntities()) // makes no sense to iterate here. Let's just safe the "active" camera and just set it
		{
			auto camera = entity->getComponent<iCameraComponent>();

			if (!entity->isActive())
			{
				continue;
			}

			auto transform = entity->getComponent<iTransformComponent>();

			iRenderer::getInstance().setViewport(camera->_viewport._x * clientWidth,
												 camera->_viewport._y * clientHeight,
												 camera->_viewport._width * clientWidth,
												 camera->_viewport._height * clientHeight);

			if (camera->_clearColorActive)
			{
				iRenderer::getInstance().clearColorBuffer(camera->_clearColor);
			}

			if (camera->_clearDepthActive)
			{
				iRenderer::getInstance().clearDepthBuffer(camera->_clearDepth);
			}

			if (camera->_projection == iProjectionType::Perspective)
			{
				iRenderer::getInstance().setPerspective(camera->_fieldOfView, camera->_viewport._width / camera->_viewport._height, camera->_clipNear, camera->_clipFar);
			}
			else
			{
				iRenderer::getInstance().setOrtho(camera->_leftOrtho, camera->_rightOrtho, camera->_bottomOrtho, camera->_topOrtho, camera->_clipNear, camera->_clipFar);
			}

			iRenderer::getInstance().setViewMatrixFromCam(transform->_worldMatrix);

			/*if (debug != nullptr)
			{
				iRenderer::getInstance().setWireframeEnabled(debug->_renderWireframe);
			}*/
		}
	}

} // igor