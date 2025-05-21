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

void TransformOverlay::setEntity(const iEntitySceneID &entitySceneID, const iEntityID &entityID)
{
    EntityOverlay::setEntity(entitySceneID, entityID);
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
    getView()->getRenderEvent().remove(iPreRenderDelegate(this, &TransformOverlay::onPreRender));

    _red = nullptr;
    _green = nullptr;
    _blue = nullptr;
    _cyan = nullptr;
    _materialCelShading = nullptr;
}

void TransformOverlay::onInit()
{
    getView()->getRenderEvent().add(iRenderDelegate(this, &TransformOverlay::onRender));
    getView()->getRenderEvent().add(iPreRenderDelegate(this, &TransformOverlay::onPreRender));

    iShaderPtr shader = iResourceManager::getInstance().loadResource<iShader>("igor_shader_material_transform_overlay_base");
    iParameters paramMaterial({
        {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
        {IGOR_RESOURCE_PARAM_GENERATE, true},

    });

    const float32 alpha = 1.0f;

    iParameters paramRed({
        {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
        {IGOR_RESOURCE_PARAM_GENERATE, true},
        {IGOR_RESOURCE_PARAM_SHADER, shader},
        {IGOR_RESOURCE_PARAM_AMBIENT, iaColor3f(0.3f, 0.0f, 0.0f)},
        {IGOR_RESOURCE_PARAM_DIFFUSE, iaColor3f(0.5f, 0.0f, 0.0f)},
        {IGOR_RESOURCE_PARAM_SPECULAR, iaColor3f(0.2f, 0.0f, 0.0f)},
        {IGOR_RESOURCE_PARAM_EMISSIVE, iaColor3f(0.8f, 0.0f, 0.0f)},
        {IGOR_RESOURCE_PARAM_ALPHA, alpha},
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
        {IGOR_RESOURCE_PARAM_ALPHA, alpha},
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
        {IGOR_RESOURCE_PARAM_ALPHA, alpha},
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
        {IGOR_RESOURCE_PARAM_ALPHA, alpha},
    });
    _cyan = iResourceManager::getInstance().loadResource<iMaterial>(paramCyan);

    _materialCelShading = iResourceManager::getInstance().loadResource<iShader>("igor_shader_material_cellshading_yellow");

    iMeshPtr translateMesh = createTranslateMesh();
    iMeshPtr scaleMesh = createScaleMesh();
    iMeshPtr ringMesh = createRingMesh();
    iMeshPtr ringMesh2D = create2DRingMesh();
    iMeshPtr cylinder = createCylinder();

    const auto &entitySceneID = getView()->getSceneID();
    auto entityScene = iEntitySystemModule::getInstance().getScene(entitySceneID);
    con_assert(entityScene != nullptr, "no scene");
    auto root = entityScene->createEntity("overlay.root");
    _rootTransform = entityScene->createEntity("overlay.transform.root");
    _rootTransform->addComponent(new iTransformComponent());
    _rootTransform->setParent(root);

    createTranslateModifier(translateMesh);
    createScaleModifier(scaleMesh);
    createRotateModifier(ringMesh, ringMesh2D, cylinder);
    createLocatorRepresentation(cylinder);

    _rootTransform->setActive(false);
}

void TransformOverlay::setActive(bool active)
{
    EntityOverlay::setActive(active);
    _rootTransform->setActive(active);
}

void TransformOverlay::update()
{
    if (!isActive())
    {
        return;
    }

    auto entityScene = iEntitySystemModule::getInstance().getScene(getSceneID());
    if (entityScene == nullptr)
    {
        return;
    }

    auto entity = entityScene->getEntity(getEntityID());
    if (entity == nullptr)
    {
        return;
    }

    auto transformComp = entity->getComponent<iTransformComponent>();
    if (transformComp == nullptr)
    {
        return;
    }

    auto camTransformComp = entityScene->getActiveCamera()->getComponent<iTransformComponent>();
    auto camWorldPosition = camTransformComp->getWorldPosition();
    auto camWorldOrientation = camTransformComp->getWorldOrientation();

    auto entityWorldMatrix = transformComp->getWorldMatrix();
    const auto &entityPos = entityWorldMatrix._pos;
    const auto &entityOrientation = transformComp->getOrientation();

    float64 distanceToCam = camWorldPosition.distance(entityPos) * 0.1;

    // update transform
    auto rootTransformComp = _rootTransform->getComponent<iTransformComponent>();
    rootTransformComp->setPosition(entityPos);
    rootTransformComp->setOrientation(entityOrientation);
    rootTransformComp->setScale(iaVector3d(distanceToCam, distanceToCam, distanceToCam));

    auto billboardTransformComp = _rotateBillboardTransform->getComponent<iTransformComponent>();
    billboardTransformComp->setOrientation(camWorldOrientation * iaQuaterniond::fromEuler(90 * IGOR_GRAD2RAD,0,0));
}

void TransformOverlay::createRotateModifier(iMeshPtr &ringMesh, iMeshPtr &ringMesh2D, iMeshPtr &cylinderMesh)
{
    const auto &entitySceneID = getView()->getSceneID();
    auto entityScene = iEntitySystemModule::getInstance().getScene(entitySceneID);
    _rotateModifier = entityScene->createEntity("overlay.rotate");
    _rotateModifier->setParent(_rootTransform);

    iEntityPtr xRingTransform = entityScene->createEntity("overlay.rotate.x");
    xRingTransform->addComponent(new iTransformComponent(iaVector3d(-0.05, -0.05, -0.05), iaQuaterniond::fromEuler(0, 0, -M_PI * 0.5)));
    xRingTransform->addComponent(new iSphereComponent(1.0));
    xRingTransform->addComponent(new iOctreeComponent());
    auto xMeshRenderComponent = xRingTransform->addComponent(new iMeshRenderComponent());
    xMeshRenderComponent->addMesh(ringMesh, _red);
    xRingTransform->setParent(_rotateModifier);

    iEntityPtr yRingTransform = entityScene->createEntity("overlay.rotate.y");
    yRingTransform->addComponent(new iTransformComponent(iaVector3d(-0.05, -0.05, -0.05), iaQuaterniond(), iaVector3d(0.99, 1.0, 0.99)));
    yRingTransform->addComponent(new iSphereComponent(1.0));
    yRingTransform->addComponent(new iOctreeComponent());
    auto yMeshRenderComponent = yRingTransform->addComponent(new iMeshRenderComponent());
    yMeshRenderComponent->addMesh(ringMesh, _green);
    yRingTransform->setParent(_rotateModifier);

    iEntityPtr zRingTransform = entityScene->createEntity("overlay.rotate.z");
    zRingTransform->addComponent(new iTransformComponent(iaVector3d(-0.05, -0.05, -0.05), iaQuaterniond::fromEuler(M_PI * 0.5, 0, 0), iaVector3d(0.98, 1.0, 0.98)));
    zRingTransform->addComponent(new iSphereComponent(1.0));
    zRingTransform->addComponent(new iOctreeComponent());
    auto zMeshRenderComponent = zRingTransform->addComponent(new iMeshRenderComponent());
    zMeshRenderComponent->addMesh(ringMesh, _blue);
    zRingTransform->setParent(_rotateModifier);

    _rotateBillboardTransform = entityScene->createEntity("overlay.rotate.billboard");
    _rotateBillboardTransform->addComponent(new iTransformComponent());
    _rotateBillboardTransform->addComponent(new iSphereComponent(1.0));
    _rotateBillboardTransform->addComponent(new iOctreeComponent());
    auto billboardMeshRenderComponent = _rotateBillboardTransform->addComponent(new iMeshRenderComponent());
    billboardMeshRenderComponent->addMesh(ringMesh2D, _cyan);
    _rotateBillboardTransform->setParent(_rotateModifier);

    _rotateIDs.push_back(xRingTransform->getID());
    _rotateIDs.push_back(yRingTransform->getID());
    _rotateIDs.push_back(zRingTransform->getID());

    // add a locator in the middle for better orientation
    iEntityPtr xCylinderTransform = entityScene->createEntity("overlay.rotate.cylinder.x");
    xCylinderTransform->addComponent(new iTransformComponent(iaVector3d(), iaQuaterniond::fromEuler(0, 0, -M_PI * 0.5)));
    xCylinderTransform->addComponent(new iSphereComponent(1.0));
    xCylinderTransform->addComponent(new iOctreeComponent());
    auto xCylinderMeshRenderComponent = xCylinderTransform->addComponent(new iMeshRenderComponent());
    xCylinderMeshRenderComponent->addMesh(cylinderMesh, _red);
    xCylinderTransform->setParent(_rotateModifier);

    iEntityPtr yCylinderTransform = entityScene->createEntity("overlay.rotate.cylinder.y");
    yCylinderTransform->addComponent(new iTransformComponent());
    yCylinderTransform->addComponent(new iSphereComponent(1.0));
    yCylinderTransform->addComponent(new iOctreeComponent());
    auto yCylinderMeshRenderComponent = yCylinderTransform->addComponent(new iMeshRenderComponent());
    yCylinderMeshRenderComponent->addMesh(cylinderMesh, _green);
    yCylinderTransform->setParent(_rotateModifier);

    iEntityPtr zCylinderTransform = entityScene->createEntity("overlay.rotate.cylinder.z");
    zCylinderTransform->addComponent(new iTransformComponent(iaVector3d(), iaQuaterniond::fromEuler(M_PI * 0.5, 0, 0)));
    zCylinderTransform->addComponent(new iSphereComponent(1.0));
    zCylinderTransform->addComponent(new iOctreeComponent());
    auto zCylinderMeshRenderComponent = zCylinderTransform->addComponent(new iMeshRenderComponent());
    zCylinderMeshRenderComponent->addMesh(cylinderMesh, _blue);
    zCylinderTransform->setParent(_rotateModifier);
}

void TransformOverlay::createLocatorRepresentation(iMeshPtr &cylinderMesh)
{
    const auto &entitySceneID = getView()->getSceneID();
    auto entityScene = iEntitySystemModule::getInstance().getScene(entitySceneID);
    _locatorRepresentation = entityScene->createEntity("overlay.locator");
    _locatorRepresentation->setParent(_rootTransform);

    iEntityPtr xCylinderTransform = entityScene->createEntity("overlay.locator.x");
    xCylinderTransform->addComponent(new iTransformComponent(iaVector3d(), iaQuaterniond::fromEuler(0, 0, -M_PI * 0.5)));
    xCylinderTransform->addComponent(new iSphereComponent(1.0));
    xCylinderTransform->addComponent(new iOctreeComponent());
    auto xCylinderMeshRenderComponent = xCylinderTransform->addComponent(new iMeshRenderComponent());
    xCylinderMeshRenderComponent->addMesh(cylinderMesh, _red);
    xCylinderTransform->setParent(_locatorRepresentation);

    iEntityPtr yCylinderTransform = entityScene->createEntity("overlay.locator.y");
    yCylinderTransform->addComponent(new iTransformComponent());
    yCylinderTransform->addComponent(new iSphereComponent(1.0));
    yCylinderTransform->addComponent(new iOctreeComponent());
    auto yCylinderMeshRenderComponent = yCylinderTransform->addComponent(new iMeshRenderComponent());
    yCylinderMeshRenderComponent->addMesh(cylinderMesh, _green);
    yCylinderTransform->setParent(_locatorRepresentation);

    iEntityPtr zCylinderTransform = entityScene->createEntity("overlay.locator.z");
    zCylinderTransform->addComponent(new iTransformComponent(iaVector3d(), iaQuaterniond::fromEuler(M_PI * 0.5, 0, 0)));
    zCylinderTransform->addComponent(new iSphereComponent(1.0));
    zCylinderTransform->addComponent(new iOctreeComponent());
    auto zCylinderMeshRenderComponent = zCylinderTransform->addComponent(new iMeshRenderComponent());
    zCylinderMeshRenderComponent->addMesh(cylinderMesh, _blue);
    zCylinderTransform->setParent(_locatorRepresentation);
}

void TransformOverlay::createTranslateModifier(iMeshPtr &translateMesh)
{
    const auto &entitySceneID = getView()->getSceneID();
    auto entityScene = iEntitySystemModule::getInstance().getScene(entitySceneID);
    _translateModifier = entityScene->createEntity("overlay.translate");
    _translateModifier->setParent(_rootTransform);

    iEntityPtr xTransform = entityScene->createEntity("overlay.translate.x");
    xTransform->addComponent(new iTransformComponent(iaVector3d(), iaQuaterniond::fromEuler(0, 0, -M_PI * 0.5)));
    xTransform->addComponent(new iSphereComponent(1.0));
    xTransform->addComponent(new iOctreeComponent());
    auto xMeshRenderComponent = xTransform->addComponent(new iMeshRenderComponent());
    xMeshRenderComponent->addMesh(translateMesh, _red);
    xTransform->setParent(_translateModifier);

    iEntityPtr yTransform = entityScene->createEntity("overlay.translate.y");
    yTransform->addComponent(new iTransformComponent());
    yTransform->addComponent(new iSphereComponent(1.0));
    yTransform->addComponent(new iOctreeComponent());
    auto yMeshRenderComponent = yTransform->addComponent(new iMeshRenderComponent());
    yMeshRenderComponent->addMesh(translateMesh, _green);
    yTransform->setParent(_translateModifier);

    iEntityPtr zTransform = entityScene->createEntity("overlay.translate.z");
    zTransform->addComponent(new iTransformComponent(iaVector3d(), iaQuaterniond::fromEuler(M_PI * 0.5, 0, 0)));
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
    const auto &entitySceneID = getView()->getSceneID();
    auto entityScene = iEntitySystemModule::getInstance().getScene(entitySceneID);
    _scaleModifier = entityScene->createEntity("overlay.scale");
    _scaleModifier->setParent(_rootTransform);

    iEntityPtr xTransform = entityScene->createEntity("overlay.scale.x");
    xTransform->addComponent(new iTransformComponent(iaVector3d(), iaQuaterniond::fromEuler(0, 0, -M_PI * 0.5)));
    xTransform->addComponent(new iSphereComponent(1.0));
    xTransform->addComponent(new iOctreeComponent());
    auto xMeshRenderComponent = xTransform->addComponent(new iMeshRenderComponent());
    xMeshRenderComponent->addMesh(scaleMesh, _red);
    xTransform->setParent(_scaleModifier);

    iEntityPtr yTransform = entityScene->createEntity("overlay.scale.y");
    yTransform->addComponent(new iTransformComponent());
    yTransform->addComponent(new iSphereComponent(1.0));
    yTransform->addComponent(new iOctreeComponent());
    auto yMeshRenderComponent = yTransform->addComponent(new iMeshRenderComponent());
    yMeshRenderComponent->addMesh(scaleMesh, _green);
    yTransform->setParent(_scaleModifier);

    iEntityPtr zTransform = entityScene->createEntity("overlay.scale.z");
    zTransform->addComponent(new iTransformComponent(iaVector3d(), iaQuaterniond::fromEuler(M_PI * 0.5, 0, 0)));
    zTransform->addComponent(new iSphereComponent(1.0));
    zTransform->addComponent(new iOctreeComponent());
    auto zMeshRenderComponent = zTransform->addComponent(new iMeshRenderComponent());
    zMeshRenderComponent->addMesh(scaleMesh, _blue);
    zTransform->setParent(_scaleModifier);

    iMeshPtr cube = createCube();
    iEntityPtr xyzCube = entityScene->createEntity("overlay.scale.cube");
    xyzCube->addComponent(new iTransformComponent(iaVector3d(0.125, 0, 0.125)));
    xyzCube->addComponent(new iSphereComponent(1.0));
    xyzCube->addComponent(new iOctreeComponent());
    auto cubeRenderComponent = xyzCube->addComponent(new iMeshRenderComponent());
    cubeRenderComponent->addMesh(cube, _cyan);
    xyzCube->setParent(_scaleModifier);

    _scaleIDs.push_back(xTransform->getID());
    _scaleIDs.push_back(yTransform->getID());
    _scaleIDs.push_back(zTransform->getID());
    _scaleIDs.push_back(xyzCube->getID());
}

void TransformOverlay::setOverlayMode(OverlayMode overlayMode)
{
    EntityOverlay::setOverlayMode(overlayMode);

    switch (overlayMode)
    {
    case OverlayMode::None:
        _locatorRepresentation->setActiveExclusive(true);
        break;

    case OverlayMode::Translate:
        _translateModifier->setActiveExclusive(true);
        break;

    case OverlayMode::Scale:
        _scaleModifier->setActiveExclusive(true);
        break;

    case OverlayMode::Rotate:
        _rotateModifier->setActiveExclusive(true);
        break;
    }
}

void TransformOverlay::onPreRender()
{
    update();
}

void TransformOverlay::onRender()
{
    renderHighlight();
}

void TransformOverlay::renderHighlight()
{
    const auto &entitySceneID = getView()->getSceneID();
    auto entityScene = iEntitySystemModule::getInstance().getScene(entitySceneID);
    con_assert(entityScene != nullptr, "no scene");

    auto entity = entityScene->getEntity(_selectionID);
    if (entity == nullptr)
    {
        return;
    }

    const auto transformComponent = entity->getComponent<iTransformComponent>();
    if (transformComponent == nullptr)
    {
        return;
    }

    const auto meshRenderComponent = entity->getComponent<iMeshRenderComponent>();
    if (meshRenderComponent == nullptr)
    {
        return;
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

bool TransformOverlay::onMouseKeyUpEvent(iEventMouseKeyUp &event)
{
    if (!_selectionID.isValid())
    {
        return false;
    }

    _selectionID = iEntityID::getInvalid();
    return true;
}

bool TransformOverlay::onMouseKeyDownEvent(iEventMouseKeyDown &event)
{
    auto rect = getView()->getViewport();
    auto window = iApplication::getInstance().getWindow();

    auto top = window->getClientHeight() - rect._height - rect._y;

    iEntityID selectionID = getView()->pickEntityID(event.getPosition()._x - rect._x, event.getPosition()._y - top);

    _selectionID = iEntityID::getInvalid();

    for (int i = 0; i < 4; ++i)
    {
        if (selectionID == _scaleIDs[i])
        {
            _selectionID = selectionID;
            return true;
        }
    }

    for (int i = 0; i < 3; ++i)
    {
        if (selectionID == _translateIDs[i])
        {
            _selectionID = selectionID;
            return true;
        }

        if (selectionID == _rotateIDs[i])
        {
            _selectionID = selectionID;
            return true;
        }
    }

    return false;
}

bool TransformOverlay::onMouseMoveEvent(iEventMouseMove &event)
{
    if(!_selectionID.isValid())
    {
        return false;
    }

    auto entityScene = iEntitySystemModule::getInstance().getScene(getSceneID());
    con_assert(entityScene != nullptr, "no scene");

    auto entity = entityScene->getEntity(getEntityID());
    if(entity == nullptr)
    {
        return false;
    }

    auto entityTransformComp = entity->getComponent<iTransformComponent>();
    if(entityTransformComp == nullptr)
    {
        return false;
    }

    auto camera = entityScene->getActiveCamera();
    if(camera == nullptr)
    {
        return false;
    }

    auto camTransformComp = camera->getComponent<iTransformComponent>();
    if(camTransformComp == nullptr)
    {
        return false;
    }

    const iaMatrixd &camWorldMatrix = camTransformComp->getWorldMatrix();
    iaVector2d fromd = event.getLastPosition().convert<float64>();
    iaVector2d tod = event.getPosition().convert<float64>();
    
    iaVector3d fromWorld = camWorldMatrix * getView()->unProject(iaVector3d(fromd._x, fromd._y, 0), camWorldMatrix);
    iaVector3d toWorld = camWorldMatrix * getView()->unProject(iaVector3d(tod._x, tod._y, 0), camWorldMatrix);

    iaMatrixd transformWorldMatrix = entityTransformComp->getWorldMatrix();
    transformWorldMatrix.invert();
    fromWorld = transformWorldMatrix * fromWorld;
    toWorld = transformWorldMatrix * toWorld;

    float64 distanceToCam = camWorldMatrix._pos.distance(entityTransformComp->getPosition());

    switch (getOverlayMode())
    {
    case OverlayMode::None:
        break;
    case OverlayMode::Rotate:
        //rotate(fromd, tod, nodeMatrix);
        break;
    case OverlayMode::Scale:
        //scale((toWorld - fromWorld) * distanceToCam * 2, nodeMatrix);
        break;
    case OverlayMode::Translate:
        translate((toWorld - fromWorld) * distanceToCam, entityTransformComp);
        break;
    }

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

void TransformOverlay::translate(const iaVector3d &vec, iTransformComponentPtr transform)
{
    static const iaVector3d dir[] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    iaVector3d translate;

    for (int i = 0; i < 3; ++i)
    {
        if (_selectionID == _translateIDs[i])
        {
            translate = vec.project(dir[i]);
            transform->setPosition(transform->getPosition() + translate);
            return;
        }
    }
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

iMeshPtr TransformOverlay::createRingMesh()
{
    iMeshBuilder meshBuilder;
    iMeshBuilderUtils::addCylinder(meshBuilder, 2.0, 0.1, 64, false);
    meshBuilder.calcNormals(true);
    return meshBuilder.createMesh();
}

iMeshPtr TransformOverlay::create2DRingMesh()
{
    iMeshBuilder meshBuilder;
    iMeshBuilderUtils::addRing(meshBuilder, 2.1, 2.15, 64);
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