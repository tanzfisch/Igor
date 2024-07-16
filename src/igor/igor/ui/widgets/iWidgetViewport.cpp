// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetViewport.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/renderer/iView.h>
#include <igor/renderer/iRenderer.h>
#include <igor/system/iApplication.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

	iWidgetViewport::iWidgetViewport(const iWidgetPtr parent)
		: iWidget(iWidgetType::iWidgetViewport, iWidgetKind::Widget, parent)
	{
		setMinWidth(50);
		setMinHeight(50);

		_view.setClearColorActive(false);
		_view.setPerspective(45.0f);
		_view.setClipPlanes(1.0f, 10000.f);
	}

	void iWidgetViewport::onUpdate()
	{
		iWindowPtr window = iApplication::getInstance().getWindow();
		const float32 width = window->getClientWidth();
		const float32 height = window->getClientHeight();
		const iaRectanglef rect = getActualRect();
		iaRectanglef viewRect(rect._x / width, (height - rect.getBottom()) / height, rect._width / width, rect._height / height);

		_view.setViewportRelative(viewRect);

		iaRectanglei windowRect(0, 0, width, height);
		_view.updateWindowRect(windowRect);
	}

	void iWidgetViewport::draw()
	{
		if (!isVisible())
		{
			return;
		}

		// store current render states
        const iaRectanglei viewport = iRenderer::getInstance().getViewport();
        const iaMatrixd modelMatrix = iRenderer::getInstance().getModelMatrix();
		const iaMatrixd viewMatrix = iRenderer::getInstance().getViewMatrix();
		const iaMatrixd projectionMatrix = iRenderer::getInstance().getProjectionMatrix();
		bool wireframeEnabled = iRenderer::getInstance().isWireframeEnabled();

	 	_view.render(true);

        // restore everything
		iRenderer::getInstance().setWireframeEnabled(wireframeEnabled);
        iRenderer::getInstance().setModelMatrix(modelMatrix);
		iRenderer::getInstance().setViewMatrix(viewMatrix);
        iRenderer::getInstance().setProjectionMatrix(projectionMatrix);
        iRenderer::getInstance().setViewport(viewport);
	}

	iView &iWidgetViewport::getView()
	{
		return _view;
	}

} // namespace igor
