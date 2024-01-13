// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "Viewport.h"

#include "overlay/TransformOverlay.h"
#include "overlay/EmitterOverlay.h"

static const float64 s_wheelSensitivity = 1.2;

Viewport::Viewport(WorkspacePtr workspace)
    : _workspace(workspace)
{
    setHeaderEnabled(false);
    setDockable(true);
    setMoveable(false);
    setIgnoreChildEventConsumption(true);
    setAcceptDrop(true);

    _viewportScene = new iWidgetViewport();
    _viewportScene->setVerticalAlignment(iVerticalAlignment::Stretch);
    _viewportScene->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _viewportScene->getView().setName("Scene");
    _viewportScene->getView().setScene(_workspace->getScene());
    _viewportScene->getView().setCamera(_workspace->getCameraArc()->getCameraNode());
    _viewportScene->getView().registerRenderDelegate(iDrawDelegate(this, &Viewport::renderScene));
    _viewportScene->getView().setClearColorActive(false);
    _viewportScene->getView().setPerspective(45.0f);
    _viewportScene->getView().setClipPlanes(1.0f, 10000.f);
    addWidget(_viewportScene);

    _viewportOverlay = new iWidgetViewport();
    _viewportOverlay->setVerticalAlignment(iVerticalAlignment::Stretch);
    _viewportOverlay->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _viewportOverlay->getView().setName("Overlay");
    _viewportOverlay->getView().setCamera(_workspace->getCameraArc()->getCameraNode());
    _viewportOverlay->getView().registerRenderDelegate(iDrawDelegate(this, &Viewport::renderOverlay));
    _viewportOverlay->getView().setClearColorActive(false);
    _viewportOverlay->getView().setClearDepthActive(true);
    _viewportOverlay->getView().setPerspective(45.0f);
    _viewportOverlay->getView().setClipPlanes(1.0f, 10000.f);

    _overlayScene = iSceneFactory::getInstance().createScene();
    _overlayScene->setName("Overlay");
    _viewportOverlay->getView().setScene(_overlayScene);

    addWidget(_viewportOverlay);

    initScene();

    _materialOrientationPlane = iResourceManager::getInstance().loadResource<iShader>("igor_shader_material_orientation_plane");

    _nodeOverlays.push_back(std::make_unique<TransformOverlay>(&_viewportOverlay->getView(), _overlayScene, _workspace));
    _nodeOverlays.push_back(std::make_unique<EmitterOverlay>(&_viewportOverlay->getView(), _overlayScene, _workspace));
}

Viewport::~Viewport()
{
    iSceneFactory::getInstance().destroyScene(_overlayScene);

    _viewportScene->getView().unregisterRenderDelegate(iDrawDelegate(this, &Viewport::renderScene));
    _viewportOverlay->getView().unregisterRenderDelegate(iDrawDelegate(this, &Viewport::renderOverlay));
}

void Viewport::setOverlayMode(OverlayMode overlayMode)
{
    _overlayMode = overlayMode;
    updateAcceptance();

    for (auto overlay : _nodeOverlays)
    {
        if (!overlay->isActive())
        {
            continue;
        }

        overlay->setOverlayMode(_overlayMode);
    }
}

OverlayMode Viewport::getOverlayMode() const
{
    return _overlayMode;
}

void Viewport::initScene()
{
    // light
    _directionalLightRotate = iNodeManager::getInstance().createNode<iNodeTransform>();
    _directionalLightRotate->setName("directional light rotate");

    _directionalLightTranslate = iNodeManager::getInstance().createNode<iNodeTransform>();
    _directionalLightTranslate->setName("directional light translate");
    _directionalLightTranslate->translate(10000, 10000, 0);

    _lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
    _lightNode->setName("directional light");
    _lightNode->setAmbient(iaColor3f(0.5f, 0.5f, 0.5f));
    _lightNode->setDiffuse(iaColor3f(0.9f, 0.9f, 0.9f));
    _lightNode->setSpecular(iaColor3f(1.0f, 1.0f, 1.0f));

    _workspace->getMicaScene()->insertNode(_directionalLightRotate);
    _directionalLightRotate->insertNode(_directionalLightTranslate);
    _directionalLightTranslate->insertNode(_lightNode);

    // load materials
    _materialCelShading = iResourceManager::getInstance().loadResource<iShader>("igor_shader_material_cellshading_yellow");
    _materialBoundingBox = iResourceManager::getInstance().loadResource<iShader>("igor_shader_material_bounding_box");
}

