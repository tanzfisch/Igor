// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "iLayerWidgets.h"

#include <igor/ui/iWidgetManager.h>
#include <igor/graphics/iRenderer.h>

namespace igor
{

    // set an increase z index of 1 to make sure the ui is rendered above the background
    iLayerWidgets::iLayerWidgets(iWidgetTheme *theme, iWindow *window, const iaString &name, int32 zIndex)
        : iLayer(window, name, zIndex), _theme(theme), _view(iView(false))
    {
        iWidgetManager::getInstance().setTheme(_theme);
    }

    void iLayerWidgets::onInit()
    {
        // set up the view
        _view.setClearColor(false);
        _view.setClearDepth(false);
        _view.setOrthogonal(0.0, static_cast<float32>(getWindow()->getClientWidth()), static_cast<float32>(getWindow()->getClientHeight()), 0.0);
        _view.registerRenderDelegate(iDrawDelegate(this, &iLayerWidgets::onRender));
        getWindow()->addView(&_view, 10);

        // update desktop size
        iWidgetManager::getInstance().setDesktopDimensions(getWindow()->getClientWidth(), getWindow()->getClientHeight());
    }

    void iLayerWidgets::onPreDraw()
    {
        iWidgetManager::getInstance().onPreDraw();
    }

    void iLayerWidgets::onDeinit()
    {
        _view.unregisterRenderDelegate(iDrawDelegate(this, &iLayerWidgets::onRender));

        iWidgetManager::getInstance().setTheme(nullptr);

        if (_theme != nullptr)
        {
            delete _theme;
            _theme = nullptr;
        }
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
        // initialize view matrix with identity matrix
        iaMatrixd identity;
        iRenderer::getInstance().setViewMatrix(identity);

        // move scene between near and far plane so be ca actually see what we render
        // any value between near and far plane would do the trick
        iaMatrixd modelMatrix;
        modelMatrix.translate(0, 0, -30);
        iRenderer::getInstance().setModelMatrix(modelMatrix);

        // tell the widget manager to draw the widgets
        iWidgetManager::getInstance().draw();
    }

} // namespace igor
