#include "WorkspaceLayer.h"

WorkspaceLayer::WorkspaceLayer(iWindow *window, int32 zIndex, WorkspacePtr workspace)
    : iLayer(window, "Workspace", zIndex), _workspace(workspace)
{
}

WorkspaceLayer::~WorkspaceLayer()
{
}

void WorkspaceLayer::onDeinit()
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

    // cam
    _cameraArc = std::make_unique<CameraArc>(_workspace->getRootMica());
    _view.setCurrentCamera(_cameraArc->getCameraNode());

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

iNodePtr WorkspaceLayer::getNodeAt(int32 x, int32 y)
{
    //bool wasVisible = _defaultSkyBox->isVisible();
    //_defaultSkyBox->setVisible(false);

    uint64 nodeID = _view.pickcolorID(x, y);
    iNodePtr node = iNodeManager::getInstance().getNode(nodeID);

    //_defaultSkyBox->setVisible(wasVisible);

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

    _cameraArc->setCenterOfInterest(sphere._center);

    if (sphere._radius > 0.0f)
    {
        _cameraArc->setDistance(sphere._radius * 4.0f);
    }
    else
    {
        _cameraArc->setDistance(10.0);
    }
}

void WorkspaceLayer::onEvent(iEvent &event)
{
    event.dispatch<iMouseKeyDownEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(WorkspaceLayer::onMouseKeyDownEvent));
    event.dispatch<iMouseKeyUpEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(WorkspaceLayer::onMouseKeyUpEvent));
    event.dispatch<iMouseMoveEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(WorkspaceLayer::onMouseMoveEvent));
    event.dispatch<iMouseWheelEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(WorkspaceLayer::onMouseWheelEvent));
    event.dispatch<iKeyDownEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(WorkspaceLayer::onKeyDown));
}

bool WorkspaceLayer::onKeyDown(iKeyDownEvent_TMP &event)
{
    switch (event.getKey())
    {
    case iKeyCode::F:
        frameOnSelection();
        return true;

    case iKeyCode::F9:
    {
        iNodeVisitorPrintTree printTree;
        printTree.printToConsole(_workspace->getRootUser());
    }
        return true;

    case iKeyCode::F10:
        _view.setWireframeVisible(!_view.isWireframeVisible());
        break;

    case iKeyCode::F11:
        _view.setOctreeVisible(!_view.isOctreeVisible());
        break;

    case iKeyCode::F12:
        _view.setBoundingBoxVisible(!_view.isBoundingBoxVisible());
        break;
    }
}

bool WorkspaceLayer::onKeyUp(iKeyUpEvent_TMP &event)
{
}

bool WorkspaceLayer::onMouseKeyDownEvent(iMouseKeyDownEvent_TMP &event)
{
    return false;
}

bool WorkspaceLayer::onMouseKeyUpEvent(iMouseKeyUpEvent_TMP &event)
{
    switch (event.getKey())
    {
    case iKeyCode::MouseLeft:

        if (!iKeyboard::getInstance().getKey(iKeyCode::Alt))
        // &&             !_manipulator->isSelected()) hmm
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

            // resetManipulatorMode();
        }

        // _manipulator->onMouseKeyUp(key);
        return true;
    }

    return false;
}

bool WorkspaceLayer::onMouseMoveEvent(iMouseMoveEvent_TMP &event)
{
    const float64 rotateSensitivity = 0.0075;
    const float64 translateSensitivity = 1.0;
    const auto from = event.getLastPosition();
    const auto to = event.getPosition();

    if (iMouse::getInstance().getLeftButton())
    {
        if (iKeyboard::getInstance().getKey(iKeyCode::Alt))
        {
            _cameraArc->setPitch(_cameraArc->getPitch() + (from._y - to._y) * rotateSensitivity);
            _cameraArc->setHeading(_cameraArc->getHeading() + (from._x - to._x) * rotateSensitivity);

            return true;
        }
    }

    if (iMouse::getInstance().getMiddleButton())
    {
        if (iKeyboard::getInstance().getKey(iKeyCode::Alt))
        {
            iNodeCameraPtr camera = static_cast<iNodeCameraPtr>(iNodeManager::getInstance().getNode(_cameraArc->getCameraNode()));
            iNodeTransformPtr cameraDistance = static_cast<iNodeTransformPtr>(iNodeManager::getInstance().getNode(_cameraArc->getCameraDistanceNode()));
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

                auto coi = _cameraArc->getCenterOfInterest();
                coi += (fromWorld - toWorld) * translateFactor;
                _cameraArc->setCenterOfInterest(coi);
            }

            return true;
        }
    }

    return false;
}

bool WorkspaceLayer::onMouseWheelEvent(iMouseWheelEvent_TMP &event)
{
    const float64 wheelSensitivity = 1.2;

    if (event.getWheelDelta() < 0)
    {
        _cameraArc->setDistance(_cameraArc->getDistance() * wheelSensitivity);
    }
    else
    {
        _cameraArc->setDistance(_cameraArc->getDistance() * (1.0 / wheelSensitivity));
    }

    return true;
}
