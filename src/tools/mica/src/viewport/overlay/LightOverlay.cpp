// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "LightOverlay.h"

LightOverlay::LightOverlay(iViewPtr view)
    : EntityOverlay(view)
{
    onInit();
}

LightOverlay::~LightOverlay()
{
}

bool LightOverlay::accepts(OverlayMode mode, iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");

    return entity->getComponent<iLightComponent>() != nullptr;
}

void LightOverlay::onInit()
{
    getView()->getRenderEvent().add(iPreRenderDelegate(this, &LightOverlay::onPreRender));

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

    const auto &entitySceneID = getView()->getSceneID();
    auto entityScene = iEntitySystemModule::getInstance().getScene(entitySceneID);
    con_assert(entityScene != nullptr, "no scene");

    // adding this is a workaround so components get initialized
    // correctly when deactivating _lightRoot right after creation
    auto root = entityScene->createEntity("overlay.light");

    _lightRoot = entityScene->createEntity("overlay.light.root");
    _lightRoot->setParent(root);

    _lightModifier = entityScene->createEntity("overlay.light.modifier");
    _lightModifier->addComponent(new iTransformComponent());
    _lightModifier->setParent(_lightRoot);

    _lightIcon = entityScene->createEntity("overlay.light.icon");
    _lightIcon->addComponent(new iTransformComponent());
    _lightIcon->addComponent(new iSphereComponent(1.0));
    _lightIcon->addComponent(new iOctreeComponent());
    auto meshRenderComponent = _lightIcon->addComponent(new iMeshRenderComponent());
    meshRenderComponent->addMesh(createSun(), _cyan);
    _lightIcon->setParent(_lightRoot);

    _lightRays = entityScene->createEntity("overlay.light.rays");
    _lightRays->addComponent(new iTransformComponent());
    _lightRays->addComponent(new iSphereComponent(1.0));
    _lightRays->addComponent(new iOctreeComponent());
    meshRenderComponent = _lightRays->addComponent(new iMeshRenderComponent());
    meshRenderComponent->addMesh(createRays(), _cyan);
    _lightRays->setParent(_lightRoot);

    iMeshPtr translateMesh = createTranslateMesh();
    iMeshPtr ringMesh = createRingMesh();
    iMeshPtr cylinder = createCylinder();

    createTranslateModifier(translateMesh);
    createRotateModifier(ringMesh, cylinder);

    _lightRoot->setActive(false);
}

iMeshPtr LightOverlay::createRays()
{
    iMeshBuilder meshBuilder;

    iaMatrixf matrix;

    iaRandom::setSeed(2);

    for (int i = 0; i < 6; ++i)
    {
        matrix.identity();
        matrix.rotate(-M_PI * 0.5, iaAxis::Z);
        matrix.translate(iaRandom::getNextFloatRange(-0.7, 0.7),
                         iaRandom::getNextFloatRange(-4.0, -2.0),
                         iaRandom::getNextFloatRange(-0.7, 0.7));
        meshBuilder.setMatrix(matrix);
        iMeshBuilderUtils::addCylinder(meshBuilder, 0.02, iaRandom::getNextFloatRange(1.0, 1.6), 3);
    }

    meshBuilder.calcNormals(true);
    return meshBuilder.createMesh();
}

iMeshPtr LightOverlay::createSun()
{
    iMeshBuilder meshBuilder;

    iMeshBuilderUtils::addSphere(meshBuilder, 0.2, 16);

    iaMatrixf matrix;

    for (int i = 0; i < 8; ++i)
    {
        matrix.identity();
        matrix.rotate(i * 0.7854, iaAxis::Z);
        matrix.translate(0, 0.3, 0);
        meshBuilder.setMatrix(matrix);
        iMeshBuilderUtils::addCylinder(meshBuilder, 0.02, 0.2, 3);
    }

    meshBuilder.calcNormals(true);
    return meshBuilder.createMesh();
}

void LightOverlay::onUpdateOverlayMode()
{
    iLightType lightType = iLightType::Undefined;

    auto entityScene = iEntitySystemModule::getInstance().getScene(getSceneID());
    if (entityScene != nullptr)
    {
        auto entity = entityScene->getEntity(getEntityID());
        if (entity != nullptr)
        {
            auto lightComp = entity->getComponent<iLightComponent>();
            if (lightComp != nullptr)
            {
                lightType = lightComp->getType();
            }
        }
    }

    switch (getOverlayMode())
    {
    case OverlayMode::None:
    case OverlayMode::Scale:
        _translateModifier->setActive(false);
        _rotateModifier->setActive(false);
        break;

    case OverlayMode::Translate:
        if (lightType == iLightType::Directional)
        {
            _translateModifier->setActive(false);
            _rotateModifier->setActive(false);
        }
        else
        {
            _translateModifier->setActiveExclusive(true);
        }
        break;

    case OverlayMode::Rotate:
        _rotateModifier->setActiveExclusive(true);
        break;
    }
}