void Viewport::frameOnSelection()
{
    auto selection = _workspace->getSelection();

    iNodeVisitorBoundings visitorBoundings;
    iaSphered sphere;
    iaSphered selectionSphere;

    for (auto nodeID : selection)
    {
        auto node = iNodeManager::getInstance().getNode(nodeID);
        if (node != nullptr)
        {
            visitorBoundings.traverseTree(node);
            visitorBoundings.getSphere(sphere);
            selectionSphere.merge(sphere);
        }
    }

    _workspace->getCameraArc()->setCenterOfInterest(sphere._center);

    if (sphere._radius > 0.0f)
    {
        _workspace->getCameraArc()->setDistance(sphere._radius * 4.0f);
    }
    else
    {
        _workspace->getCameraArc()->setDistance(10.0);
    }
}

void Viewport::renderScene()
{
    renderSelection();
}

void Viewport::renderSelection()
{
    for (auto nodeID : _workspace->getSelection())
    {
        iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
        if (node == nullptr)
        {
            continue;
        }

        if (node->getKind() != iNodeKind::Renderable &&
            node->getKind() != iNodeKind::Volume)
        {
            continue;
        }

        iNodeRender *renderNode = static_cast<iNodeRender *>(node);
        iaMatrixd matrix = renderNode->getWorldMatrix();
        iRenderer::getInstance().setModelMatrix(matrix);

        if (node->getType() == iNodeType::iNodeMesh)
        {
            iRenderer::getInstance().setShaderMaterial(_materialCelShading);

            iNodeMesh *meshNode = static_cast<iNodeMesh *>(node);
            iRenderer::getInstance().setLineWidth(4);
            iRenderer::getInstance().drawMesh(meshNode->getMesh(), nullptr);
        }
        else
        {
            if (node->getKind() == iNodeKind::Volume)
            {
                iNodeVolume *renderVolume = static_cast<iNodeVolume *>(node);
                iRenderer::getInstance().setShaderMaterial(_materialBoundingBox);

                iAABoxd box = renderVolume->getBoundingBox();
                iRenderer::getInstance().drawBox(box, iaColor4f::yellow);
            }
        }
    }
}

void Viewport::renderOverlay()
{
    renderOrientationPlane();
}

