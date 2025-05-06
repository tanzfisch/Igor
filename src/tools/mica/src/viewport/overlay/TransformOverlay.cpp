// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "TransformOverlay.h"

TransformOverlay::TransformOverlay(iViewPtr view)
    : EntityOverlay(view)
{
    onInit();
}

TransformOverlay::~TransformOverlay()
{
    onDeinit();
}

void TransformOverlay::setEntity(iEntityID entityID)
{
    EntityOverlay::setEntity(entityID);

    update();
}

bool TransformOverlay::accepts(OverlayMode mode, iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");

    if (mode != OverlayMode::Rotate &&
        mode != OverlayMode::Translate &&
        mode != OverlayMode::Scale)
    {
        return false;
    }

    auto transform = entity->getComponent<iTransformComponent>();

    return transform != nullptr;
}

void TransformOverlay::onDeinit()
{
    getView()->getRenderEvent().remove(iRenderDelegate(this, &TransformOverlay::onRender));

    _red = nullptr;
    _green = nullptr;
    _blue = nullptr;
    _cyan = nullptr;
    _materialCelShading = nullptr;
}

void TransformOverlay::onInit()
{
    getView()->getRenderEvent().add(iRenderDelegate(this, &TransformOverlay::onRender));

    iShaderPtr shader = iResourceManager::getInstance().loadResource<iShader>("igor_shader_material_transform_overlay_base");
    iParameters paramMaterial({
        {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
        {IGOR_RESOURCE_PARAM_GENERATE, true},

    });

    iShaderPtr shaderMaterialCelShading = iResourceManager::getInstance().loadResource<iShader>("igor_shader_material_cellshading_yellow");
    iParameters paramMaterialCelshading({
        {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
        {IGOR_RESOURCE_PARAM_GENERATE, true},
        {IGOR_RESOURCE_PARAM_SHADER, shaderMaterialCelShading},
    });
    _materialCelShading = iResourceManager::getInstance().loadResource<iMaterial>(paramMaterialCelshading);

    iParameters paramRed({
        {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
        {IGOR_RESOURCE_PARAM_GENERATE, true},
        {IGOR_RESOURCE_PARAM_SHADER, shader},
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
        {IGOR_RESOURCE_PARAM_SHADER, shader},
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
        {IGOR_RESOURCE_PARAM_SHADER, shader},
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
        {IGOR_RESOURCE_PARAM_SHADER, shader},
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

    const auto &entitySceneID = getView()->getEntitySceneID();
    auto entityScene = iEntitySystemModule::getInstance().getScene(entitySceneID);
    con_assert(entityScene != nullptr, "no scene");
    _rootTransform = entityScene->createEntity();
    _rootTransform->setActive(false);

    createTranslateModifier(translateMesh);
    createScaleModifier(scaleMesh);
    createRotateModifier(ringMesh, ringMesh2D, cylinder);
    createLocatorRepresentation(cylinder);
}

void TransformOverlay::setActive(bool active)
{
    EntityOverlay::setActive(active);

    _rootTransform->setActive(active);

    update();
}

void TransformOverlay::update()
{
    /*iNodePtr node = iNodeManager::getInstance().getNode(getNodeID());
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
    _rotateBillboardTransform->scale(2.1, 2.1, 2.1);*/
}

void TransformOverlay::createRotateModifier(iMeshPtr &ringMesh, iMeshPtr &ringMesh2D, iMeshPtr &cylinder)
{
    const auto &entitySceneID = getView()->getEntitySceneID();
    auto entityScene = iEntitySystemModule::getInstance().getScene(entitySceneID);
    _roateModifier = entityScene->createEntity();
    _roateModifier->setParent(_rootTransform);

    iEntityPtr xRingTransform = entityScene->createEntity("ring.x");
    xRingTransform->addComponent(new iTransformComponent(iaVector3d(), iaVector3d(0, 0, -M_PI * 0.5), iaVector3d(2.0, 0.1, 2.0)));
    xRingTransform->addComponent(new iSphereComponent(1.0));
    xRingTransform->addComponent(new iOctreeComponent());
    auto xMeshRenderComponent = xRingTransform->addComponent(new iMeshRenderComponent());
    xMeshRenderComponent->addMesh(ringMesh, _red);
    xRingTransform->setParent(_roateModifier);

    iEntityPtr yRingTransform = entityScene->createEntity("ring.y");
    yRingTransform->addComponent(new iTransformComponent(iaVector3d(), iaVector3d(), iaVector3d(1.99, 0.1, 1.99)));
    yRingTransform->addComponent(new iSphereComponent(1.0));
    yRingTransform->addComponent(new iOctreeComponent());
    auto yMeshRenderComponent = yRingTransform->addComponent(new iMeshRenderComponent());
    yMeshRenderComponent->addMesh(ringMesh, _green);
    yRingTransform->setParent(_roateModifier);

    iEntityPtr zRingTransform = entityScene->createEntity("ring.z");
    zRingTransform->addComponent(new iTransformComponent(iaVector3d(), iaVector3d(M_PI * 0.5, 0, 0), iaVector3d(1.99, 0.1, 1.99)));
    zRingTransform->addComponent(new iSphereComponent(1.0));
    zRingTransform->addComponent(new iOctreeComponent());
    auto zMeshRenderComponent = zRingTransform->addComponent(new iMeshRenderComponent());
    zMeshRenderComponent->addMesh(ringMesh, _blue);
    zRingTransform->setParent(_roateModifier);

    _rotateBillboardTransform = entityScene->createEntity("ring.billboard");
    _rotateBillboardTransform->addComponent(new iTransformComponent(iaVector3d(), iaVector3d(), iaVector3d()));
    _rotateBillboardTransform->addComponent(new iSphereComponent(1.0));
    _rotateBillboardTransform->addComponent(new iOctreeComponent());
    auto billboardMeshRenderComponent = _rotateBillboardTransform->addComponent(new iMeshRenderComponent());
    billboardMeshRenderComponent->addMesh(ringMesh2D, _cyan);
    _rotateBillboardTransform->setParent(_roateModifier);

    /*_rotateIDs.push_back(xRing->getID());
    _rotateIDs.push_back(yRing->getID());
    _rotateIDs.push_back(zRing->getID());*/

    // add a locator in the middle for better orientation
    /*iNodeTransform *xTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
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
    xCylinder->setMaterial(_red);
    xTransform->insertNode(xCylinder);

    iNodeMesh *yCylinder = iNodeManager::getInstance().createNode<iNodeMesh>();
    yCylinder->setName("manipulator.cylinder.y");
    yCylinder->setMesh(cylinder);
    yCylinder->setMaterial(_green);
    yTransform->insertNode(yCylinder);

    iNodeMesh *zCylinder = iNodeManager::getInstance().createNode<iNodeMesh>();
    zCylinder->setName("manipulator.cylinder.z");
    zCylinder->setMesh(cylinder);
    zCylinder->setMaterial(_blue);
    zTransform->insertNode(zCylinder);*/
}

void TransformOverlay::createLocatorRepresentation(iMeshPtr &cylinder)
{
    /*_locatorRepresentation = iNodeManager::getInstance().createNode<iNode>();
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
    xCylinder->setMaterial(_red);
    xTransform->insertNode(xCylinder);

    iNodeMesh *yCylinder = iNodeManager::getInstance().createNode<iNodeMesh>();
    yCylinder->setName("manipulator.cylinder.y");
    yCylinder->setMesh(cylinder);
    yCylinder->setMaterial(_green);
    yTransform->insertNode(yCylinder);

    iNodeMesh *zCylinder = iNodeManager::getInstance().createNode<iNodeMesh>();
    zCylinder->setName("manipulator.cylinder.z");
    zCylinder->setMesh(cylinder);
    zCylinder->setMaterial(_blue);
    zTransform->insertNode(zCylinder);*/
}

void TransformOverlay::createTranslateModifier(iMeshPtr &translateMesh)
{
    const auto &entitySceneID = getView()->getEntitySceneID();
    auto entityScene = iEntitySystemModule::getInstance().getScene(entitySceneID);
    _translateModifier = entityScene->createEntity();
    _translateModifier->setParent(_rootTransform);

    iEntityPtr xTransform = entityScene->createEntity("transform.x");
    xTransform->addComponent(new iTransformComponent(iaVector3d(), iaVector3d(0, 0, -M_PI * 0.5)));
    xTransform->addComponent(new iSphereComponent(1.0));
    xTransform->addComponent(new iOctreeComponent());
    auto xMeshRenderComponent = xTransform->addComponent(new iMeshRenderComponent());
    xMeshRenderComponent->addMesh(translateMesh, _red);
    xTransform->setParent(_translateModifier);

    iEntityPtr yTransform = entityScene->createEntity("transform.y");
    yTransform->addComponent(new iTransformComponent(iaVector3d(), iaVector3d()));
    yTransform->addComponent(new iSphereComponent(1.0));
    yTransform->addComponent(new iOctreeComponent());
    auto yMeshRenderComponent = yTransform->addComponent(new iMeshRenderComponent());
    yMeshRenderComponent->addMesh(translateMesh, _green);
    yTransform->setParent(_translateModifier);

    iEntityPtr zTransform = entityScene->createEntity("transform.z");
    zTransform->addComponent(new iTransformComponent(iaVector3d(), iaVector3d(M_PI * 0.5, 0, 0)));
    zTransform->addComponent(new iSphereComponent(1.0));
    zTransform->addComponent(new iOctreeComponent());
    auto zMeshRenderComponent = zTransform->addComponent(new iMeshRenderComponent());
    zMeshRenderComponent->addMesh(translateMesh, _blue);
    zTransform->setParent(_translateModifier);

    _translateIDs.push_back(xTransform->getID());
    _translateIDs.push_back(yTransform->getID());
    _translateIDs.push_back(zTransform->getID());
}

void TransformOverlay::createScaleModifier(iMeshPtr &scaleMesh)
{
    /*_scaleModifier = iNodeManager::getInstance().createNode<iNode>();
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
    xCube->setMaterial(_red);
    xTransform->insertNode(xCube);

    iNodeMesh *yCube = iNodeManager::getInstance().createNode<iNodeMesh>();
    yCube->setName("manipulator.cube.y");
    yCube->setMesh(scaleMesh);
    yCube->setMaterial(_green);
    yTransform->insertNode(yCube);

    iNodeMesh *zCube = iNodeManager::getInstance().createNode<iNodeMesh>();
    zCube->setName("manipulator.cube.z");
    zCube->setMesh(scaleMesh);
    zCube->setMaterial(_blue);
    zTransform->insertNode(zCube);

    iMeshPtr cube = createCube();

    iNodeMesh *xyzCube = iNodeManager::getInstance().createNode<iNodeMesh>();
    xyzCube->setName("manipulator.cube.xyz");
    xyzCube->setMesh(cube);
    xyzCube->setMaterial(_cyan);
    _scaleModifier->insertNode(xyzCube);

    _scaleIDs.push_back(xCube->getID());
    _scaleIDs.push_back(yCube->getID());
    _scaleIDs.push_back(zCube->getID());
    _scaleIDs.push_back(xyzCube->getID());*/
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
    meshBuilder.setJoinVertices(true);

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
    meshBuilder.setJoinVertices(false);

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

void TransformOverlay::setOverlayMode(OverlayMode overlayMode)
{
    EntityOverlay::setOverlayMode(overlayMode);

    switch (overlayMode)
    {
    case OverlayMode::None:
        // _switchNode->setActiveChild(_locatorRepresentation);
        break;

    case OverlayMode::Translate:
        _translateModifier->setActiveExclusive(true);
        break;

    case OverlayMode::Scale:
        //   _switchNode->setActiveChild(_scaleModifier);
        break;

    case OverlayMode::Rotate:
        // _switchNode->setActiveChild(_roateModifier);
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
    /*if (_selectedManipulatorNodeID == iNode::INVALID_NODE_ID)
    {
        return;
    }

    const iNodePtr node = iNodeManager::getInstance().getNode(_selectedManipulatorNodeID);

    if (node->getKind() != iNodeKind::Renderable &&
        node->getKind() != iNodeKind::Volume)
    {
        return;
    }

    const iNodeRenderPtr renderNode = static_cast<iNodeRenderPtr>(node);
    iRenderer::getInstance().setModelMatrix(renderNode->getWorldMatrix());

    if (node->getType() != iNodeType::iNodeMesh)
    {
        return;
    }

    iNodeMesh *meshNode = static_cast<iNodeMesh *>(node);
    iRenderer::getInstance().setLineWidth(4);
    iRenderer::getInstance().setShader(_materialCelShading->getShader());
    iRenderer::getInstance().drawMesh(meshNode->getMesh(), _materialCelShading);*/
}

bool TransformOverlay::onMouseKeyUpEvent(iEventMouseKeyUp &event)
{
    /*if (_selectedManipulatorNodeID == iNode::INVALID_NODE_ID)
    {
        return false;
    }

    _selectedManipulatorNodeID = iNode::INVALID_NODE_ID;*/
    return true;
}

bool TransformOverlay::onMouseKeyDownEvent(iEventMouseKeyDown &event)
{
    /*auto rect = getView()->getViewport();
    auto window = iApplication::getInstance().getWindow();

    auto top = window->getClientHeight() - rect._height - rect._y;

    iNodeID nodeID = getView()->pickColorID(event.getPosition()._x - rect._x, event.getPosition()._y - top);

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
    }*/

    return false;
}

bool TransformOverlay::onMouseMoveEvent(iEventMouseMove &event)
{
    /*if (_selectedManipulatorNodeID == iNode::INVALID_NODE_ID)
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

    transformNode->setMatrix(nodeMatrix);*/

    update();

    return false;
}

void TransformOverlay::scale(const iaVector3d &vec, iaMatrixd &matrix)
{
    /*const iaVector3d dir[] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {1, 1, 1}};
    iaVector3d scale;

    for (int i = 0; i < 4; ++i)
    {
        if (_selectedManipulatorNodeID == _scaleIDs[i])
        {
            scale = vec.project(dir[i]) + iaVector3d(1, 1, 1);
            matrix.scale(scale);
            return;
        }
    }*/
}

void TransformOverlay::translate(const iaVector3d &vec, iaMatrixd &matrix)
{
    /*static const iaVector3d dir[] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    iaVector3d translate;

    for (int i = 0; i < 3; ++i)
    {
        if (_selectedManipulatorNodeID == _translateIDs[i])
        {
            translate = vec.project(dir[i]);
            matrix.translate(translate);
            return;
        }
    }*/
}

void TransformOverlay::rotate(const iaVector2d &from, const iaVector2d &to, iaMatrixd &matrix)
{
    /*  iNode *node = iNodeManager::getInstance().getNode(getNodeID());
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
      }*/
}
