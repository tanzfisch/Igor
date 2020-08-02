// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/layers/iLayerProfiler.h>

#include <igor/graphics/iRenderer.h>
#include <igor/resources/texture/iTextureResourceFactory.h>

namespace igor
{

    // set an increase z index of 1 to make sure the ui is rendered above the background
    iLayerProfiler::iLayerProfiler(iWindow *window, const iaString &name, int32 zIndex, iProfilerVerbosity verbosity)
        : iLayer(window, name, zIndex), _view(iView(false))
    {
        _profilerVisualizer.setVerbosity(verbosity);
    }

    void iLayerProfiler::onInit()
    {
        // set up the view
        _view.setName("Profiler View");
        _view.setClearColor(false);
        _view.setClearDepth(false);
        _view.setOrthogonal(0.0, static_cast<float32>(getWindow()->getClientWidth()),
                            static_cast<float32>(getWindow()->getClientHeight()), 0.0);
        _view.registerRenderDelegate(iDrawDelegate(this, &iLayerProfiler::onRender));
        getWindow()->addView(&_view, getZIndex());

        // init font for render profiler
        _font = new iTextureFont("StandardFont.png");
    }

    void iLayerProfiler::onDeinit()
    {
        // release resources
        if (_font != nullptr)
        {
            delete _font;
            _font = nullptr;
        }

        _view.unregisterRenderDelegate(iDrawDelegate(this, &iLayerProfiler::onRender));
    }

    void iLayerProfiler::onEvent(iEvent &event)
    {
        event.dispatch<iEventWindowResize>(IGOR_BIND_EVENT_FUNCTION(iLayerProfiler::onWindowResize));
        event.dispatch<iEventKeyUp>(IGOR_BIND_EVENT_FUNCTION(iLayerProfiler::onKeyUp));
    }

    bool iLayerProfiler::onKeyUp(iEventKeyUp &event)
    {
        switch (event.getKey())
        {
        case iKeyCode::F8:
            _profilerVisualizer.cycleVerbosity();
            return true;
        }

        return false;
    }

    bool iLayerProfiler::onWindowResize(iEventWindowResize &event)
    {
        _view.setOrthogonal(0.0, static_cast<float32>(event.getWindow()->getClientWidth()),
                            static_cast<float32>(event.getWindow()->getClientHeight()), 0.0);

        return false;
    }

    void iLayerProfiler::onRender()
    {
        // initialize view matrix with identity matrix
        iaMatrixd identity;
        iRenderer::getInstance().setViewMatrix(identity);

        // move scene between near and far plane so be ca actually see what we render
        // any value between near and far plane would do the trick
        iaMatrixd modelMatrix;
        modelMatrix.translate(0, 0, -30);
        iRenderer::getInstance().setModelMatrix(modelMatrix);

        _profilerVisualizer.draw(getWindow(), _font);
    }

} // namespace igor
