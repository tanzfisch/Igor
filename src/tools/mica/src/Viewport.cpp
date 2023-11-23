// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "Viewport.h"

Viewport::Viewport(WorkspacePtr workspace)
    : _workspace(workspace)
{
    setHeaderEnabled(false);
    setDockable(true);
    setMoveable(false);
    setIgnoreChildEventConsumption(true);

    _viewportScene = new iWidgetViewport();
    _viewportScene->setVerticalAlignment(iVerticalAlignment::Stretch);
    _viewportScene->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _viewportScene->getView().setScene(_workspace->getScene());
    _viewportScene->getView().setCamera(_workspace->getCameraArc()->getCameraNode());
    _viewportScene->getView().registerRenderDelegate(iDrawDelegate(this, &Viewport::renderSelection));
    addWidget(_viewportScene);

    _viewportOverlay = new iWidgetViewport();
    _viewportOverlay->setVerticalAlignment(iVerticalAlignment::Stretch);
    _viewportOverlay->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    addWidget(_viewportOverlay);

    initScene();
}

Viewport::~Viewport()
{
    _viewportScene->getView().unregisterRenderDelegate(iDrawDelegate(this, &Viewport::renderSelection));
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

    _workspace->getRootMica()->insertNode(_directionalLightRotate);
    _directionalLightRotate->insertNode(_directionalLightTranslate);
    _directionalLightTranslate->insertNode(_lightNode);

    // load materials
    _materialCelShading = iResourceManager::getInstance().loadResource<iMaterial>("igor_material_cellshading_yellow");
    _materialBoundingBox = iResourceManager::getInstance().loadResource<iMaterial>("igor_material_bounding_box");

    // load particle material just so we have one available in the UI
    // TODO this needs a better solution
    iResourceManager::getInstance().loadResource<iMaterial>("igor_material_particles");
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
            iRenderer::getInstance().setMaterial(_materialCelShading);

            iNodeMesh *meshNode = static_cast<iNodeMesh *>(node);
            iRenderer::getInstance().setLineWidth(4);
            iRenderer::getInstance().drawMesh(meshNode->getMesh(), nullptr);
        }
        else
        {
            if (node->getKind() == iNodeKind::Volume)
            {
                iNodeVolume *renderVolume = static_cast<iNodeVolume *>(node);
                iRenderer::getInstance().setMaterial(_materialBoundingBox);

                iAABoxd box = renderVolume->getBoundingBox();
                iRenderer::getInstance().drawBox(box, iaColor4f::yellow);
            }
        }
    }
}

bool Viewport::onKeyDown(iKeyCode key)
{
    if (iWidget::onKeyDown(key))
    {
        return true;
    }

    switch (key)
    {
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

bool Viewport::onMouseKeyUp(iKeyCode key)
{
    iWidget::onMouseKeyUp(key);

    switch (key)
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

void Viewport::onMouseMove(const iaVector2f &pos, bool consumed)
{
    iWidget::onMouseMove(pos, consumed);

    const float64 rotateSensitivity = 0.0075;
    const float64 translateSensitivity = 1.0;
    const auto from = _lastMousePos;
    const auto to = pos;

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

    _lastMousePos = pos;
}

bool Viewport::onMouseWheel(int32 d)
{
    if(iWidget::onMouseWheel(d))
    {
        return true;
    }

    const float64 wheelSensitivity = 1.2;

    if (d < 0)
    {
        _workspace->getCameraArc()->setDistance(_workspace->getCameraArc()->getDistance() * wheelSensitivity);
    }
    else
    {
        _workspace->getCameraArc()->setDistance(_workspace->getCameraArc()->getDistance() * (1.0 / wheelSensitivity));
    }

    return true;
}

iNodePtr Viewport::getNodeAt(int32 x, int32 y)
{
    iView &view = _viewportScene->getView();
    const auto &rect = getActualRect();
    return iNodeManager::getInstance().getNode(view.pickcolorID(x - rect._x, y - rect._y));
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