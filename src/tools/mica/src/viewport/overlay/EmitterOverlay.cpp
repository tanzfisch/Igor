// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "EmitterOverlay.h"

EmitterOverlay::EmitterOverlay(iViewPtr view, iScenePtr scene, WorkspacePtr workspace)
    : NodeOverlay(view, scene, workspace)
{
    onInit();
}

EmitterOverlay::~EmitterOverlay()
{
    onDeinit();
}

void EmitterOverlay::setNodeID(uint64 nodeID)
{
    NodeOverlay::setNodeID(nodeID);

    update();    
}

void EmitterOverlay::setActive(bool active)
{
    NodeOverlay::setActive(active);

    _rootTransform->setActive(active);

    update();    
}

bool EmitterOverlay::accepts(OverlayMode mode, iNodeKind nodeKind, iNodeType nodeType)
{
    return nodeType == iNodeType::iNodeEmitter;
}

void EmitterOverlay::setOverlayMode(OverlayMode mode)
{
    // don't care about the overlay mode
}

void EmitterOverlay::onDeinit()
{
    getView()->unregisterRenderDelegate(iDrawDelegate(this, &EmitterOverlay::onRender));

    _targetMaterial = nullptr;
    _materialFlat = nullptr;
    _materialVolume = nullptr;
}

void EmitterOverlay::onInit()
{
    getView()->registerRenderDelegate(iDrawDelegate(this, &EmitterOverlay::onRender));

    _materialFlat = iResourceManager::getInstance().loadResource<iMaterial>("igor_material_emitter_overlay_flat");
    _materialVolume = iResourceManager::getInstance().loadResource<iMaterial>("igor_material_emitter_overlay_volume");

    _targetMaterial = iTargetMaterial::create();
    _targetMaterial->setEmissive(iaColor3f(0.0f, 0.0f, 0.0f));
    _targetMaterial->setSpecular(iaColor3f(0.0f, 0.5f, 0.0f));
    _targetMaterial->setDiffuse(iaColor3f(0.0f, 0.5f, 0.0f));
    _targetMaterial->setAmbient(iaColor3f(0.0f, 0.5f, 0.0f));
    _targetMaterial->setAlpha(0.5);

    _rootTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    getScene()->getRoot()->insertNode(_rootTransform);

    _scaleTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
    _rootTransform->insertNode(_scaleTransform);

    _switchNode = iNodeManager::getInstance().createNode<iNodeSwitch>();
    _scaleTransform->insertNode(_switchNode);

    _discMeshNode = iNodeManager::getInstance().createNode<iNodeMesh>();
    _discMeshNode->setName("disc mesh");
    _discMeshNode->setMesh(createDisc());
    _discMeshNode->setMaterial(_materialFlat);
    _discMeshNode->setTargetMaterial(_targetMaterial);
    _switchNode->insertNode(_discMeshNode);

    _circleMeshNode = iNodeManager::getInstance().createNode<iNodeMesh>();
    _circleMeshNode->setName("circle mesh");
    _circleMeshNode->setMesh(createCircle());
    _circleMeshNode->setMaterial(_materialFlat);
    _circleMeshNode->setTargetMaterial(_targetMaterial);
    _switchNode->insertNode(_circleMeshNode);

    _sphereMeshNode = iNodeManager::getInstance().createNode<iNodeMesh>();
    _sphereMeshNode->setName("sphere mesh");
    _sphereMeshNode->setMesh(createSphere());
    _sphereMeshNode->setMaterial(_materialVolume);
    _sphereMeshNode->setTargetMaterial(_targetMaterial);
    _switchNode->insertNode(_sphereMeshNode);

    _squareMeshNode = iNodeManager::getInstance().createNode<iNodeMesh>();
    _squareMeshNode->setName("square mesh");
    _squareMeshNode->setMesh(createSquare());
    _squareMeshNode->setMaterial(_materialFlat);
    _squareMeshNode->setTargetMaterial(_targetMaterial);
    _switchNode->insertNode(_squareMeshNode);

    _cubeMeshNode = iNodeManager::getInstance().createNode<iNodeMesh>();
    _cubeMeshNode->setName("cube mesh");
    _cubeMeshNode->setMesh(createCube());
    _cubeMeshNode->setMaterial(_materialVolume);
    _cubeMeshNode->setTargetMaterial(_targetMaterial);
    _switchNode->insertNode(_cubeMeshNode);

    _rootTransform->setActive(false);
    _switchNode->setActiveChild(nullptr);
}

void EmitterOverlay::update()
{
    iNodePtr node = iNodeManager::getInstance().getNode(getNodeID());
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
    _rootTransform->setMatrix(matrix);
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
