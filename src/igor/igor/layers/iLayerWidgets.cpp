// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "iLayerWidgets.h"

#include <igor/ui/iWidgetManager.h>
#include <igor/renderer/iRenderer.h>

namespace igor
{

    // set an increase z index of 1 to make sure the ui is rendered above the background
    iLayerWidgets::iLayerWidgets(iWidgetThemePtr theme, iWindowPtr window, const iaString &name, int32 zIndex)
        : iLayer(window, name, zIndex), _theme(theme)
    {
        iWidgetManager::getInstance().setTheme(_theme);
    }

    void iLayerWidgets::onInit()
    {
        // set up the view
        _view.setName("UI Layer");
        _view.setClearColorActive(false);
        _view.setClearDepthActive(false);
        _view.setOrthogonal(0.0, static_cast<float32>(getWindow()->getClientWidth()), static_cast<float32>(getWindow()->getClientHeight()), 0.0);
        _view.registerRenderDelegate(iDrawDelegate(this, &iLayerWidgets::onRender));
        getWindow()->addView(&_view, 10);

        // update desktop size
        iWidgetManager::getInstance().setDesktopDimensions(getWindow()->getClientWidth(), getWindow()->getClientHeight());
    }

    void iLayerWidgets::onUpdate()
    {
        iWidgetManager::getInstance().onUpdate();
    }

    void iLayerWidgets::onDeinit()
    {
        _view.unregisterRenderDelegate(iDrawDelegate(this, &iLayerWidgets::onRender));

        iWidgetManager::getInstance().setTheme(nullptr);
        
        _theme = nullptr;
    }

    void iLayerWidgets::onEvent(iEvent &event)
    {
        event.dispatch<iEventWindowResize>(IGOR_BIND_EVENT_FUNCTION(iLayerWidgets::onWindowResize));

        iWidgetManager::getInstance().onEvent(event);
    }

    bool iLayerWidgets::onWindowResize(iEventWindowResize &event)
    {
        _view.setOrthogonal(0.0, static_cast<float32>(event.getWindow()->getClientWidth()),
                            static_cast<float32>(event.getWindow()->getClientHeight()), 0.0);

        return false;
    }

    void iLayerWidgets::onRender()
    {
        iaMatrixd identity;
        iRenderer::getInstance().setViewMatrix(identity);

        iaMatrixd modelMatrix;
        modelMatrix.translate(0, 0, -1);
        iRenderer::getInstance().setModelMatrix(modelMatrix);

        // tell the widget manager to draw the widgets
        iWidgetManager::getInstance().draw();
    }

} // namespace igor
