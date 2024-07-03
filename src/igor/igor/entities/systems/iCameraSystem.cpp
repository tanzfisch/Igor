// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iCameraSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/components/iTransformComponent.h>
#include <igor/entities/components/iCameraComponent.h>
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
		return iEntitySystemStage::Render;
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

		auto entity = getActiveCamera();
		if (entity == nullptr)
		{
			return;
		}

		// TODO instead of setting the camera values pass the cam values to the render engine so it can be used in mesh render system for culling

		iWindowPtr window = iApplication::getInstance().getWindow();

		int32 clientWidth = window->getClientWidth();
		int32 clientHeight = window->getClientHeight();

		auto camera = entity->getComponent<iCameraComponent>();
		auto viewport = camera->getViewport();

		iaRectanglei rect;
		rect.setX(viewport.getX() * static_cast<float32>(clientWidth) + 0.5f);
		rect.setY(viewport.getY() * static_cast<float32>(clientHeight) + 0.5f);
		rect.setWidth(viewport.getWidth() * static_cast<float32>(clientWidth) + 0.5f);
		rect.setHeight(viewport.getHeight() * static_cast<float32>(clientHeight) + 0.5f);
		iRenderer::getInstance().setViewport(rect);

		if (camera->isClearColorActive())
		{
			iRenderer::getInstance().clearColorBuffer(camera->getClearColor());
		}

		if (camera->isClearDepthActive())
		{
			iRenderer::getInstance().clearDepthBuffer(camera->getClearDepth());
		}

		if (camera->getProjectionType() == iProjectionType::Perspective)
		{
			iRenderer::getInstance().setPerspective(camera->getFieldOfView(),
													camera->getAspectRatio(),
													camera->getNearClipPlane(),
													camera->getFarClipPlane());
		}
		else
		{
			iRenderer::getInstance().setOrtho(camera->getLeftOrtho(),
											  camera->getRightOrtho(),
											  camera->getBottomOrtho(),
											  camera->getTopOrtho(),
											  camera->getNearClipPlane(),
											  camera->getFarClipPlane());
		}

		auto transform = entity->getComponent<iTransformComponent>();
		iRenderer::getInstance().setViewMatrixFromCam(transform->_worldMatrix);
	}

} // igor