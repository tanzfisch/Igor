#include "OverlayLayer.h"

OverlayLayer::OverlayLayer(iWindow *window, int32 zIndex, WorkspacePtr workspace)
    : iLayer(window, "Overlay", zIndex), _workspace(workspace)
{
}

OverlayLayer::~OverlayLayer()
{
}

void OverlayLayer::onInit()
{
    // init 3D user controls
    _view.setName("Overlay");
    _view.setClearColor(false);
    _view.setClearDepth(true);
    _view.setPerspective(45.0f);
    _view.setClipPlanes(1.0f, 10000.f);
    _view.registerRenderDelegate(iDrawDelegate(this, &OverlayLayer::render));

    _scene = iSceneFactory::getInstance().createScene();
    _scene->setName("Overlay");
    _view.setScene(_scene);

    getWindow()->addView(&_view, getZIndex());

    _viewOrtho.setName("Overlay Ortho");
    _viewOrtho.setClearColor(false);
    _viewOrtho.setClearDepth(false);
    _viewOrtho.setClipPlanes(-1.0f, 1.0f);
    _viewOrtho.setOrthogonal(0.0f, static_cast<float32>(getWindow()->getClientWidth()), static_cast<float32>(getWindow()->getClientHeight()), 0.0f);
    _viewOrtho.registerRenderDelegate(iDrawDelegate(this, &OverlayLayer::renderOrtho));
    getWindow()->addView(&_viewOrtho, getZIndex() + 1);

    _materialOrientationPlane = iMaterialResourceFactory::getInstance().createMaterial("OrientationPlane");
    auto oriPlaneMaterial = iMaterialResourceFactory::getInstance().getMaterial(_materialOrientationPlane);
    oriPlaneMaterial->setRenderState(iRenderState::Blend, iRenderStateValue::On);
    oriPlaneMaterial->setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);

    // font for
    _font = new iTextureFont("StandardFont.png");

    // _manipulator = new Manipulator(&_window, &_view, _sceneWidget3D);
}

void OverlayLayer::onDeinit()
{
    _view.unregisterRenderDelegate(iDrawDelegate(this, &OverlayLayer::render));
}

void OverlayLayer::render()
{
    renderOrientationPlane();
}

void OverlayLayer::renderOrientationPlane()
{
    iaMatrixd identity;
    iRenderer::getInstance().setModelMatrix(identity);

    iRenderer::getInstance().setMaterial(_materialOrientationPlane);
    iRenderer::getInstance().setLineWidth(1);

    for (int i = -20; i < 21; ++i)
    {
        if (i % 2 == 0)
        {
            iRenderer::getInstance().setColor(1.0f, 1.0f, 1.0f, 0.5f);
        }
        else
        {
            iRenderer::getInstance().setColor(1.0f, 1.0f, 1.0f, 0.25f);
        }

        iRenderer::getInstance().drawLine(iaVector3f(-20.0f, 0.0f, i), iaVector3f(20.0f, 0.0f, i));
        iRenderer::getInstance().drawLine(iaVector3f(i, 0.0f, 20.0f), iaVector3f(i, 0.0f, -20.0f));
    }

    iRenderer::getInstance().setColor(1.0f, 0.0f, 0.0f, 1.0f);
    iRenderer::getInstance().drawLine(iaVector3f(0.0f, 0.0f, 0.0f), iaVector3f(20.0f, 0.0f, 0.0f));
    iRenderer::getInstance().setColor(0.0f, 0.0f, 1.0f, 1.0f);
    iRenderer::getInstance().drawLine(iaVector3f(0.0f, 0.0f, 0.0f), iaVector3f(0.0f, 0.0f, 20.0f));
}

void OverlayLayer::renderOrtho()
{
    // reset matrices
    iaMatrixd identity;
    iRenderer::getInstance().setViewMatrix(identity);
    iRenderer::getInstance().setModelMatrix(identity);

    // render profiler
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
    _profilerVisualizer.draw(getWindow(), _font, iaColor4f(0, 0, 0, 1));
}

void OverlayLayer::onEvent(iEvent &event)
{
    event.dispatch<iKeyDownEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(OverlayLayer::onKeyDown));
    event.dispatch<iWindowResizeEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(OverlayLayer::onWindowResize));
}

bool OverlayLayer::onKeyDown(iKeyDownEvent_TMP &event)
{
    switch (event.getKey())
    {
    case iKeyCode::F8:
        _profilerVisualizer.cycleVerbosity();
        return true;
    }

    return false;
}

bool OverlayLayer::onWindowResize(iWindowResizeEvent_TMP &event)
{
    _viewOrtho.setOrthogonal(0.0, static_cast<float32>(event.getWindow()->getClientWidth()),
                             static_cast<float32>(event.getWindow()->getClientHeight()), 0.0);

    return false;
}