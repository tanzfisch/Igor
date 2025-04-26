// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "Viewport.h"

#include "overlay/TransformOverlay.h"
#include "overlay/EmitterOverlay.h"

static const float64 s_wheelSensitivity = 1.2;

Viewport::Viewport()
{
    setTitle("Viewport");
    setHeaderEnabled(false);
    setDockable(true);
    setMoveable(false);
    setIgnoreChildEventConsumption(true);
    setAcceptDrop(true);

    _viewportScene = new iWidgetViewport();
    _viewportScene->setVerticalAlignment(iVerticalAlignment::Stretch);
    _viewportScene->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _viewportScene->getView().setName("Scene");
    _viewportScene->getView().setClearColorActive(false);
    _viewportScene->getView().setPerspective(45.0f);
    _viewportScene->getContextMenuEvent().add(iContextMenuDelegate(this, &Viewport::onContextMenu));
    addWidget(_viewportScene);

    _viewportOverlay = new iWidgetViewport();
    _viewportOverlay->setVerticalAlignment(iVerticalAlignment::Stretch);
    _viewportOverlay->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _viewportOverlay->getView().setName("Overlay");
    _viewportOverlay->getView().registerRenderDelegate(iDrawDelegate(this, &Viewport::renderOverlay));
    _viewportOverlay->getView().setClearColorActive(false);
    _viewportOverlay->getView().setClearDepthActive(true);
    _viewportOverlay->getView().setPerspective(45.0f);
    _viewportOverlay->getView().setClipPlanes(1.0f, 10000.f);
    _viewportOverlay->getView().setEntityScene(iEntitySystemModule::getInstance().createScene("Overlay"));
    addWidget(_viewportOverlay);

    _materialOrientationPlane = iResourceManager::getInstance().loadResource<iShader>("igor_shader_material_orientation_plane");

    _entityOverlays.push_back(std::make_unique<TransformOverlay>(&_viewportOverlay->getView()));
    _entityOverlays.push_back(std::make_unique<EmitterOverlay>(&_viewportOverlay->getView()));

    iProject::getInstance().getProjectUnloadedEvent().add(iProjectUnloadedDelegate(this, &Viewport::onProjectUnloaded));
    iProject::getInstance().getProjectLoadedEvent().add(iProjectLoadedDelegate(this, &Viewport::onProjectLoaded));

    iResourceManager::getInstance().getResourceProcessedEvent().add(iResourceProcessedDelegate(this, &Viewport::onResourceLoaded), false, true);
}

Viewport::~Viewport()
{
    _viewportScene->getView().unregisterRenderDelegate(iDrawDelegate(this, &Viewport::renderScene));
    _viewportOverlay->getView().unregisterRenderDelegate(iDrawDelegate(this, &Viewport::renderOverlay));
}

void Viewport::onChangeCamera(iWidgetPtr source)
{
    iWidgetButtonPtr button = static_cast<iWidgetButtonPtr>(source);
    const iEntityActionContext *actionContext = static_cast<const iEntityActionContext *>(&*button->getActionContext());
    if (actionContext->getEntities().empty())
    {
        return;
    }

    auto entityScene = _viewportScene->getView().getEntityScene();
    if (entityScene == nullptr)
    {
        return;
    }

    auto cameras = entityScene->getCameras();
    for (const auto &camera : cameras)
    {
        if (camera->getID() == actionContext->getEntities()[0])
        {
            camera->setActive(true);
            _viewportOverlay->getView().setOverrideCamera(camera->getID(), entityScene->getID());
        }
        else
        {
            camera->setActive(false);
        }
    }
}

void Viewport::onContextMenu(iWidgetPtr source)
{
    _contextMenu.clear();
    _contextMenu.setPos(iMouse::getInstance().getPos());

    auto entityScene = _viewportScene->getView().getEntityScene();
    if (entityScene == nullptr)
    {
        return;
    }

    auto cameras = entityScene->getCameras();
    if (cameras.size() > 1)
    {
        iWidgetMenuPtr camMenu = new iWidgetMenu("Camera");
        _contextMenu.addMenu(camMenu);

        bool skipFirst = true;
        for (const auto &camera : cameras)
        {
            // skip first because it is the active camera
            if (skipFirst)
            {
                skipFirst = false;
                continue;
            }
            std::vector<iEntityID> cameraID = {camera->getID()};
            iActionContextPtr actionContext = std::make_shared<iEntityActionContext>(entityScene->getID(), cameraID);
            iaString description = iaString("Switch to ") + camera->getName() + " camera";
            camMenu->addCallback(iClickDelegate(this, &Viewport::onChangeCamera), camera->getName(), description, "", true, actionContext);
        }
    }

    if (_contextMenu.hasEntries())
    {
        _contextMenu.open();
    }
}

void Viewport::onProjectLoaded()
{
    auto projectScene = iProject::getInstance().getProjectScene();
    _viewportScene->getView().setEntityScene(projectScene);
    _cameraArc = std::make_unique<CameraArc>(projectScene->getID(), projectScene->getRootEntity()->getID());

    // TODO this should be true by the time the project is loaded
    if (projectScene->getActiveCamera() != nullptr)
    {
        _viewportOverlay->getView().setOverrideCamera(projectScene->getActiveCamera()->getID(), projectScene->getID());
    }
}

