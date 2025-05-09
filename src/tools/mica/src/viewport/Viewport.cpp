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

    auto boxLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);
    boxLayout->setVerticalAlignment(iVerticalAlignment::Stretch);
    boxLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    boxLayout->setStretchIndex(1);

    _viewportScene = new iWidgetViewport();
    _viewportScene->setVerticalAlignment(iVerticalAlignment::Stretch);
    _viewportScene->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _viewportScene->getView().setName("Scene");
    _viewportScene->getView().getRenderEvent().add(iRenderDelegate(this, &Viewport::renderScene));
    _viewportScene->getView().setClearColorActive(false);
    _viewportScene->getView().setPerspective(45.0f);

    _viewportOverlay = new iWidgetViewport();
    _viewportOverlay->setVerticalAlignment(iVerticalAlignment::Stretch);
    _viewportOverlay->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _viewportOverlay->getView().setName("Overlay");
    _viewportOverlay->getView().getRenderEvent().add(iRenderDelegate(this, &Viewport::renderOverlay));
    _viewportOverlay->getView().setClearColorActive(false);
    _viewportOverlay->getView().setClearDepthActive(true);
    _viewportOverlay->getView().setPerspective(45.0f);
    _viewportOverlay->getView().setClipPlanes(1.0f, 10000.f);
    auto overlayScene = iEntitySystemModule::getInstance().createScene("Overlay");
    overlayScene->initializeOctree(iAACubed(iaVector3d(), 1000000000));
    _overlaySceneID = overlayScene->getID();
    iEntitySystemModule::getInstance().activateScene(overlayScene);
    _viewportOverlay->getView().setEntityScene(_overlaySceneID);
    _viewportOverlay->getContextMenuEvent().add(iContextMenuDelegate(this, &Viewport::onContextMenu));

    _entityOverlays.push_back(std::make_unique<TransformOverlay>(&_viewportOverlay->getView()));
    _entityOverlays.push_back(std::make_unique<EmitterOverlay>(&_viewportOverlay->getView()));

    _materialOrientationPlane = iResourceManager::getInstance().loadResource<iShader>("igor_shader_material_orientation_plane");

    iResourceManager::getInstance().getResourceProcessedEvent().add(iResourceProcessedDelegate(this, &Viewport::onResourceLoaded), false, true);

    _materialCelShading = iResourceManager::getInstance().loadResource<iShader>("igor_shader_material_cellshading_yellow");
    _materialBoundingBox = iResourceManager::getInstance().loadResource<iShader>("igor_shader_material_bounding_box");

    auto buttonLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, boxLayout);
    _buttonGrid = new iWidgetButton();
    _buttonGrid->setIcon("igor_icon_grid");
    _buttonGrid->setTooltip("On/Off orientation grid [F9]");
    _buttonGrid->setMinSize(24, 24);
    _buttonGrid->getClickEvent().add(iClickDelegate(this, &Viewport::onGridClick));
    _buttonGrid->setCheckable(true);
    _buttonGrid->setChecked(_renderOrientationPlane);
    buttonLayout->addWidget(_buttonGrid);

    _buttonBounds = new iWidgetButton();
    _buttonBounds->setIcon("igor_icon_bounds");
    _buttonBounds->setTooltip("On/Off mesh bounds [F12]");
    _buttonBounds->setMinSize(24, 24);
    _buttonBounds->getClickEvent().add(iClickDelegate(this, &Viewport::onBoundsClick));
    _buttonBounds->setCheckable(true);
    _buttonBounds->setChecked(_viewportScene->getView().isBoundingBoxVisible());
    buttonLayout->addWidget(_buttonBounds);

    boxLayout->addWidget(_viewportScene);
    _viewportScene->addWidget(_viewportOverlay);
}

Viewport::~Viewport()
{
    _viewportScene->getView().getRenderEvent().remove(iRenderDelegate(this, &Viewport::renderScene));
    _viewportOverlay->getView().getRenderEvent().remove(iRenderDelegate(this, &Viewport::renderOverlay));
    iEntitySystemModule::getInstance().destroyScene(_overlaySceneID);
}

void Viewport::onGridClick(iWidgetPtr source)
{
    _renderOrientationPlane = !_renderOrientationPlane;
    _buttonGrid->setChecked(_renderOrientationPlane);
}

void Viewport::onBoundsClick(iWidgetPtr source)
{
    _viewportScene->getView().setBoundingBoxVisible(!_viewportScene->getView().isBoundingBoxVisible());
    _buttonBounds->setChecked(_viewportScene->getView().isBoundingBoxVisible());
}

