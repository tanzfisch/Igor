#include "OverlayLayer.h"

OverlayLayer::OverlayLayer(iWindowPtr window, int32 zIndex, WorkspacePtr workspace)
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
    _view.setClearColorActive(false);
    _view.setClearDepthActive(true);
    _view.setPerspective(45.0f);
    _view.setClipPlanes(1.0f, 10000.f);
    _view.registerRenderDelegate(iDrawDelegate(this, &OverlayLayer::render));

    _scene = iSceneFactory::getInstance().createScene();
    _scene->setName("Overlay");
    _view.setScene(_scene);

    getWindow()->addView(&_view, getZIndex());

    // set default camera as current
    _view.setCurrentCamera(_workspace->getCameraArc()->getCameraNode());

    _materialOrientationPlane = iMaterialResourceFactory::getInstance().loadMaterial("igor/materials/orientation_plane.mat");

    // font for
    _font = iTextureFont::create("igor/textures/StandardFontOutlined.png");

    // create the manipulator
    _nodeOverlay = new NodeOverlay(&_view, _scene, _workspace);
}

void OverlayLayer::resetNodeOverlayMode()
{
    setNodeOverlayMode(NodeOverlayMode::None);
}

void OverlayLayer::setNodeOverlayMode(NodeOverlayMode manipulatorMode)
{
    if (!_workspace->getSelection().empty())
    {
        iNodePtr node = iNodeManager::getInstance().getNode(_workspace->getSelection()[0]);
        if (node != nullptr &&
            node->getKind() == iNodeKind::Transformation)
        {
            _nodeOverlay->setVisible(true);
            _nodeOverlay->setNodeOverlayMode(manipulatorMode);

            return;
        }
    }

    _nodeOverlay->setVisible(false);
    _nodeOverlay->setNodeOverlayMode(NodeOverlayMode::None);
}

void OverlayLayer::onDeinit()
{
    if (_nodeOverlay != nullptr)
    {
        delete _nodeOverlay;
        _nodeOverlay = nullptr;
    }

    // release some resources
    _font = nullptr;

    iSceneFactory::getInstance().destroyScene(_scene);

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

    const iaColor4f color1(1.0f, 1.0f, 1.0f, 0.08f);
    const iaColor4f color2(1.0f, 1.0f, 1.0f, 0.04f);

    for (int i = -20; i < 21; ++i)
    {
        iRenderer::getInstance().drawLine(iaVector3f(-20.0f, 0.0f, i), iaVector3f(20.0f, 0.0f, i), i % 2 == 0 ? color1 : color2);
        iRenderer::getInstance().drawLine(iaVector3f(i, 0.0f, 20.0f), iaVector3f(i, 0.0f, -20.0f), i % 2 == 0 ? color1 : color2);
    }

    // TODO put this in top right corner of view
    iRenderer::getInstance().drawLine(iaVector3f(0.0f, 0.0f, 0.0f), iaVector3f(1.0f, 0.0f, 0.0f), iaColor4f::red);
    iRenderer::getInstance().drawLine(iaVector3f(0.0f, 0.0f, 0.0f), iaVector3f(0.0f, 1.0f, 0.0f), iaColor4f::green);
    iRenderer::getInstance().drawLine(iaVector3f(0.0f, 0.0f, 0.0f), iaVector3f(0.0f, 0.0f, 1.0f), iaColor4f::blue);
}

void OverlayLayer::onEvent(iEvent &event)
{
    event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(OverlayLayer::onKeyDown));
    event.dispatch<iEventMouseKeyDown>(IGOR_BIND_EVENT_FUNCTION(OverlayLayer::onMouseKeyDownEvent));
    event.dispatch<iEventMouseKeyUp>(IGOR_BIND_EVENT_FUNCTION(OverlayLayer::onMouseKeyUpEvent));
    event.dispatch<iEventMouseMove>(IGOR_BIND_EVENT_FUNCTION(OverlayLayer::onMouseMoveEvent));
    event.dispatch<iEventSceneSelectionChanged>(IGOR_BIND_EVENT_FUNCTION(OverlayLayer::onSceneSelectionChanged));
}

bool OverlayLayer::onMouseMoveEvent(iEventMouseMove &event)
{
    if (_nodeOverlay->isSelected())
    {
        _nodeOverlay->onMouseMoved(event.getLastPosition(), event.getPosition());
        return true;
    }

    return false;
}

bool OverlayLayer::onMouseKeyUpEvent(iEventMouseKeyUp &event)
{
    switch (event.getKey())
    {
    case iKeyCode::MouseLeft:
        if (_nodeOverlay->isSelected())
        {
            _nodeOverlay->unselect();
            return true;
        }
        break;
    }

    return false;
}

bool OverlayLayer::onMouseKeyDownEvent(iEventMouseKeyDown &event)
{
    switch (event.getKey())
    {
    case iKeyCode::MouseLeft:
        if (!iKeyboard::getInstance().getKey(iKeyCode::Alt))
        {
            _nodeOverlay->select();
            if (_nodeOverlay->isSelected())
            {
                return true;
            }
        }
        break;
    }

    return false;
}

bool OverlayLayer::onSceneSelectionChanged(iEventSceneSelectionChanged &event)
{
    if (!_workspace->getSelection().empty())
    {
        _nodeOverlay->setNodeID(_workspace->getSelection()[0]);
        resetNodeOverlayMode();
    }

    return false;
}

bool OverlayLayer::onKeyDown(iEventKeyDown &event)
{
    switch (event.getKey())
    {
    case iKeyCode::Q:
        setNodeOverlayMode(NodeOverlayMode::None);
        return true;

    case iKeyCode::W:
        setNodeOverlayMode(NodeOverlayMode::Translate);
        return true;

    case iKeyCode::E:
        setNodeOverlayMode(NodeOverlayMode::Rotate);
        return true;

    case iKeyCode::R:
        setNodeOverlayMode(NodeOverlayMode::Scale);
        return true;
    }

    return false;
}