void Viewport::onProjectUnloaded()
{
    _viewportScene->getView().setEntityScene(nullptr);
    _cameraArc = nullptr;
}

void Viewport::onResourceLoaded(const iResourceID resourceID)
{
    // TODO remove?
}

void Viewport::setOverlayMode(OverlayMode overlayMode)
{
    _overlayMode = overlayMode;
    updateAcceptance();

    for (auto overlay : _entityOverlays)
    {
        /* if (!overlay->isActive())
         {
             continue;
         }*/

        overlay->setOverlayMode(_overlayMode);
        overlay->setActive(true);
    }
}

OverlayMode Viewport::getOverlayMode() const
{
    return _overlayMode;
}

void Viewport::frameOnSelection()
{
    if (_cameraArc == nullptr)
    {
        return;
    }

    /*auto selection = _workspace->getSelection();

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

    _cameraArc->setCenterOfInterest(sphere._center);

    if (sphere._radius > 0.0f)
    {
        _cameraArc->setDistance(sphere._radius * 4.0f);
    }
    else
    {
        _cameraArc->setDistance(10.0);
    }*/
}

void Viewport::renderScene()
{
    renderSelection();
}

void Viewport::renderSelection()
{
    // TODO
}

void Viewport::renderOverlay()
{
    renderOrientationPlane();
}

void Viewport::renderOrientationPlane()
{
    // TODO this does not render because the camera is not setup correctly
    // we need to use the camera from the other scene

    iaMatrixd identity;
    iRenderer::getInstance().setModelMatrix(identity);

    iRenderer::getInstance().setShader(_materialOrientationPlane);
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

    for (auto &overlay : _entityOverlays)
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
        // TODO unused
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
    for (auto &overlay : _entityOverlays)
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
    for (auto &overlay : _entityOverlays)
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
            // TODO select entity

            /*auto node = getNodeAt(iMouse::getInstance().getPos()._x, iMouse::getInstance().getPos()._y);
            if (node != nullptr)
            {
                _workspace->setSelection({node->getID()});
            }
            else
            {
                _workspace->clearSelection();
            }*/
        }
        return true;
    }

    return false;
}

void Viewport::onMouseMove(iEventMouseMove &event)
{
    iWidget::onMouseMove(event);

    bool result = false;
    for (auto &overlay : _entityOverlays)
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

    // TODO this needs to move in to camera arc
    if (_cameraArc)
    {
        if (iMouse::getInstance().getLeftButton())
        {
            if (iKeyboard::getInstance().getKey(iKeyCode::Alt))
            {
                _cameraArc->setPitch(_cameraArc->getPitch() + (from._y - to._y) * rotateSensitivity);
                _cameraArc->setHeading(_cameraArc->getHeading() + (from._x - to._x) * rotateSensitivity);
            }
        }

        if (iMouse::getInstance().getMiddleButton())
        {
            if (iKeyboard::getInstance().getKey(iKeyCode::Alt))
            {
                iaMatrixd camWorldMatrix;
                _cameraArc->getWorldTransformation(camWorldMatrix);

                iaVector3d fromWorld = camWorldMatrix * _viewportScene->getView().unProject(iaVector3d(from._x, from._y, 0), camWorldMatrix);
                iaVector3d toWorld = camWorldMatrix * _viewportScene->getView().unProject(iaVector3d(to._x, to._y, 0), camWorldMatrix);

                float64 translateFactor = _cameraArc->getDistance() * translateSensitivity;

                auto coi = _cameraArc->getCenterOfInterest();
                coi += (fromWorld - toWorld) * translateFactor;
                _cameraArc->setCenterOfInterest(coi);
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
        if (_cameraArc)
        {
            _cameraArc->setDistance(_cameraArc->getDistance() * s_wheelSensitivity);
        }
    }
    else
    {
        if (_cameraArc)
        {
            _cameraArc->setDistance(_cameraArc->getDistance() * (1.0 / s_wheelSensitivity));
        }
    }

    return true;
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
    return false;
}

void Viewport::updateAcceptance()
{
    if (_selectedEntity != nullptr)
    {
        for (auto overlay : _entityOverlays)
        {
            overlay->setActive(overlay->accepts(_overlayMode, _selectedEntity));
        }
    }
    else
    {
        for (auto overlay : _entityOverlays)
        {
            overlay->setActive(false);
        }
    }
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

void Viewport::onDrop(const iDrag &drag, const iaVector2f &mousePos)
{
    const iMimeData &mimeData = drag.getMimeData();
    if (!mimeData.hasResourceID())
    {
        return;
    }

    // TODO
    /*iResourceID id = mimeData.getResourceID();

    const iaString resourceType = iResourceManager::getInstance().getType(id);
    if (resourceType == IGOR_RESOURCE_MODEL)
    {
        const iaString filename = iResourceManager::getInstance().getFilename(id);

        iParameters parameters({{IGOR_RESOURCE_PARAM_ID, id},
                                {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MODEL},
                                {IGOR_RESOURCE_PARAM_CACHE_MODE, iResourceCacheMode::Cache},
                                {IGOR_RESOURCE_PARAM_JOIN_VERTICES, true},
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
    }*/
}
