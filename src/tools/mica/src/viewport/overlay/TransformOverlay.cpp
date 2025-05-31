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
    getView()->getRenderEvent().remove(iPreRenderDelegate(this, &TransformOverlay::onPreRender));

    _red = nullptr;
    _green = nullptr;
    _blue = nullptr;
    _cyan = nullptr;
}

void TransformOverlay::onInit()
{
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

    iMeshPtr translateMesh = createTranslateMesh();
    iMeshPtr scaleMesh = createScaleMesh();
    iMeshPtr ringMesh = createRingMesh();
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
    createRotateModifier(ringMesh, cylinder);

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
        setActive(false);
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
}

void TransformOverlay::createRotateModifier(iMeshPtr &ringMesh, iMeshPtr &cylinderMesh)
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
    xyzCube->addComponent(new iTransformComponent(iaVector3d(0.1, 0, 0.1)));
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
    if (!_selectionID.isValid())
    {
        return false;
    }

    auto entityScene = iEntitySystemModule::getInstance().getScene(getSceneID());
    con_assert(entityScene != nullptr, "no scene");

    auto entity = entityScene->getEntity(getEntityID());
    if (entity == nullptr)
    {
        return false;
    }

    auto entityTransformComp = entity->getComponent<iTransformComponent>();
    if (entityTransformComp == nullptr)
    {
        return false;
    }

    auto camera = entityScene->getActiveCamera();
    if (camera == nullptr)
    {
        return false;
    }

    auto camTransformComp = camera->getComponent<iTransformComponent>();
    if (camTransformComp == nullptr)
    {
        return false;
    }

    const iaMatrixd &camWorldMatrix = camTransformComp->getWorldMatrix();
    iaVector2d fromd = event.getLastPosition().convert<float64>();
    iaVector2d tod = event.getPosition().convert<float64>();

    iaVector3d from = camWorldMatrix * getView()->unProject(iaVector3d(fromd._x, fromd._y, 0), camWorldMatrix);
    iaVector3d to = camWorldMatrix * getView()->unProject(iaVector3d(tod._x, tod._y, 0), camWorldMatrix);

    iaMatrixd transformWorldMatrix = entityTransformComp->getWorldMatrix();
    transformWorldMatrix.invert();
    from = transformWorldMatrix * from;
    to = transformWorldMatrix * to;

    float64 distanceToCam = camWorldMatrix._pos.distance(entityTransformComp->getPosition());

    switch (getOverlayMode())
    {
    case OverlayMode::None:
        break;
    case OverlayMode::Rotate:
        rotate(from, to, entityTransformComp);
        break;
    case OverlayMode::Scale:
        scale((to - from) * distanceToCam * 2, entityTransformComp);
        break;
    case OverlayMode::Translate:
        translate((to - from) * distanceToCam, entityTransformComp);
        break;
    }

    return false;
}

void TransformOverlay::scale(const iaVector3d &vec, iTransformComponentPtr transform)
{
    auto iter = std::find(_scaleIDs.begin(), _scaleIDs.end(), _selectionID);
    if (iter == _scaleIDs.end())
    {
        return;
    }

    int axisIndex = std::distance(_scaleIDs.begin(), iter);
    static const iaVector3d axis[] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {1, 1, 1}};
    
    iaVector3d scale = vec.project(axis[axisIndex]) + iaVector3d(1, 1, 1);
    scale = transform->getScale() * scale;
    transform->setScale(scale);
}

void TransformOverlay::translate(const iaVector3d &vec, iTransformComponentPtr transform)
{
    auto iter = std::find(_translateIDs.begin(), _translateIDs.end(), _selectionID);
    if (iter == _translateIDs.end())
    {
        return;
    }

    int axisIndex = std::distance(_translateIDs.begin(), iter);
    static const iaVector3d axis[] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

    iaVector3d translate = transform->getOrientation().rotate(vec.project(axis[axisIndex])) * transform->getScale();
    transform->setPosition(transform->getPosition() + translate);
}

void TransformOverlay::rotate(const iaVector3d &localFrom, const iaVector3d &localTo, iTransformComponentPtr transform)
{
    auto iter = std::find(_rotateIDs.begin(), _rotateIDs.end(), _selectionID);
    if (iter == _rotateIDs.end())
    {
        return;
    }

    int axisIndex = std::distance(_rotateIDs.begin(), iter);
    static const iaVector3d axis[] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    const iaVector3d localAxis = axis[axisIndex];

    const auto projectedFrom = localFrom - (localAxis * localFrom.dot(localAxis));
    const auto projectedTo = localTo - (localAxis * localTo.dot(localAxis));

    float64 angle = projectedFrom.angle(projectedTo) * 20;

    angle = ((projectedFrom % projectedTo).dot(localAxis) < 0) ? -angle : angle;

    const iaQuaterniond q = iaQuaterniond::fromAxisAngle(localAxis, angle);
    transform->setOrientation(transform->getOrientation() * q);
}

iMeshPtr TransformOverlay::createRingMesh()
{
    iMeshBuilder meshBuilder;
    iMeshBuilderUtils::addCylinder(meshBuilder, 2.0, 0.13, 64, false);
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
    matrix.scale(0.2, 0.2, 0.2);
    meshBuilder.setMatrix(matrix);
    iMeshBuilderUtils::addBox(meshBuilder, 1, 1, 1);

    meshBuilder.calcNormals(true);
    return meshBuilder.createMesh();
}

iMeshPtr TransformOverlay::createCube()
{
    iMeshBuilder meshBuilder;

    iaMatrixf matrix;
    matrix.scale(0.2, 0.2, 0.2);
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