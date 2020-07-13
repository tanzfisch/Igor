#include "WorkspaceLayer.h"

WorkspaceLayer::WorkspaceLayer(iWindow *window, int32 zIndex, WorkspacePtr workspace)
    : iLayer(window, "Workspace", zIndex), _workspace(workspace)
{
}

WorkspaceLayer::~WorkspaceLayer()
{
}

void WorkspaceLayer::onInit()
{
    _view.setName("Workspace");
    _view.setClearColor(iaColor4f(0.25f, 0.25f, 0.25f, 1.0f));
    _view.setPerspective(45.0f);
    _view.setClipPlanes(1.0f, 10000.f);
    _view.setScene(_workspace->getScene());
    _view.registerRenderDelegate(iDrawDelegate(this, &WorkspaceLayer::renderSelection));

    getWindow()->addView(&_view, getZIndex());

    // set default camera as current
    _view.setCurrentCamera(_workspace->getCameraArc()->getCameraNode());

    // light
    _directionalLightRotate = iNodeManager::getInstance().createNode<iNodeTransform>();
    _directionalLightRotate->setName("directional light rotate");

    _directionalLightTranslate = iNodeManager::getInstance().createNode<iNodeTransform>();
    _directionalLightTranslate->setName("directional light translate");
    _directionalLightTranslate->translate(10000, 10000, 0);

    _lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
    _lightNode->setName("directional light");
    _lightNode->setAmbient(iaColor4f(0.5f, 0.5f, 0.5f, 1.0f));
    _lightNode->setDiffuse(iaColor4f(0.9f, 0.9f, 0.9f, 1.0f));
    _lightNode->setSpecular(iaColor4f(1.0f, 1.0f, 1.0f, 1.0f));

    _workspace->getRootMica()->insertNode(_directionalLightRotate);
    _directionalLightRotate->insertNode(_directionalLightTranslate);
    _directionalLightTranslate->insertNode(_lightNode);

    // create materials
    _materialCelShading = iMaterialResourceFactory::getInstance().createMaterial("celShadingMaterial");
    auto celShadingMaterial = iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading);
    celShadingMaterial->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
    celShadingMaterial->addShaderSource("Mica/yellow.frag", iShaderObjectType::Fragment);
    celShadingMaterial->compileShader();
    celShadingMaterial->setRenderState(iRenderState::Wireframe, iRenderStateValue::On);
    celShadingMaterial->setRenderState(iRenderState::CullFace, iRenderStateValue::On);
    celShadingMaterial->setRenderState(iRenderState::CullFaceFunc, iRenderStateValue::Front);

    _materialBoundingBox = iMaterialResourceFactory::getInstance().createMaterial("materialBoundingBox");
}

void WorkspaceLayer::onDeinit()
{
    _view.unregisterRenderDelegate(iDrawDelegate(this, &WorkspaceLayer::renderSelection));
}

iNodePtr WorkspaceLayer::getNodeAt(int32 x, int32 y)
{
    // TODO if there is a sky box it needs to be invisible now

    uint64 nodeID = _view.pickcolorID(x, y);
    iNodePtr node = iNodeManager::getInstance().getNode(nodeID);

    return node;
}

void WorkspaceLayer::renderSelection()
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
            iRenderer::getInstance().setMaterial(iMaterialResourceFactory::getInstance().getMaterial(_materialCelShading));

            iNodeMesh *meshNode = static_cast<iNodeMesh *>(node);
            std::shared_ptr<iMeshBuffers> buffers = meshNode->getMeshBuffers();
            iRenderer::getInstance().setLineWidth(4);
            iRenderer::getInstance().drawMesh(buffers);
        }
        else
        {
            if (node->getKind() == iNodeKind::Volume)
            {
                iNodeVolume *renderVolume = static_cast<iNodeVolume *>(node);
                iRenderer::getInstance().setMaterial(iMaterialResourceFactory::getInstance().getMaterial(_materialBoundingBox));

                iAABoxd box = renderVolume->getBoundingBox();

                iRenderer::getInstance().setColor(1, 1, 0, 1);
                iRenderer::getInstance().drawBBox(box);
            }
        }
    }
}