void Viewport::renderOrientationPlane()
{
    iaMatrixd identity;
    iRenderer::getInstance().setModelMatrix(identity);

    iRenderer::getInstance().setShaderMaterial(_materialOrientationPlane);
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

bool Viewport::onKeyDown(iEventKeyDown &event)
{
    if (iWidget::onKeyDown(event))
    {
        return true;
    }

    for (auto &overlay : _nodeOverlays)
    {
        if (!overlay->isActive())
        {
            continue;
        }

        if (overlay->onKeyDown(event))
        {
            return true;
        }
    }

    switch (event.getKey())
    {
    case iKeyCode::Q:
        setOverlayMode(OverlayMode::None);
        return true;

    case iKeyCode::W:
        setOverlayMode(OverlayMode::Translate);
        return true;

    case iKeyCode::E:
        setOverlayMode(OverlayMode::Rotate);
        return true;

    case iKeyCode::R:
        setOverlayMode(OverlayMode::Scale);
        return true;

    case iKeyCode::F:
        frameOnSelection();
        return true;

    case iKeyCode::F9:
    {
        iNodeVisitorPrintTree printTree;
        const auto &selection = _workspace->getSelection();
        for (auto nodeID : selection)
        {
            iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
            printTree.printToConsole(node);
        }
    }
        return true;

    case iKeyCode::F10:
        _viewportScene->getView().setWireframeVisible(!_viewportScene->getView().isWireframeVisible());
        return true;

    case iKeyCode::F11:
        _viewportScene->getView().setOctreeVisible(!_viewportScene->getView().isOctreeVisible());
        return true;

    case iKeyCode::F12:
        _viewportScene->getView().setBoundingBoxVisible(!_viewportScene->getView().isBoundingBoxVisible());
        return true;
    }

    return false;
}

bool Viewport::onMouseKeyDown(iEventMouseKeyDown &event)
{
    iWidget::onMouseKeyDown(event);

    bool result = false;
    for (auto &overlay : _nodeOverlays)
    {
        if (!overlay->isActive())
        {
            continue;
        }

        if (overlay->onMouseKeyDownEvent(event))
        {
            result = true;
            break;
        }
    }

    return result;
}

bool Viewport::onMouseKeyUp(iEventMouseKeyUp &event)
{
    iWidget::onMouseKeyUp(event);

    bool result = false;
    for (auto &overlay : _nodeOverlays)
    {
        if (!overlay->isActive())
        {
            continue;
        }

        if (overlay->onMouseKeyUpEvent(event))
        {
            result = true;
            break;
        }
    }

    if (result)
    {
        return true;
    }

    switch (event.getKey())
    {
    case iKeyCode::MouseLeft:
        if (!iKeyboard::getInstance().getKey(iKeyCode::Alt))
        {
            auto node = getNodeAt(iMouse::getInstance().getPos()._x, iMouse::getInstance().getPos()._y);
            if (node != nullptr)
            {
                _workspace->setSelection({node->getID()});
            }
            else
            {
                _workspace->clearSelection();
            }
        }
        return true;
    }

    return false;
}

void Viewport::onMouseMove(iEventMouseMove &event)
{
    iWidget::onMouseMove(event);

    bool result = false;
    for (auto &overlay : _nodeOverlays)
    {
        if (!overlay->isActive())
        {
            continue;
        }

        if (overlay->onMouseMoveEvent(event))
        {
            result = true;
            break;
        }
    }

    if (result)
    {
        return;
    }

    const float64 rotateSensitivity = 0.0075;
    const float64 translateSensitivity = 1.0;
    const auto from = _lastMousePos;
    const auto to = event.getPosition();

    if (iMouse::getInstance().getLeftButton())
    {
        if (iKeyboard::getInstance().getKey(iKeyCode::Alt))
        {
            _workspace->getCameraArc()->setPitch(_workspace->getCameraArc()->getPitch() + (from._y - to._y) * rotateSensitivity);
            _workspace->getCameraArc()->setHeading(_workspace->getCameraArc()->getHeading() + (from._x - to._x) * rotateSensitivity);
        }
    }

    if (iMouse::getInstance().getMiddleButton())
    {
        if (iKeyboard::getInstance().getKey(iKeyCode::Alt))
        {
            iNodeCameraPtr camera = static_cast<iNodeCameraPtr>(iNodeManager::getInstance().getNode(_workspace->getCameraArc()->getCameraNode()));
            iNodeTransformPtr cameraDistance = static_cast<iNodeTransformPtr>(iNodeManager::getInstance().getNode(_workspace->getCameraArc()->getCameraDistanceNode()));
            if (camera != nullptr &&
                cameraDistance != nullptr)
            {
                iaMatrixd camWorldMatrix;
                camera->calcWorldTransformation(camWorldMatrix);
                iaVector3d fromWorld = camWorldMatrix * _viewportScene->getView().unProject(iaVector3d(from._x, from._y, 0), camWorldMatrix);
                iaVector3d toWorld = camWorldMatrix * _viewportScene->getView().unProject(iaVector3d(to._x, to._y, 0), camWorldMatrix);

                iaMatrixd camTranslateMatrix;
                cameraDistance->getMatrix(camTranslateMatrix);
                float64 translateFactor = camTranslateMatrix._pos.length() * translateSensitivity;

                auto coi = _workspace->getCameraArc()->getCenterOfInterest();
                coi += (fromWorld - toWorld) * translateFactor;
                _workspace->getCameraArc()->setCenterOfInterest(coi);
            }
        }
    }

    _lastMousePos = to;
}

bool Viewport::onMouseWheel(iEventMouseWheel &event)
{
    if (!isEnabled() ||
        !isMouseOver())
    {
        return false;
    }

    if (iWidget::onMouseWheel(event) &&
        !_ignoreChildEventConsumption)
    {
        return true;
    }

    if (event.getWheelDelta() < 0)
    {
        _workspace->getCameraArc()->setDistance(_workspace->getCameraArc()->getDistance() * s_wheelSensitivity);
    }
    else
    {
        _workspace->getCameraArc()->setDistance(_workspace->getCameraArc()->getDistance() * (1.0 / s_wheelSensitivity));
    }

    return true;
}

iNodePtr Viewport::getNodeAt(int32 x, int32 y)
{
    iView &view = _viewportScene->getView();
    const auto &rect = getActualRect();
    return iNodeManager::getInstance().getNode(view.pickColorID(x - rect._x, y - rect._y));
}

void Viewport::setCamera(iNodeID cameraID)
{
    _viewportScene->getView().setCamera(cameraID);
}

iNodeID Viewport::getCamera() const
{
    return _viewportScene->getView().getCamera();
}

void Viewport::draw()
{
    if (!isVisible())
    {
        return;
    }

    iRenderer::getInstance().drawFilledRectangleColored(getActualRect(), iaColor4f::black, iaColor4f::gray, iaColor4f::gray, iaColor4f::black);

    for (const auto child : _children)
    {
        child->draw();
    }
}

bool Viewport::onEvent(iEvent &event)
{
    event.dispatch<iEventSceneSelectionChanged>(IGOR_BIND_EVENT_FUNCTION(Viewport::onSceneSelectionChanged));

    return false;
}

void Viewport::updateAcceptance()
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

bool Viewport::onSceneSelectionChanged(iEventSceneSelectionChanged &event)
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

    if (_selectedNode == nullptr)
    {
        return false;
    }

    for (auto overlay : _nodeOverlays)
    {
        if (!overlay->isActive())
        {
            continue;
        }

        overlay->setNodeID(_selectedNode->getID());
    }

    return false;
}

