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
    onDeinit();
}

bool LightOverlay::accepts(OverlayMode mode, iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");
    return entity->getComponent<iLightComponent>() != nullptr;
}

void LightOverlay::onDeinit()
{
}

void LightOverlay::onInit()
{
    getView()->getRenderEvent().add(iPreRenderDelegate(this, &LightOverlay::onPreRender));

    iShaderPtr shader = iResourceManager::getInstance().loadResource<iShader>("igor_shader_material_transform_overlay_base");
    iParameters paramMaterial({
        {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
        {IGOR_RESOURCE_PARAM_GENERATE, true},

    });

    iParameters paramCyan({
        {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
        {IGOR_RESOURCE_PARAM_GENERATE, true},
        {IGOR_RESOURCE_PARAM_SHADER, shader},
        {IGOR_RESOURCE_PARAM_AMBIENT, iaColor3f(0.0f, 0.3f, 0.3f)},
        {IGOR_RESOURCE_PARAM_DIFFUSE, iaColor3f(0.0f, 0.5f, 0.5f)},
        {IGOR_RESOURCE_PARAM_SPECULAR, iaColor3f(0.0f, 0.2f, 0.2f)},
        {IGOR_RESOURCE_PARAM_EMISSIVE, iaColor3f(0.0f, 0.8f, 0.8f)},
        {IGOR_RESOURCE_PARAM_ALPHA, 1.0f},
    });
    _cyan = iResourceManager::getInstance().loadResource<iMaterial>(paramCyan);

    const auto &entitySceneID = getView()->getSceneID();
    auto entityScene = iEntitySystemModule::getInstance().getScene(entitySceneID);
    con_assert(entityScene != nullptr, "no scene");

    _lightRoot = entityScene->createEntity("overlay.light.root");
    _lightRoot->addComponent(new iTransformComponent());
    _lightRoot->addComponent(new iSphereComponent(1.0));
    _lightRoot->addComponent(new iOctreeComponent());
    auto meshRenderComponent = _lightRoot->addComponent(new iMeshRenderComponent());
    iMeshPtr sun = createSun();
    meshRenderComponent->addMesh(sun, _cyan);

    auto root = entityScene->createEntity("overlay.root");
    _lightRoot->setParent(root);

    _lightRoot->setActive(false);
}

iMeshPtr LightOverlay::createSun()
{
    iMeshBuilder meshBuilder;
    meshBuilder.setJoinVertices(false);

    iMeshBuilderUtils::addSphere(meshBuilder, 0.2, 16);

    iaMatrixf matrix;

    for (int i = 0; i < 8; ++i)
    {
        matrix.identity();
        matrix.rotate(i * 0.7854, iaAxis::Z);
        matrix.translate(0,0.3,0);
        meshBuilder.setMatrix(matrix);
        iMeshBuilderUtils::addCylinder(meshBuilder, 0.02, 0.2, 3);
    }

    meshBuilder.calcNormals(true);
    return meshBuilder.createMesh();
}

void LightOverlay::setActive(bool active)
{
    EntityOverlay::setActive(active);
    _lightRoot->setActive(active);
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

    auto transformComp = entity->getComponent<iTransformComponent>();
    if (transformComp == nullptr)
    {
        return;
    }

    auto camTransformComp = entityScene->getActiveCamera()->getComponent<iTransformComponent>();
    const auto &entityPos = transformComp->getWorldPosition();

    float64 distanceToCam = camTransformComp->getWorldPosition().distance(entityPos) * 0.1;

    auto lightTransformComp = _lightRoot->getComponent<iTransformComponent>();
    lightTransformComp->setPosition(entityPos);
    lightTransformComp->setOrientation(camTransformComp->getWorldOrientation());
    lightTransformComp->setScale(iaVector3d(distanceToCam, distanceToCam, distanceToCam));
}

void LightOverlay::onPreRender()
{
    onUpdate();
}