void WorkspaceLayer::frameOnSelection()
{
    auto selection = _workspace->getSelection();

    iNodeVisitorBoundings visitorBoundings;
    iSphered sphere;
    iSphered selectionSphere;

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

void WorkspaceLayer::onEvent(iEvent &event)
{
    event.dispatch<iEventMouseKeyDown>(IGOR_BIND_EVENT_FUNCTION(WorkspaceLayer::onMouseKeyDownEvent));
    event.dispatch<iEventMouseKeyUp>(IGOR_BIND_EVENT_FUNCTION(WorkspaceLayer::onMouseKeyUpEvent));
    event.dispatch<iEventMouseMove>(IGOR_BIND_EVENT_FUNCTION(WorkspaceLayer::onMouseMoveEvent));
    event.dispatch<iEventMouseWheel>(IGOR_BIND_EVENT_FUNCTION(WorkspaceLayer::onMouseWheelEvent));
    event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(WorkspaceLayer::onKeyDown));
}

bool WorkspaceLayer::onKeyDown(iEventKeyDown &event)
{
    switch (event.getKey())
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
        _view.setWireframeVisible(!_view.isWireframeVisible()); // TODO does not work in mica
        return true;

    case iKeyCode::F11:
        _view.setOctreeVisible(!_view.isOctreeVisible()); // TODO does not work in mica
        return true;

    case iKeyCode::F12:
        _view.setBoundingBoxVisible(!_view.isBoundingBoxVisible()); // TODO does not work in mica
        return true;
    }

    return false;
}

bool WorkspaceLayer::onMouseKeyDownEvent(iEventMouseKeyDown &event)
{
    return false;
}

bool WorkspaceLayer::onMouseKeyUpEvent(iEventMouseKeyUp &event)
{
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

bool WorkspaceLayer::onMouseMoveEvent(iEventMouseMove &event)
{
    const float64 rotateSensitivity = 0.0075;
    const float64 translateSensitivity = 1.0;
    const auto from = event.getLastPosition();
    const auto to = event.getPosition();

    if (iMouse::getInstance().getLeftButton())
    {
        if (iKeyboard::getInstance().getKey(iKeyCode::Alt))
        {
            _workspace->getCameraArc()->setPitch(_workspace->getCameraArc()->getPitch() + (from._y - to._y) * rotateSensitivity);
            _workspace->getCameraArc()->setHeading(_workspace->getCameraArc()->getHeading() + (from._x - to._x) * rotateSensitivity);

            return true;
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
                iaVector3d fromWorld = camWorldMatrix * _view.unProject(iaVector3d(from._x, from._y, 0), camWorldMatrix);
                iaVector3d toWorld = camWorldMatrix * _view.unProject(iaVector3d(to._x, to._y, 0), camWorldMatrix);

                iaMatrixd camTranslateMatrix;
                cameraDistance->getMatrix(camTranslateMatrix);
                float64 translateFactor = camTranslateMatrix._pos.length() * translateSensitivity;

                auto coi = _workspace->getCameraArc()->getCenterOfInterest();
                coi += (fromWorld - toWorld) * translateFactor;
                _workspace->getCameraArc()->setCenterOfInterest(coi);
            }

            return true;
        }
    }

    return false;
}

bool WorkspaceLayer::onMouseWheelEvent(iEventMouseWheel &event)
{
    const float64 wheelSensitivity = 1.2;

    if (event.getWheelDelta() < 0)
    {
        _workspace->getCameraArc()->setDistance(_workspace->getCameraArc()->getDistance() * wheelSensitivity);
    }
    else
    {
        _workspace->getCameraArc()->setDistance(_workspace->getCameraArc()->getDistance() * (1.0 / wheelSensitivity));
    }

    return true;
}
