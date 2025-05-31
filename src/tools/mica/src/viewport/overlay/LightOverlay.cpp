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

    iMeshPtr lightbulb = createLightbulb();

    _lightRoot = entityScene->createEntity("overlay.light.root");
    _lightRoot->addComponent(new iTransformComponent());
    _lightRoot->addComponent(new iSphereComponent(1.0));
    _lightRoot->addComponent(new iOctreeComponent());
    auto xMeshRenderComponent = _lightRoot->addComponent(new iMeshRenderComponent());
    xMeshRenderComponent->addMesh(lightbulb, _cyan);

    _lightRoot->setActive(false);
}

iMeshPtr LightOverlay::createLightbulb()
{
    iMeshBuilder meshBuilder;

    iaMatrixf transform;

    // === Bulb (sphere) ===
    float32 bulbRadius = 1.0f;

    // Move the bulb up so it sits on top of the base
    //transform._pos.set(0.0f, bulbRadius + 0.6f, 0.0f); // 0.6f = base height
    //meshBuilder.setMatrix(transform);
    iMeshBuilderUtils::addSphere(meshBuilder, bulbRadius, 32);

    // === Base (cylinder) ===
    /*float32 baseRadius = 0.4f;
    float32 baseHeight = 0.6f;

    // Place base at origin
    transform._pos.set(0.0f, baseHeight / 2.0f, 0.0f); // Center the cylinder vertically
    meshBuilder.setMatrix(transform);
    iMeshBuilderUtils::addCylinder(meshBuilder, baseRadius, baseHeight, 32, true);*/

    // === Threads (toruses) ===
    /*int threadCount = 3;
    float32 threadSpacing = 0.15f;
    float32 threadOffsetY = threadSpacing * 0.5f;

    for (int i = 0; i < threadCount; ++i)
    {
        float32 yPos = threadOffsetY + (i * threadSpacing);

        transform._pos.set(0.0f, yPos, 0.0f);
        meshBuilder.setMatrix(transform);
        iMeshBuilderUtils::addTorus(meshBuilder, baseRadius, 0.05f, 0.05f, 16, 8);
    }

    // === Contact Plate (circle) at bottom ===
    float32 contactRadius = 0.3f;

    // transform.rotationX(-90.0f); // Rotate so circle faces down
    transform._pos.set(0.0f, 0.0f, 0.0f); // Bottom of the base
    meshBuilder.setMatrix(transform);
    iMeshBuilderUtils::addCircle(meshBuilder, contactRadius, 16);*/

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

    auto lightTransformComp = _lightRoot->getComponent<iTransformComponent>();
    lightTransformComp->setPosition(transformComp->getWorldPosition());
}

void LightOverlay::onPreRender()
{
    onUpdate();
}