void LightOverlay::setActive(bool active)
{
    EntityOverlay::setActive(active);
    _lightRoot->setActive(active);

    if (!isActive())
    {
        return;
    }

    onUpdateOverlayMode();
}

void LightOverlay::onUpdate()
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

    auto lightComp = entity->getComponent<iLightComponent>();
    if (lightComp == nullptr)
    {
        return;
    }

    auto transformComp = entity->getComponent<iTransformComponent>();
    if (transformComp == nullptr)
    {
        return;
    }

    auto camTransformComp = entityScene->getActiveCamera()->getComponent<iTransformComponent>();
    auto lightIconTransformComp = _lightIcon->getComponent<iTransformComponent>();
    auto lightRaysTransformComp = _lightRays->getComponent<iTransformComponent>();
    auto modifierTransformComp = _lightModifier->getComponent<iTransformComponent>();

    switch (lightComp->getType())
    {
    case iLightType::Directional:
    {
        auto camMatrix = camTransformComp->getWorldMatrix();
        const auto lightToCamPos = camMatrix._pos + camMatrix._depth * -10.0;
        lightIconTransformComp->setPosition(lightToCamPos);
        lightIconTransformComp->setOrientation(camTransformComp->getWorldOrientation());
        lightIconTransformComp->setScale(iaVector3d(1.0, 1.0, 1.0));

        _lightRays->setActive(true);
        lightRaysTransformComp->setPosition(lightToCamPos);
        lightRaysTransformComp->setOrientation(transformComp->getWorldOrientation());
        lightRaysTransformComp->setScale(iaVector3d(1.0, 1.0, 1.0));

        modifierTransformComp->setPosition(lightToCamPos);
        modifierTransformComp->setOrientation(transformComp->getWorldOrientation());
        modifierTransformComp->setScale(iaVector3d(1.0, 1.0, 1.0));
    }
    break;

    case iLightType::Point:
    {
        const auto &entityPos = transformComp->getWorldPosition();
        const float64 distanceToCam = camTransformComp->getWorldPosition().distance(entityPos) * 0.1;

        lightIconTransformComp->setPosition(entityPos);
        lightIconTransformComp->setOrientation(camTransformComp->getWorldOrientation());
        lightIconTransformComp->setScale(iaVector3d(distanceToCam, distanceToCam, distanceToCam));

        _lightRays->setActive(false);

        modifierTransformComp->setPosition(entityPos);
        modifierTransformComp->setOrientation(transformComp->getWorldOrientation());
        modifierTransformComp->setScale(iaVector3d(distanceToCam, distanceToCam, distanceToCam));
    }
    break;
    };
}

void LightOverlay::onPreRender()
{
    onUpdate();
}

