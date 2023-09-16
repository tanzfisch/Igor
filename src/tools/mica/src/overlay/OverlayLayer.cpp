#include "OverlayLayer.h"

#include "TransformOverlay.h"
#include "EmitterOverlay.h"

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

    _materialOrientationPlane = iResourceManager::getInstance().loadResource<iMaterial>("igor_material_orientation_plane");

    // font for
    _font = iTextureFont::create("igor_font_default_outline");

    _nodeOverlays.push_back(std::make_unique<TransformOverlay>(&_view, _scene, _workspace));
    _nodeOverlays.push_back(std::make_unique<EmitterOverlay>(&_view, _scene, _workspace));
}

void OverlayLayer::setOverlayMode(OverlayMode overlayMode)
{
    _overlayMode = overlayMode;
    updateAcceptance();

    for (auto overlay : _nodeOverlays)
    {
        if(!overlay->isActive())
        {
            continue;
        }

        overlay->setOverlayMode(_overlayMode);
    }
}

void OverlayLayer::updateAcceptance()
{
    if (_selectedNode != nullptr)
    {
        for (auto overlay : _nodeOverlays)
        {
            overlay->setActive(overlay->accepts(_overlayMode, _selectedNode->getKind(), _selectedNode->getType()));
        }
    }
    else
    {
        for (auto overlay : _nodeOverlays)
        {
            overlay->setActive(overlay->accepts(OverlayMode::None, iNodeKind::Undefined, iNodeType::Undefined));
        }
    }
}

bool OverlayLayer::onSceneSelectionChanged(iEventSceneSelectionChanged &event)
{
    if (!_workspace->getSelection().empty())
    {
        _selectedNode = iNodeManager::getInstance().getNode(_workspace->getSelection()[0]);
    }
    else
    {
        _selectedNode = nullptr;
    }

    updateAcceptance();

    if(_selectedNode == nullptr)
    {
        return false;
    }

    for (auto overlay : _nodeOverlays)
    {
        if(!overlay->isActive())
        {
            continue;
        }
        
        overlay->setNodeID(_selectedNode->getID());
    }    

    return false;
}

OverlayMode OverlayLayer::getOverlayMode() const
{
    return _overlayMode;
}

void OverlayLayer::onDeinit()
{
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
    bool result = false;
    for (auto &overlay : _nodeOverlays)
    {
        if (!overlay->isActive())
        {
            continue;
        }

        if(overlay->onMouseMoveEvent(event))
        {
            result = true;
        }
    }

    return result;
}

bool OverlayLayer::onMouseKeyUpEvent(iEventMouseKeyUp &event)
{
    bool result = false;
    for (auto &overlay : _nodeOverlays)
    {
        if (!overlay->isActive())
        {
            continue;
        }

        if(overlay->onMouseKeyUpEvent(event))
        {
            result = true;
        }
    }

    return result;
}

bool OverlayLayer::onMouseKeyDownEvent(iEventMouseKeyDown &event)
{
    bool result = false;
    for (auto &overlay : _nodeOverlays)
    {
        if (!overlay->isActive())
        {
            continue;
        }

        if(overlay->onMouseKeyDownEvent(event))
        {
            result = true;
        }
    }

    return result;
}

bool OverlayLayer::onKeyDown(iEventKeyDown &event)
{
    bool result = false;
    switch (event.getKey())
    {
    case iKeyCode::Q:
        setOverlayMode(OverlayMode::None);
        result = true;
        break;

    case iKeyCode::W:
        setOverlayMode(OverlayMode::Translate);
        result = true;
        break;

    case iKeyCode::E:
        setOverlayMode(OverlayMode::Rotate);
        result = true;
        break;

    case iKeyCode::R:
        setOverlayMode(OverlayMode::Scale);
        result = true;
        break;
    }

    updateAcceptance();

    for (auto &overlay : _nodeOverlays)
    {
        if (!overlay->isActive())
        {
            continue;
        }

        overlay->onKeyDown(event);
    }

    return result;
}