void Viewport::onChangeCamera(iWidgetPtr source)
{
    iWidgetButtonPtr button = static_cast<iWidgetButtonPtr>(source);
    const iEntityActionContext *actionContext = static_cast<const iEntityActionContext *>(&*button->getActionContext());
    if (actionContext->getEntities().empty())
    {
        return;
    }

    const auto &entitySceneID = _viewportScene->getView().getEntitySceneID();
    auto entityScene = iEntitySystemModule::getInstance().getScene(entitySceneID);
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
            _viewportOverlay->getView().setOverrideCamera(camera->getID(), entitySceneID);
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

    const auto &entitySceneID = _viewportScene->getView().getEntitySceneID();
    auto entityScene = iEntitySystemModule::getInstance().getScene(entitySceneID);
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
            const auto &entitySceneID = _viewportScene->getView().getEntitySceneID();
            iActionContextPtr actionContext = std::make_shared<iEntityActionContext>(entitySceneID, cameraID);
            iaString description = iaString("Switch to ") + camera->getName() + " camera";
            camMenu->addCallback(iClickDelegate(this, &Viewport::onChangeCamera), camera->getName(), description, "", true, actionContext);
        }
    }

    if (_contextMenu.hasEntries())
    {
        _contextMenu.open();
    }
}

void Viewport::onSelectionChanged(const iEntitySceneID &sceneID, const std::vector<iEntityID> &entities)
{
    if(entities.size() != 1)
    {
        return;
    }

    for (auto overlay : _entityOverlays)
    {
        overlay->setEntity(sceneID, entities[0]);
    }
}

bool Viewport::onProjectLoaded(iEventProjectLoaded &event)
{
    auto projectScene = iProject::getInstance().getProjectScene();
    if (projectScene == nullptr)
    {
        return false;
    }

    projectScene->getEntitySelectionChangedEvent().add(iEntitySelectionChangedDelegate(this, &Viewport::onSelectionChanged));

    _viewportScene->getView().setEntityScene(projectScene->getID());
    _cameraArc = std::make_unique<CameraArc>(projectScene->getID(), projectScene->getRootEntity()->getID());

    if (projectScene->getActiveCamera() != nullptr)
    {
        _viewportOverlay->getView().setOverrideCamera(projectScene->getActiveCamera()->getID(), projectScene->getID());
    }
    else
    {
        // TODO this is a workarround for now until active camera returns the right camera after loading the project

        auto entities = projectScene->getEntities();
        for (const auto &entity : entities)
        {
            if (!entity->isActive() ||
                entity->getComponent<iCameraComponent>() == nullptr ||
                entity->getComponent<iTransformComponent>() == nullptr)
            {
                continue;
            }

            _viewportOverlay->getView().setOverrideCamera(entity->getID(), projectScene->getID());

            break;
        }
    }

    return false;
}

bool Viewport::onProjectUnloaded(iEventProjectUnloaded &event)
{
    _viewportScene->getView().setEntityScene(iEntitySceneID::getInvalid());
    _cameraArc = nullptr;

    setOverlayMode(OverlayMode::None);

    return false;
}

void Viewport::onResourceLoaded(const iResourceID resourceID)
{
    // TODO remove?
}

