// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "EmitterOverlay.h"

EmitterOverlay::EmitterOverlay(iViewPtr view)
    : EntityOverlay(view)
{
    onInit();
}

EmitterOverlay::~EmitterOverlay()
{
    onDeinit();
}

void EmitterOverlay::setActive(bool active)
{
    EntityOverlay::setActive(active);

    // _rootTransform->setActive(active);

    update();
}

bool EmitterOverlay::accepts(OverlayMode mode, iEntityPtr entity)
{
    // TODO check if this entity has an emitter component
    return false;
}

void EmitterOverlay::setOverlayMode(OverlayMode mode)
{
    // don't care about the overlay mode
}

void EmitterOverlay::onDeinit()
{
    getView()->getRenderEvent().remove(iRenderDelegate(this, &EmitterOverlay::onRender));

    _materialFlat = nullptr;
    _materialVolume = nullptr;
}

void EmitterOverlay::onInit()
{
    getView()->getRenderEvent().add(iRenderDelegate(this, &EmitterOverlay::onRender));

    iShaderPtr shaderFlat = iResourceManager::getInstance().loadResource<iShader>("igor_shader_material_emitter_overlay_flat");
    iParameters paramFlat({
        {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
        {IGOR_RESOURCE_PARAM_GENERATE, true},
        {IGOR_RESOURCE_PARAM_SHADER, shaderFlat},
        {IGOR_RESOURCE_PARAM_AMBIENT, iaColor3f(0.0f, 0.5f, 0.0f)},
        {IGOR_RESOURCE_PARAM_DIFFUSE, iaColor3f(0.0f, 0.5f, 0.0f)},
        {IGOR_RESOURCE_PARAM_SPECULAR, iaColor3f(0.0f, 0.5f, 0.0f)},
        {IGOR_RESOURCE_PARAM_EMISSIVE, iaColor3f(0.0f, 0.0f, 0.0f)},
        {IGOR_RESOURCE_PARAM_ALPHA, 0.6f},
    });
    _materialFlat = iResourceManager::getInstance().loadResource<iMaterial>(paramFlat);

    iShaderPtr shaderVolume = iResourceManager::getInstance().loadResource<iShader>("igor_shader_material_emitter_overlay_volume");
    iParameters paramVolume({
        {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
        {IGOR_RESOURCE_PARAM_GENERATE, true},
        {IGOR_RESOURCE_PARAM_SHADER, shaderVolume},
        {IGOR_RESOURCE_PARAM_AMBIENT, iaColor3f(0.0f, 0.5f, 0.0f)},
        {IGOR_RESOURCE_PARAM_DIFFUSE, iaColor3f(0.0f, 0.5f, 0.0f)},
        {IGOR_RESOURCE_PARAM_SPECULAR, iaColor3f(0.0f, 0.5f, 0.0f)},
        {IGOR_RESOURCE_PARAM_EMISSIVE, iaColor3f(0.0f, 0.0f, 0.0f)},
        {IGOR_RESOURCE_PARAM_ALPHA, 0.6f},
    });
    _materialVolume = iResourceManager::getInstance().loadResource<iMaterial>(paramVolume);

   // TODO
}

void EmitterOverlay::update()
{
    /*iNodePtr node = iNodeManager::getInstance().getNode(getNodeID());
    if (node == nullptr ||
        node->getType() != iNodeType::iNodeEmitter)
    {
        return;
    }

    const iNodeEmitterPtr emitter = static_cast<iNodeEmitterPtr>(node);
    const iEmitterType emitterType = emitter->getEmitterType();

    switch (emitterType)
    {
    case iEmitterType::Mesh:
        _switchNode->setActiveChild(nullptr);
        // not supported yet
        break;

    case iEmitterType::Point:
        // show nothing
        _switchNode->setActiveChild(nullptr);
        break;

    case iEmitterType::Disc:
        _switchNode->setActiveChild(_discMeshNode);
        break;

    case iEmitterType::Circle:
        _switchNode->setActiveChild(_circleMeshNode);
        break;

    case iEmitterType::Sphere:
        _switchNode->setActiveChild(_sphereMeshNode);
        break;

    case iEmitterType::Square:
        _switchNode->setActiveChild(_squareMeshNode);
        break;

    case iEmitterType::Cube:
        _switchNode->setActiveChild(_cubeMeshNode);
        break;
    }

    // update size
    _scaleTransform->identity();
    _scaleTransform->scale(emitter->getSize(), emitter->getSize(), emitter->getSize());

    // update matrix
    iaMatrixd matrix;
    node->calcWorldTransformation(matrix);
    _rootTransform->setMatrix(matrix);*/
}

void EmitterOverlay::onRender()
{
    update();
}

iMeshPtr EmitterOverlay::createDisc()
{
    iMeshBuilder meshBuilder;

    iMeshBuilderUtils::addCircle(meshBuilder, 1, 32);
    meshBuilder.calcNormals(true);
    return meshBuilder.createMesh();
}

iMeshPtr EmitterOverlay::createCircle()
{
    iMeshBuilder meshBuilder;

    iMeshBuilderUtils::addRing(meshBuilder, 0.95, 1, 32);
    meshBuilder.calcNormals(true);
    return meshBuilder.createMesh();
}

iMeshPtr EmitterOverlay::createCube()
{
    iMeshBuilder meshBuilder;

    iaMatrixf matrix;
    matrix.translate(0, -1, 0);
    meshBuilder.setMatrix(matrix);

    iMeshBuilderUtils::addBox(meshBuilder, 2, 2, 2);
    meshBuilder.calcNormals(true);
    return meshBuilder.createMesh();
}

iMeshPtr EmitterOverlay::createSphere()
{
    iMeshBuilder meshBuilder;

    iMeshBuilderUtils::addSphere(meshBuilder, 1, 32);
    meshBuilder.calcNormals(true);
    return meshBuilder.createMesh();
}

iMeshPtr EmitterOverlay::createSquare()
{
    iMeshBuilder meshBuilder;

    iMeshBuilderUtils::addPlane(meshBuilder, 2, 2, 1, 1);
    return meshBuilder.createMesh();
}