void Viewport::onDragMove(iDrag &drag, const iaVector2f &mousePos)
{
    const iMimeData &mimeData = drag.getMimeData();
    if (!mimeData.hasResourceID())
    {
        drag.reject();
        return;
    }

    iResourceID id = mimeData.getResourceID();

    const iaString resourceType = iResourceManager::getInstance().getType(id);
    if (resourceType == IGOR_RESOURCE_MODEL)
    {
        drag.accept();
        return;
    }

    if (resourceType == IGOR_RESOURCE_TEXTURE)
    {
        iNodeID nodeID = _viewportScene->getView().pickColorID(iaVector2i(mousePos._x - getActualPosX(),
                                                                          mousePos._y - getActualPosY()));
        if (nodeID != iNode::INVALID_NODE_ID)
        {
            iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
            if (node != nullptr &&
                node->getType() == iNodeType::iNodeMesh)
            {
                drag.accept();
                return;
            }
        }
    }

    drag.reject();
}

void Viewport::onDrop(const iDrag &drag)
{
    const iMimeData &mimeData = drag.getMimeData();
    if (!mimeData.hasResourceID())
    {
        return;
    }

    iResourceID id = mimeData.getResourceID();

    const iaString resourceType = iResourceManager::getInstance().getType(id);
    if (resourceType == IGOR_RESOURCE_MODEL)
    {
        const iaString filename = iResourceManager::getInstance().getFilename(id);

        iParameters parameters({{IGOR_RESOURCE_PARAM_ID, id},
                                {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MODEL},
                                {IGOR_RESOURCE_PARAM_CACHE_MODE, iResourceCacheMode::Cache},
                                {IGOR_RESOURCE_PARAM_JOIN_VERTICES, false},
                                {IGOR_RESOURCE_PARAM_KEEP_MESH, true}});
        iNodeModel *model = iNodeManager::getInstance().createNode<iNodeModel>();
        model->setModel(iResourceManager::getInstance().requestResource<iModel>(parameters));

        // TODO open dialog and ask if this is to be added to root, cam etc.
        // also add option to add by reference or not

        _workspace->getUserScene()->insertNode(model);
        return;
    }

    if (resourceType == IGOR_RESOURCE_TEXTURE)
    {
        const auto mousePos = iMouse::getInstance().getPos();
        iNodeID nodeID = _viewportScene->getView().pickColorID(iaVector2i(mousePos._x - getActualPosX(),
                                                                          mousePos._y - getActualPosY()));
        if (nodeID != iNode::INVALID_NODE_ID)
        {
            iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
            if (node->getType() == iNodeType::iNodeMesh)
            {
                iNodeMeshPtr mesh = static_cast<iNodeMeshPtr>(node);

                iParameters parameters({{IGOR_RESOURCE_PARAM_ID, id},
                                        {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_TEXTURE},
                                        {IGOR_RESOURCE_PARAM_CACHE_MODE, iResourceCacheMode::Cache}});

                mesh->getMaterial()->setTexture(iResourceManager::getInstance().requestResource<iTexture>(parameters), 0);
                return;
            }
        }
    }
}