void Viewport::setOverlayMode(OverlayMode overlayMode)
{
    _overlayMode = overlayMode;
    updateOverlay();

    for (auto overlay : _entityOverlays)
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

void Viewport::frameOnSelection()
{
    if (_cameraArc == nullptr)
    {
        return;
    }

    auto projectScene = iProject::getInstance().getProjectScene();
    if (projectScene == nullptr)
    {
        return;
    }

    const auto &selection = projectScene->getSelection();

    iEntityBoundsTraverser traverser;
    iaSphered selectionSphere;
    bool firstSphere = true;

    for (auto entityID : selection)
    {
        auto entity = projectScene->getEntity(entityID);

        traverser.traverse(entity);
        if (firstSphere)
        {
            selectionSphere = traverser.getSphere();
            firstSphere = false;
        }
        else
        {
            selectionSphere.merge(traverser.getSphere());
        }
    }

    _cameraArc->setCenterOfInterest(selectionSphere._center);

    if (selectionSphere._radius > 0.0f)
    {
        _cameraArc->setDistance(selectionSphere._radius * 3.0f);
    }
    else
    {
        _cameraArc->setDistance(10.0);
    }
}

void Viewport::renderScene()
{
    renderSelection();
}

void Viewport::renderSelection()
{
    auto projectScene = iProject::getInstance().getProjectScene();
    if (projectScene == nullptr)
    {
        return;
    }

    for (const auto &entityID : projectScene->getSelection())
    {
        const auto entity = projectScene->getEntity(entityID);
        if (entity == nullptr)
        {
            continue;
        }

        const auto transformComponent = entity->getComponent<iTransformComponent>();
        if (transformComponent == nullptr)
        {
            continue;
        }
        const auto meshRenderComponent = entity->getComponent<iMeshRenderComponent>();
        if (meshRenderComponent == nullptr)
        {
            continue;
        }

        iRenderer::getInstance().setShader(_materialCelShading);
        iRenderer::getInstance().setLineWidth(4);

        for (const auto &meshRef : meshRenderComponent->getMeshReferences())
        {
            auto matrix = transformComponent->getWorldMatrix();
            matrix *= meshRef._offset;

            iRenderer::getInstance().setModelMatrix(matrix);
            iRenderer::getInstance().drawMesh(meshRef._mesh, nullptr);
        }
    }
}

void Viewport::renderOverlay()
{
    if (_renderOrientationPlane)
    {
        renderOrientationPlane();
    }
}

void Viewport::renderOrientationPlane()
{
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
        _renderOrientationPlane = !_renderOrientationPlane;
        _buttonGrid->setChecked(_renderOrientationPlane);
        return true;

    case iKeyCode::F10:
        _viewportScene->getView().setWireframeVisible(!_viewportScene->getView().isWireframeVisible());
        return true;

    case iKeyCode::F11:
        _viewportScene->getView().setOctreeVisible(!_viewportScene->getView().isOctreeVisible());
        return true;

    case iKeyCode::F12:
        _viewportScene->getView().setBoundingBoxVisible(!_viewportScene->getView().isBoundingBoxVisible());
        _buttonBounds->setChecked(_viewportScene->getView().isBoundingBoxVisible());
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
        if (!iKeyboard::getInstance().keyPressed(iKeyCode::Alt))
        {
            const auto &entitySceneID = _viewportScene->getView().getEntitySceneID();
            auto entityScene = iEntitySystemModule::getInstance().getScene(entitySceneID);
            if (entityScene != nullptr)
            {
                auto entityID = getEntityIDAt(iMouse::getInstance().getPos()._x, iMouse::getInstance().getPos()._y);
                auto entity = entityScene->getEntity(entityID);
                if (entity == nullptr)
                {
                    entityScene->clearSelection();
                }

                if (!iKeyboard::getInstance().keyPressed(iKeyCode::LControl))
                {
                    entityScene->setSelection({entityID});
                }
                else
                {
                    auto selection = entityScene->getSelection();
                    auto iter = std::find(selection.begin(), selection.end(), entityID);
                    if (iter != selection.end())
                    {
                        selection.erase(iter);
                    }
                    else
                    {
                        selection.push_back(entityID);
                    }
                    entityScene->setSelection(selection);
                }
            }
        }
        return true;
    }

    return false;
}

iEntityID Viewport::getEntityIDAt(int32 x, int32 y)
{
    iView &view = _viewportScene->getView();
    const auto &rect = _viewportScene->getActualRect();
    return iEntityID(view.pickEntityID(x - rect._x, y - rect._y));
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
            if (iKeyboard::getInstance().keyPressed(iKeyCode::Alt))
            {
                _cameraArc->setPitch(_cameraArc->getPitch() + (from._y - to._y) * rotateSensitivity);
                _cameraArc->setHeading(_cameraArc->getHeading() + (from._x - to._x) * rotateSensitivity);
            }
        }

        if (iMouse::getInstance().getMiddleButton())
        {
            if (iKeyboard::getInstance().keyPressed(iKeyCode::Alt))
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
    iWidget::onEvent(event);

    event.dispatch<iEventProjectLoaded>(IGOR_BIND_EVENT_FUNCTION(Viewport::onProjectLoaded));
    event.dispatch<iEventProjectUnloaded>(IGOR_BIND_EVENT_FUNCTION(Viewport::onProjectUnloaded));

    return false;
}

void Viewport::clearOverlay()
{
    for (auto overlay : _entityOverlays)
    {
        overlay->setActive(false);
    }
}

void Viewport::updateOverlay()
{
    auto projectScene = iProject::getInstance().getProjectScene();
    if (projectScene == nullptr)
    {
        clearOverlay();
        return;
    }

    const auto &selection = projectScene->getSelection();
    if (selection.size() != 1)
    {
        clearOverlay();
        return;
    }

    auto entity = projectScene->getEntity(selection[0]);
    if (entity == nullptr)
    {
        clearOverlay();
        return;
    }

    for (auto overlay : _entityOverlays)
    {
        const bool active = overlay->accepts(_overlayMode, entity);
        overlay->setActive(active);
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
        iNodeID nodeID = _viewportScene->getView().pickEntityID(iaVector2i(mousePos._x - getActualPosX(),
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