void LightOverlay::createRotateModifier(iMeshPtr &ringMesh, iMeshPtr &cylinderMesh)
{
    const auto &entitySceneID = getView()->getSceneID();
    auto entityScene = iEntitySystemModule::getInstance().getScene(entitySceneID);
    _rotateModifier = entityScene->createEntity("overlay.light.modifier.rotate");
    _rotateModifier->setParent(_lightModifier);

    iEntityPtr xRingTransform = entityScene->createEntity("overlay.light.modifier.rotate.x");
    xRingTransform->addComponent(new iTransformComponent(iaVector3d(-0.05, -0.05, -0.05), iaQuaterniond::fromEuler(0, 0, -M_PI * 0.5)));
    xRingTransform->addComponent(new iSphereComponent(1.0));
    xRingTransform->addComponent(new iOctreeComponent());
    auto xMeshRenderComponent = xRingTransform->addComponent(new iMeshRenderComponent());
    xMeshRenderComponent->addMesh(ringMesh, _red);
    xRingTransform->setParent(_rotateModifier);

    iEntityPtr yRingTransform = entityScene->createEntity("overlay.light.modifier.rotate.y");
    yRingTransform->addComponent(new iTransformComponent(iaVector3d(-0.05, -0.05, -0.05), iaQuaterniond(), iaVector3d(0.99, 1.0, 0.99)));
    yRingTransform->addComponent(new iSphereComponent(1.0));
    yRingTransform->addComponent(new iOctreeComponent());
    auto yMeshRenderComponent = yRingTransform->addComponent(new iMeshRenderComponent());
    yMeshRenderComponent->addMesh(ringMesh, _green);
    yRingTransform->setParent(_rotateModifier);

    iEntityPtr zRingTransform = entityScene->createEntity("overlay.light.modifier.rotate.z");
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
    iEntityPtr xCylinderTransform = entityScene->createEntity("overlay.light.modifier.rotate.cylinder.x");
    xCylinderTransform->addComponent(new iTransformComponent(iaVector3d(), iaQuaterniond::fromEuler(0, 0, -M_PI * 0.5)));
    xCylinderTransform->addComponent(new iSphereComponent(1.0));
    xCylinderTransform->addComponent(new iOctreeComponent());
    auto xCylinderMeshRenderComponent = xCylinderTransform->addComponent(new iMeshRenderComponent());
    xCylinderMeshRenderComponent->addMesh(cylinderMesh, _red);
    xCylinderTransform->setParent(_rotateModifier);

    iEntityPtr yCylinderTransform = entityScene->createEntity("overlay.light.modifier.rotate.cylinder.y");
    yCylinderTransform->addComponent(new iTransformComponent());
    yCylinderTransform->addComponent(new iSphereComponent(1.0));
    yCylinderTransform->addComponent(new iOctreeComponent());
    auto yCylinderMeshRenderComponent = yCylinderTransform->addComponent(new iMeshRenderComponent());
    yCylinderMeshRenderComponent->addMesh(cylinderMesh, _green);
    yCylinderTransform->setParent(_rotateModifier);

    iEntityPtr zCylinderTransform = entityScene->createEntity("overlay.light.modifier.rotate.cylinder.z");
    zCylinderTransform->addComponent(new iTransformComponent(iaVector3d(), iaQuaterniond::fromEuler(M_PI * 0.5, 0, 0)));
    zCylinderTransform->addComponent(new iSphereComponent(1.0));
    zCylinderTransform->addComponent(new iOctreeComponent());
    auto zCylinderMeshRenderComponent = zCylinderTransform->addComponent(new iMeshRenderComponent());
    zCylinderMeshRenderComponent->addMesh(cylinderMesh, _blue);
    zCylinderTransform->setParent(_rotateModifier);
}

void LightOverlay::createTranslateModifier(iMeshPtr &translateMesh)
{
    const auto &entitySceneID = getView()->getSceneID();
    auto entityScene = iEntitySystemModule::getInstance().getScene(entitySceneID);
    _translateModifier = entityScene->createEntity("overlay.light.modifier.translate");
    _translateModifier->setParent(_lightModifier);

    iEntityPtr xTransform = entityScene->createEntity("overlay.light.modifier.translate.x");
    xTransform->addComponent(new iTransformComponent(iaVector3d(), iaQuaterniond::fromEuler(0, 0, -M_PI * 0.5)));
    xTransform->addComponent(new iSphereComponent(1.0));
    xTransform->addComponent(new iOctreeComponent());
    auto xMeshRenderComponent = xTransform->addComponent(new iMeshRenderComponent());
    xMeshRenderComponent->addMesh(translateMesh, _red);
    xTransform->setParent(_translateModifier);

    iEntityPtr yTransform = entityScene->createEntity("overlay.light.modifier.translate.y");
    yTransform->addComponent(new iTransformComponent());
    yTransform->addComponent(new iSphereComponent(1.0));
    yTransform->addComponent(new iOctreeComponent());
    auto yMeshRenderComponent = yTransform->addComponent(new iMeshRenderComponent());
    yMeshRenderComponent->addMesh(translateMesh, _green);
    yTransform->setParent(_translateModifier);

    iEntityPtr zTransform = entityScene->createEntity("overlay.light.modifier.translate.z");
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

void LightOverlay::setOverlayMode(OverlayMode overlayMode)
{
    EntityOverlay::setOverlayMode(overlayMode);

    if (!isActive())
    {
        return;
    }

    onUpdateOverlayMode();
}

bool LightOverlay::onMouseKeyUpEvent(const iEventMouseKeyUp &event)
{
    if (!_selectionID.isValid())
    {
        return false;
    }

    _selectionID = iEntityID::getInvalid();
    return true;
}

bool LightOverlay::onMouseKeyDownEvent(const iEventMouseKeyDown &event)
{
    auto rect = getView()->getViewport();
    auto window = iApplication::getInstance().getWindow();

    auto top = window->getClientHeight() - rect._height - rect._y;

    iEntityID selectionID = getView()->pickEntityID(event.getPosition()._x - rect._x, event.getPosition()._y - top);

    _selectionID = iEntityID::getInvalid();

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

bool LightOverlay::onMouseMoveEvent(const iEventMouseMove &event)
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
        break;
    case OverlayMode::Translate:
        translate((to - from) * distanceToCam, entityTransformComp);
        break;
    }

    return false;
}

void LightOverlay::translate(const iaVector3d &vec, iTransformComponentPtr transform)
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

void LightOverlay::rotate(const iaVector3d &localFrom, const iaVector3d &localTo, iTransformComponentPtr transform)
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