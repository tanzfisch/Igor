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

    _materialOrientationPlane = iMaterialResourceFactory::getInstance().createMaterial("OrientationPlane");
    auto oriPlaneMaterial = iMaterialResourceFactory::getInstance().getMaterial(_materialOrientationPlane);
    oriPlaneMaterial->setRenderState(iRenderState::Blend, iRenderStateValue::On);
    oriPlaneMaterial->setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);

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

/*
    case iKeyCode::F8:
        // _profilerVisualizer.cycleVerbosity();
        break;

*/