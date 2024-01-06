// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "TransformOverlay.h"

TransformOverlay::TransformOverlay(iViewPtr view, iScenePtr scene, WorkspacePtr workspace)
    : NodeOverlay(view, scene, workspace)
{
    onInit();
}

TransformOverlay::~TransformOverlay()
{
    onDeinit();
}

void TransformOverlay::setNodeID(uint64 nodeID)
{
    NodeOverlay::setNodeID(nodeID);

    update();
}

bool TransformOverlay::accepts(OverlayMode mode, iNodeKind nodeKind, iNodeType nodeType)
{
    return nodeKind == iNodeKind::Transformation;
}

void TransformOverlay::onDeinit()
{
    getView()->unregisterRenderDelegate(iDrawDelegate(this, &TransformOverlay::onRender));

    _red = nullptr;
    _green = nullptr;
    _blue = nullptr;
    _cyan = nullptr;
    _material = nullptr;
    _materialCelShading = nullptr;
}

void TransformOverlay::onInit()
{
    getView()->registerRenderDelegate(iDrawDelegate(this, &TransformOverlay::onRender));

    _material = iResourceManager::getInstance().loadResource<iShaderMaterial>("igor_material_transform_overlay_base");
    _materialCelShading = iResourceManager::getInstance().loadResource<iShaderMaterial>("igor_material_cellshading_yellow");

    iParameters paramRed({
        {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
        {IGOR_RESOURCE_PARAM_GENERATE, true},
        {IGOR_RESOURCE_PARAM_AMBIENT, iaColor3f(0.3f, 0.0f, 0.0f)},
        {IGOR_RESOURCE_PARAM_DIFFUSE, iaColor3f(0.5f, 0.0f, 0.0f)},
        {IGOR_RESOURCE_PARAM_SPECULAR, iaColor3f(0.2f, 0.0f, 0.0f)},
        {IGOR_RESOURCE_PARAM_EMISSIVE, iaColor3f(0.8f, 0.0f, 0.0f)},
        {IGOR_RESOURCE_PARAM_ALPHA, 0.8f},
    });

    _red = iResourceManager::getInstance().loadResource<iMaterial>(paramRed);    

    iParameters paramGreen({
        {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
        {IGOR_RESOURCE_PARAM_GENERATE, true},
        {IGOR_RESOURCE_PARAM_AMBIENT, iaColor3f(0.0f, 0.3f, 0.0f)},
        {IGOR_RESOURCE_PARAM_DIFFUSE, iaColor3f(0.0f, 0.5f, 0.0f)},
        {IGOR_RESOURCE_PARAM_SPECULAR, iaColor3f(0.0f, 0.2f, 0.0f)},
        {IGOR_RESOURCE_PARAM_EMISSIVE, iaColor3f(0.0f, 0.8f, 0.0f)},
        {IGOR_RESOURCE_PARAM_ALPHA, 0.8f},
    });

    _green = iResourceManager::getInstance().loadResource<iMaterial>(paramGreen);    

    iParameters paramBlue({
        {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
        {IGOR_RESOURCE_PARAM_GENERATE, true},
        {IGOR_RESOURCE_PARAM_AMBIENT, iaColor3f(0.0f, 0.0f, 0.3f)},
        {IGOR_RESOURCE_PARAM_DIFFUSE, iaColor3f(0.0f, 0.0f, 0.5f)},
        {IGOR_RESOURCE_PARAM_SPECULAR, iaColor3f(0.0f, 0.0f, 0.2f)},
        {IGOR_RESOURCE_PARAM_EMISSIVE, iaColor3f(0.0f, 0.0f, 0.8f)},
        {IGOR_RESOURCE_PARAM_ALPHA, 0.8f},
    });

    _blue = iResourceManager::getInstance().loadResource<iMaterial>(paramBlue);    

    iParameters paramCyan({
        {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
        {IGOR_RESOURCE_PARAM_GENERATE, true},
        {IGOR_RESOURCE_PARAM_AMBIENT, iaColor3f(0.0f, 0.3f, 0.3f)},
        {IGOR_RESOURCE_PARAM_DIFFUSE, iaColor3f(0.0f, 0.5f, 0.5f)},
        {IGOR_RESOURCE_PARAM_SPECULAR, iaColor3f(0.0f, 0.2f, 0.2f)},
        {IGOR_RESOURCE_PARAM_EMISSIVE, iaColor3f(0.0f, 0.8f, 0.8f)},
        {IGOR_RESOURCE_PARAM_ALPHA, 0.8f},
    });

    _cyan = iResourceManager::getInstance().loadResource<iMaterial>(paramCyan);    

    iMeshPtr translateMesh = createTranslateMesh();
    iMeshPtr scaleMesh = createScaleMesh();
    iMeshPtr ringMesh = createRingMesh();
    iMeshPtr ringMesh2D = create2DRingMesh();
    iMeshPtr cylinder = createCylinder();

    _rootTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    _switchNode = iNodeManager::getInstance().createNode<iNodeSwitch>();
    _rootTransform->insertNode(_switchNode);

    createTranslateModifier(translateMesh);
    createScaleModifier(scaleMesh);
    createRotateModifier(ringMesh, ringMesh2D, cylinder);
    createLocatorRepresentation(cylinder);

    getScene()->getRoot()->insertNode(_rootTransform);

    _rootTransform->setActive(false);
    _switchNode->setActiveChild(nullptr);
}

void TransformOverlay::scale(const iaVector3d &vec, iaMatrixd &matrix)
{
    const iaVector3d dir[] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {1, 1, 1}};
    iaVector3d scale;

    for (int i = 0; i < 4; ++i)
    {
        if (_selectedManipulatorNodeID == _scaleIDs[i])
        {
            scale = vec.project(dir[i]) + iaVector3d(1, 1, 1);
            matrix.scale(scale);
            return;
        }
    }
}

void TransformOverlay::translate(const iaVector3d &vec, iaMatrixd &matrix)
{
    static const iaVector3d dir[] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    iaVector3d translate;

    for (int i = 0; i < 3; ++i)
    {
        if (_selectedManipulatorNodeID == _translateIDs[i])
        {
            translate = vec.project(dir[i]);
            matrix.translate(translate);
            return;
        }
    }
}

void TransformOverlay::rotate(const iaVector2d &from, const iaVector2d &to, iaMatrixd &matrix)
{
    iNode *node = iNodeManager::getInstance().getNode(getNodeID());
    iaMatrixd transformWorldMatrix;
    node->calcWorldTransformation(transformWorldMatrix);

    iaMatrixd camWorldMatrix;
    getWorkspace()->getCameraArc()->getWorldTransformation(camWorldMatrix);
    iaVector3d center = getView()->project(transformWorldMatrix._pos, camWorldMatrix);

    iaVector2d center2D(center._x, center._y);

    iaVector2d a = from - center2D;
    iaVector2d b = to - center2D;

    float64 angle = b.angle(a);

    for (int i = 0; i < 3; ++i)
    {
        if (_selectedManipulatorNodeID == _rotateIDs[i])
        {
            iaAxis axis = static_cast<iaAxis>(i);
            float64 scalar = 0;

            iaVector3d toCam = camWorldMatrix._pos - matrix._pos;

            switch (axis)
            {
            case iaAxis::X:
                scalar = toCam * matrix._right;
                break;
            case iaAxis::Y:
                scalar = toCam * matrix._top;
                break;
            case iaAxis::Z:
                scalar = toCam * matrix._depth;
                break;
            }

            if (scalar < 0)
            {
                angle = -angle;
            }

            matrix.rotate(angle, static_cast<iaAxis>(i));
            return;
        }
    }
}

void TransformOverlay::setActive(bool active)
{
    NodeOverlay::setActive(active);

    _rootTransform->setActive(active);

    update();
}

void TransformOverlay::update()
{
    iNodePtr node = iNodeManager::getInstance().getNode(getNodeID());
    if (node == nullptr)
    {
        return;
    }

    iaMatrixd matrix;
    node->calcWorldTransformation(matrix);

    iaMatrixd camMatrix;
    getWorkspace()->getCameraArc()->getWorldTransformation(camMatrix);

    float64 distanceToCam = camMatrix._pos.distance(matrix._pos) * 0.1;

    matrix._right.normalize();
    matrix._top.normalize();
    matrix._depth.normalize();
    _rootTransform->setMatrix(matrix);
    _rootTransform->scale(distanceToCam, distanceToCam, distanceToCam);

    // compensate for parent orientation
    iaMatrixd parentMatrix;
    _rotateBillboardTransform->getParent()->calcWorldTransformation(parentMatrix);
    parentMatrix._pos.set(0, 0, 0);
    parentMatrix.invert();
    parentMatrix._right.normalize();
    parentMatrix._top.normalize();
    parentMatrix._depth.normalize();

    _rotateBillboardTransform->identity();
    iaMatrixd billboardMatrix;
    _rotateBillboardTransform->getMatrix(billboardMatrix);
    billboardMatrix._right = camMatrix._right;
    billboardMatrix._top = camMatrix._top;
    billboardMatrix._depth = camMatrix._depth;
    _rotateBillboardTransform->setMatrix(parentMatrix * billboardMatrix);
    _rotateBillboardTransform->rotate(M_PI * 0.5, iaAxis::X);
    _rotateBillboardTransform->scale(2.1, 2.1, 2.1);
}

void TransformOverlay::createRotateModifier(iMeshPtr &ringMesh, iMeshPtr &ringMesh2D, iMeshPtr &cylinder)
{
    _roateModifier = iNodeManager::getInstance().createNode<iNode>();
    _switchNode->insertNode(_roateModifier);

    iNodeTransform *xRingTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    xRingTransform->rotate(-M_PI * 0.5, iaAxis::Z);
    xRingTransform->scale(2.0, 0.1, 2.0);
    _roateModifier->insertNode(xRingTransform);

    iNodeTransform *yRingTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    yRingTransform->scale(1.99, 0.1, 1.99);
    _roateModifier->insertNode(yRingTransform);

    iNodeTransform *zRingTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    zRingTransform->rotate(M_PI * 0.5, iaAxis::X);
    zRingTransform->scale(1.98, 0.1, 1.98);
    _roateModifier->insertNode(zRingTransform);

    iNodeMesh *xRing = iNodeManager::getInstance().createNode<iNodeMesh>();
    xRing->setName("manipulator.ring.x");
    xRing->setMesh(ringMesh);
    xRing->setMaterial(_material);
    xRing->setTargetMaterial(_red);
    xRingTransform->insertNode(xRing);

    iNodeMesh *yRing = iNodeManager::getInstance().createNode<iNodeMesh>();
    yRing->setName("manipulator.ring.y");
    yRing->setMesh(ringMesh);
    yRing->setMaterial(_material);
    yRing->setTargetMaterial(_green);
    yRingTransform->insertNode(yRing);

    iNodeMesh *zRing = iNodeManager::getInstance().createNode<iNodeMesh>();
    zRing->setName("manipulator.ring.z");
    zRing->setMesh(ringMesh);
    zRing->setMaterial(_material);
    zRing->setTargetMaterial(_blue);
    zRingTransform->insertNode(zRing);

    _rotateBillboardTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    _roateModifier->insertNode(_rotateBillboardTransform);

    iNodeMesh *ring = iNodeManager::getInstance().createNode<iNodeMesh>();
    ring->setName("manipulator.ring");
    ring->setMesh(ringMesh2D);
    ring->setMaterial(_material);
    ring->setTargetMaterial(_cyan);
    _rotateBillboardTransform->insertNode(ring);

    _rotateIDs.push_back(xRing->getID());
    _rotateIDs.push_back(yRing->getID());
    _rotateIDs.push_back(zRing->getID());

    // add a locator in the middle for better orientation
    iNodeTransform *xTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
    _roateModifier->insertNode(xTransform);

    iNodeTransform *yTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    _roateModifier->insertNode(yTransform);

    iNodeTransform *zTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    zTransform->rotate(M_PI * 0.5, iaAxis::X);
    _roateModifier->insertNode(zTransform);

    iNodeMesh *xCylinder = iNodeManager::getInstance().createNode<iNodeMesh>();
    xCylinder->setName("manipulator.cylinder.x");
    xCylinder->setMesh(cylinder);
    xCylinder->setMaterial(_material);
    xCylinder->setTargetMaterial(_red);
    xTransform->insertNode(xCylinder);

    iNodeMesh *yCylinder = iNodeManager::getInstance().createNode<iNodeMesh>();
    yCylinder->setName("manipulator.cylinder.y");
    yCylinder->setMesh(cylinder);
    yCylinder->setMaterial(_material);
    yCylinder->setTargetMaterial(_green);
    yTransform->insertNode(yCylinder);

    iNodeMesh *zCylinder = iNodeManager::getInstance().createNode<iNodeMesh>();
    zCylinder->setName("manipulator.cylinder.z");
    zCylinder->setMesh(cylinder);
    zCylinder->setMaterial(_material);
    zCylinder->setTargetMaterial(_blue);
    zTransform->insertNode(zCylinder);
}

void TransformOverlay::createLocatorRepresentation(iMeshPtr &cylinder)
{
    _locatorRepresentation = iNodeManager::getInstance().createNode<iNode>();
    _switchNode->insertNode(_locatorRepresentation);

    iNodeTransform *xTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
    _locatorRepresentation->insertNode(xTransform);

    iNodeTransform *yTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    _locatorRepresentation->insertNode(yTransform);

    iNodeTransform *zTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    zTransform->rotate(M_PI * 0.5, iaAxis::X);
    _locatorRepresentation->insertNode(zTransform);

    iNodeMesh *xCylinder = iNodeManager::getInstance().createNode<iNodeMesh>();
    xCylinder->setName("manipulator.cylinder.x");
    xCylinder->setMesh(cylinder);
    xCylinder->setMaterial(_material);
    xCylinder->setTargetMaterial(_red);
    xTransform->insertNode(xCylinder);

    iNodeMesh *yCylinder = iNodeManager::getInstance().createNode<iNodeMesh>();
    yCylinder->setName("manipulator.cylinder.y");
    yCylinder->setMesh(cylinder);
    yCylinder->setMaterial(_material);
    yCylinder->setTargetMaterial(_green);
    yTransform->insertNode(yCylinder);

    iNodeMesh *zCylinder = iNodeManager::getInstance().createNode<iNodeMesh>();
    zCylinder->setName("manipulator.cylinder.z");
    zCylinder->setMesh(cylinder);
    zCylinder->setMaterial(_material);
    zCylinder->setTargetMaterial(_blue);
    zTransform->insertNode(zCylinder);
}

void TransformOverlay::createTranslateModifier(iMeshPtr &translateMesh)
{
    _translateModifier = iNodeManager::getInstance().createNode<iNode>();
    _switchNode->insertNode(_translateModifier);

    iNodeTransform *xTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
    _translateModifier->insertNode(xTransform);

    iNodeTransform *yTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    _translateModifier->insertNode(yTransform);

    iNodeTransform *zTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    zTransform->rotate(M_PI * 0.5, iaAxis::X);
    _translateModifier->insertNode(zTransform);

    iNodeMesh *xUmbrella = iNodeManager::getInstance().createNode<iNodeMesh>();
    xUmbrella->setName("manipulator.umbrella.x");
    xUmbrella->setMesh(translateMesh);
    xUmbrella->setMaterial(_material);
    xUmbrella->setTargetMaterial(_red);
    xTransform->insertNode(xUmbrella);

    iNodeMesh *yUmbrella = iNodeManager::getInstance().createNode<iNodeMesh>();
    yUmbrella->setName("manipulator.umbrella.y");
    yUmbrella->setMesh(translateMesh);
    yUmbrella->setMaterial(_material);
    yUmbrella->setTargetMaterial(_green);
    yTransform->insertNode(yUmbrella);

    iNodeMesh *zUmbrella = iNodeManager::getInstance().createNode<iNodeMesh>();
    zUmbrella->setName("manipulator.umbrella.z");
    zUmbrella->setMesh(translateMesh);
    zUmbrella->setMaterial(_material);
    zUmbrella->setTargetMaterial(_blue);
    zTransform->insertNode(zUmbrella);

    _translateIDs.push_back(xUmbrella->getID());
    _translateIDs.push_back(yUmbrella->getID());
    _translateIDs.push_back(zUmbrella->getID());
}

void TransformOverlay::createScaleModifier(iMeshPtr &scaleMesh)
{
    _scaleModifier = iNodeManager::getInstance().createNode<iNode>();
    _switchNode->insertNode(_scaleModifier);

    iNodeTransform *xTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
    _scaleModifier->insertNode(xTransform);

    iNodeTransform *yTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    _scaleModifier->insertNode(yTransform);

    iNodeTransform *zTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    zTransform->rotate(M_PI * 0.5, iaAxis::X);
    _scaleModifier->insertNode(zTransform);

    iNodeMesh *xCube = iNodeManager::getInstance().createNode<iNodeMesh>();
    xCube->setName("manipulator.cube.x");
    xCube->setMesh(scaleMesh);
    xCube->setMaterial(_material);
    xCube->setTargetMaterial(_red);
    xTransform->insertNode(xCube);

    iNodeMesh *yCube = iNodeManager::getInstance().createNode<iNodeMesh>();
    yCube->setName("manipulator.cube.y");
    yCube->setMesh(scaleMesh);
    yCube->setMaterial(_material);
    yCube->setTargetMaterial(_green);
    yTransform->insertNode(yCube);

    iNodeMesh *zCube = iNodeManager::getInstance().createNode<iNodeMesh>();
    zCube->setName("manipulator.cube.z");
    zCube->setMesh(scaleMesh);
    zCube->setMaterial(_material);
    zCube->setTargetMaterial(_blue);
    zTransform->insertNode(zCube);

    iMeshPtr cube = createCube();

    iNodeMesh *xyzCube = iNodeManager::getInstance().createNode<iNodeMesh>();
    xyzCube->setName("manipulator.cube.xyz");
    xyzCube->setMesh(cube);
    xyzCube->setMaterial(_material);
    xyzCube->setTargetMaterial(_cyan);
    _scaleModifier->insertNode(xyzCube);

    _scaleIDs.push_back(xCube->getID());
    _scaleIDs.push_back(yCube->getID());
    _scaleIDs.push_back(zCube->getID());
    _scaleIDs.push_back(xyzCube->getID());
}

iMeshPtr TransformOverlay::createRingMesh()
{
    iMeshBuilder meshBuilder;
    iMeshBuilderUtils::addCylinder(meshBuilder, 1, 1, 64, false);
    meshBuilder.calcNormals(true);
    return meshBuilder.createMesh();
}

iMeshPtr TransformOverlay::create2DRingMesh()
{
    iMeshBuilder meshBuilder;
    iMeshBuilderUtils::addRing(meshBuilder, 0.99, 1, 64);
    meshBuilder.calcNormals(true);
    return meshBuilder.createMesh();
}

iMeshPtr TransformOverlay::createScaleMesh()
{
    iMeshBuilder meshBuilder;

    iaMatrixf matrix;
    matrix.scale(0.02, 1.5, 0.02);
    meshBuilder.setMatrix(matrix);
    iMeshBuilderUtils::addCylinder(meshBuilder, 1, 1, 6);

    matrix.identity();
    matrix.translate(0, 1.5, 0);
    matrix.scale(0.25, 0.25, 0.25);
    meshBuilder.setMatrix(matrix);
    iMeshBuilderUtils::addBox(meshBuilder, 1, 1, 1);

    meshBuilder.calcNormals(true);
    return meshBuilder.createMesh();
}

iMeshPtr TransformOverlay::createCube()
{
    iMeshBuilder meshBuilder;

    iaMatrixf matrix;
    matrix.scale(0.25, 0.25, 0.25);
    meshBuilder.setMatrix(matrix);
    iMeshBuilderUtils::addBox(meshBuilder, 1, 1, 1);

    meshBuilder.calcNormals(true);
    return meshBuilder.createMesh();
}

iMeshPtr TransformOverlay::createCylinder()
{
    iMeshBuilder meshBuilder;
    meshBuilder.setJoinVertexes(true);

    iaMatrixf matrix;
    matrix.scale(0.025, 1.5, 0.025);
    meshBuilder.setMatrix(matrix);
    iMeshBuilderUtils::addCylinder(meshBuilder, 1, 1, 6);

    meshBuilder.calcNormals(true);

    return meshBuilder.createMesh();
}

iMeshPtr TransformOverlay::createTranslateMesh()
{
    iMeshBuilder meshBuilder;
    meshBuilder.setJoinVertexes(false);

    iaMatrixf matrix;

    matrix.translate(0, 1.5, 0);
    matrix.scale(0.1, 0.5, 0.1);
    meshBuilder.setMatrix(matrix);
    iMeshBuilderUtils::addCone(meshBuilder, 1, 1, 6);

    matrix.identity();
    matrix.scale(0.025, 1.5, 0.025);
    meshBuilder.setMatrix(matrix);
    iMeshBuilderUtils::addCylinder(meshBuilder, 1, 1, 6);

    meshBuilder.calcNormals(true);

    return meshBuilder.createMesh();
}

void TransformOverlay::setOverlayMode(OverlayMode manipulatorMode)
{
    NodeOverlay::setOverlayMode(manipulatorMode);

    switch (manipulatorMode)
    {
    case OverlayMode::None:
        _switchNode->setActiveChild(_locatorRepresentation);
        break;

    case OverlayMode::Translate:
        _switchNode->setActiveChild(_translateModifier);
        break;

    case OverlayMode::Scale:
        _switchNode->setActiveChild(_scaleModifier);
        break;

    case OverlayMode::Rotate:
        _switchNode->setActiveChild(_roateModifier);
        break;
    }

    update();
}

void TransformOverlay::onRender()
{
    renderHighlight();
}

void TransformOverlay::renderHighlight()
{
    if (_selectedManipulatorNodeID == iNode::INVALID_NODE_ID)
    {
        return;
    }

    const iNodePtr node = iNodeManager::getInstance().getNode(_selectedManipulatorNodeID);

    if (node->getKind() == iNodeKind::Renderable ||
        node->getKind() == iNodeKind::Volume)
    {
        const iNodeRenderPtr renderNode = static_cast<iNodeRenderPtr>(node);
        iRenderer::getInstance().setModelMatrix(renderNode->getWorldMatrix());

        if (node->getType() == iNodeType::iNodeMesh)
        {
            iRenderer::getInstance().setMaterial(_materialCelShading);

            iNodeMesh *meshNode = static_cast<iNodeMesh *>(node);
            iRenderer::getInstance().setLineWidth(4);
            iRenderer::getInstance().drawMesh(meshNode->getMesh(), nullptr);
        }
    }
}

bool TransformOverlay::onMouseKeyUpEvent(iEventMouseKeyUp &event)
{
    if (_selectedManipulatorNodeID == iNode::INVALID_NODE_ID)
    {
        return false;
    }

    _selectedManipulatorNodeID = iNode::INVALID_NODE_ID;
    return true;
}

bool TransformOverlay::onMouseKeyDownEvent(iEventMouseKeyDown &event)
{
    auto rect = getView()->getViewport();
    auto window = iApplication::getInstance().getWindow();

    auto top = window->getClientHeight() - rect._height - rect._y;

    iNodeID nodeID = getView()->pickcolorID(event.getPosition()._x - rect._x, event.getPosition()._y - top);

    _selectedManipulatorNodeID = iNode::INVALID_NODE_ID;

    for (int i = 0; i < 4; ++i)
    {
        if (nodeID == _scaleIDs[i])
        {
            _selectedManipulatorNodeID = nodeID;
            return true;
        }
    }

    for (int i = 0; i < 3; ++i)
    {
        if (nodeID == _translateIDs[i])
        {
            _selectedManipulatorNodeID = nodeID;
            return true;
        }

        if (nodeID == _rotateIDs[i])
        {
            _selectedManipulatorNodeID = nodeID;
            return true;
        }
    }

    return false;
}

bool TransformOverlay::onMouseMoveEvent(iEventMouseMove &event)
{
    if (_selectedManipulatorNodeID == iNode::INVALID_NODE_ID)
    {
        return false;
    }

    iNodePtr node = iNodeManager::getInstance().getNode(getNodeID());
    if (node == nullptr ||
        node->getType() != iNodeType::iNodeTransform)
    {
        return false;
    }

    iaVector2d fromd = event.getLastPosition().convert<float64>();
    iaVector2d tod = event.getPosition().convert<float64>();

    iaMatrixd camWorldMatrix;
    getWorkspace()->getCameraArc()->getWorldTransformation(camWorldMatrix);

    iaVector3d fromWorld = camWorldMatrix * getView()->unProject(iaVector3d(fromd._x, fromd._y, 0), camWorldMatrix);
    iaVector3d toWorld = camWorldMatrix * getView()->unProject(iaVector3d(tod._x, tod._y, 0), camWorldMatrix);

    iNodeTransform *transformNode = static_cast<iNodeTransform *>(node);
    iaMatrixd transformWorldMatrix;
    transformNode->calcWorldTransformation(transformWorldMatrix);
    transformWorldMatrix.invert();
    fromWorld = transformWorldMatrix * fromWorld;
    toWorld = transformWorldMatrix * toWorld;

    float64 distance = getWorkspace()->getCameraArc()->getDistance();

    iaMatrixd nodeMatrix;
    transformNode->getMatrix(nodeMatrix);

    switch (getOverlayMode())
    {
    case OverlayMode::None:
        break;
    case OverlayMode::Rotate:
        rotate(fromd, tod, nodeMatrix);
        break;
    case OverlayMode::Scale:
        scale((toWorld - fromWorld) * distance * 2, nodeMatrix);
        break;
    case OverlayMode::Translate:
        translate((toWorld - fromWorld) * distance, nodeMatrix);
        break;
    }

    transformNode->setMatrix(nodeMatrix);

    update();

    return false;
}
