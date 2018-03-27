// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "Manipulator.h"

#include <iMaterialResourceFactory.h>
#include <iTargetMaterial.h>
#include <iMaterialGroup.h>
#include <iRenderer.h>
#include <iMeshBuilder.h>
#include <iNodeFactory.h>
#include <iRenderStateSet.h>
#include <iNodeTransform.h>
#include <iNodeMesh.h>
#include <iNodeSwitch.h>
#include <iMeshBuilderUtils.h>
using namespace Igor;

Manipulator::Manipulator(iNode* parent)
{
    con_assert(parent != nullptr, "zero pointer");

    _parent = parent;
    init();
}

void Manipulator::init()
{
    _material = iMaterialResourceFactory::getInstance().createMaterial("Manipulator");
    iMaterialResourceFactory::getInstance().getMaterial(_material)->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
    iMaterialResourceFactory::getInstance().getMaterial(_material)->addShaderSource("igor/default_directional_light.frag", iShaderObjectType::Fragment);
    iMaterialResourceFactory::getInstance().getMaterial(_material)->compileShader();
    iMaterialResourceFactory::getInstance().getMaterialGroup(_material)->setOrder(iMaterial::RENDER_ORDER_MAX);

    _red = iMaterialResourceFactory::getInstance().createTargetMaterial();
    _red->setEmissive(iaColor3f(0.6f, 0.0f, 0.0f));
    _red->setSpecular(iaColor3f(0.2f, 0.0f, 0.0f));
    _red->setDiffuse(iaColor3f(0.5f, 0.0f, 0.0f));
    _red->setAmbient(iaColor3f(0.3f, 0.0f, 0.0f));

    _green = iMaterialResourceFactory::getInstance().createTargetMaterial();
    _green->setEmissive(iaColor3f(0.0f, 0.6f, 0.0f));
    _green->setSpecular(iaColor3f(0.0f, 0.2f, 0.0f));
    _green->setDiffuse(iaColor3f(0.0f, 0.5f, 0.0f));
    _green->setAmbient(iaColor3f(0.0f, 0.3f, 0.0f));

    _blue = iMaterialResourceFactory::getInstance().createTargetMaterial();
    _blue->setEmissive(iaColor3f(0.0f, 0.0f, 0.6f));
    _blue->setSpecular(iaColor3f(0.0f, 0.0f, 0.2f));
    _blue->setDiffuse(iaColor3f(0.0f, 0.0f, 0.5f));
    _blue->setAmbient(iaColor3f(0.0f, 0.0f, 0.3f));

    _cyan = iMaterialResourceFactory::getInstance().createTargetMaterial();
    _cyan->setEmissive(iaColor3f(0.0f, 0.6f, 0.6f));
    _cyan->setSpecular(iaColor3f(0.0f, 0.2f, 0.2f));
    _cyan->setDiffuse(iaColor3f(0.0f, 0.5f, 0.5f));
    _cyan->setAmbient(iaColor3f(0.0f, 0.3f, 0.3f));

    shared_ptr<iMesh> translateMesh = createTranslateMesh();
    shared_ptr<iMesh> scaleMesh = createScaleMesh();
    shared_ptr<iMesh> locatorMesh = createLocatorMesh();
    shared_ptr<iMesh> ringMesh = createRingMesh();
    shared_ptr<iMesh> ringMesh2D = create2DRingMesh();

    _rootTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _rootTransform->setActive(_visible);

    _switchNode = static_cast<iNodeSwitch*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSwitch));
    _rootTransform->insertNode(_switchNode);

    createLocatorModifier(locatorMesh);
    createTranslateModifier(translateMesh);
    createScaleModifier(scaleMesh);
    createRotateModifier(locatorMesh, ringMesh, ringMesh2D);

    _parent->insertNode(_rootTransform);

    setModifierMode(_modifierMode);
}

void Manipulator::createRotateModifier(shared_ptr<iMesh> &locatorMesh, shared_ptr<iMesh> &ringMesh, shared_ptr<iMesh> &ringMesh2D)
{
    _roateModifier = iNodeFactory::getInstance().createNode(iNodeType::iNode);
    _switchNode->insertNode(_roateModifier);

    iNodeTransform* xTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
    _roateModifier->insertNode(xTransform);

    iNodeTransform* yTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _roateModifier->insertNode(yTransform);

    iNodeTransform* zTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    zTransform->rotate(M_PI * 0.5, iaAxis::X);
    _roateModifier->insertNode(zTransform);

    iNodeMesh* xCylinder = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    xCylinder->setMesh(locatorMesh);
    xCylinder->setMaterial(_material);
    xCylinder->setTargetMaterial(_red);
    xTransform->insertNode(xCylinder);

    iNodeMesh* yCylinder = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    yCylinder->setMesh(locatorMesh);
    yCylinder->setMaterial(_material);
    yCylinder->setTargetMaterial(_green);
    yTransform->insertNode(yCylinder);

    iNodeMesh* zCylinder = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    zCylinder->setMesh(locatorMesh);
    zCylinder->setMaterial(_material);
    zCylinder->setTargetMaterial(_blue);
    zTransform->insertNode(zCylinder);

    iNodeTransform* xRingTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    xRingTransform->rotate(-M_PI * 0.5, iaAxis::Z);
    xRingTransform->scale(2.0, 0.05, 2.0);
    _roateModifier->insertNode(xRingTransform);

    iNodeTransform* yRingTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    yRingTransform->scale(1.99, 0.05, 1.99);
    _roateModifier->insertNode(yRingTransform);

    iNodeTransform* zRingTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    zRingTransform->rotate(M_PI * 0.5, iaAxis::X);
    zRingTransform->scale(1.98, 0.05, 1.98);
    _roateModifier->insertNode(zRingTransform);

    iNodeMesh* xRing = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    xRing->setMesh(ringMesh);
    xRing->setMaterial(_material);
    xRing->setTargetMaterial(_red);
    xRingTransform->insertNode(xRing);

    iNodeMesh* yRing = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    yRing->setMesh(ringMesh);
    yRing->setMaterial(_material);
    yRing->setTargetMaterial(_green);
    yRingTransform->insertNode(yRing);

    iNodeMesh* zRing = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    zRing->setMesh(ringMesh);
    zRing->setMaterial(_material);
    zRing->setTargetMaterial(_blue);
    zRingTransform->insertNode(zRing);

    _rotateBillboardTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _roateModifier->insertNode(_rotateBillboardTransform);

    iNodeMesh* ring = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    ring->setMesh(ringMesh2D);
    ring->setMaterial(_material);
    ring->setTargetMaterial(_cyan);
    _rotateBillboardTransform->insertNode(ring);
}

void Manipulator::createTranslateModifier(shared_ptr<iMesh> &translateMesh)
{
    _translateModifier = iNodeFactory::getInstance().createNode(iNodeType::iNode);
    _switchNode->insertNode(_translateModifier);

    iNodeTransform* xTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
    _translateModifier->insertNode(xTransform);

    iNodeTransform* yTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _translateModifier->insertNode(yTransform);

    iNodeTransform* zTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    zTransform->rotate(M_PI * 0.5, iaAxis::X);
    _translateModifier->insertNode(zTransform);

    iNodeMesh* xUmbrella = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    xUmbrella->setMesh(translateMesh);
    xUmbrella->setMaterial(_material);
    xUmbrella->setTargetMaterial(_red);
    xTransform->insertNode(xUmbrella);

    iNodeMesh* yUmbrella = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    yUmbrella->setMesh(translateMesh);
    yUmbrella->setMaterial(_material);
    yUmbrella->setTargetMaterial(_green);
    yTransform->insertNode(yUmbrella);

    iNodeMesh* zUmbrella = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    zUmbrella->setMesh(translateMesh);
    zUmbrella->setMaterial(_material);
    zUmbrella->setTargetMaterial(_blue);
    zTransform->insertNode(zUmbrella);

    _translateXNodeID = xUmbrella->getID();
    _translateYNodeID = yUmbrella->getID();
    _translateZNodeID = zUmbrella->getID();
}

void Manipulator::createScaleModifier(shared_ptr<iMesh> &scaleMesh)
{
    _scaleModifier = iNodeFactory::getInstance().createNode(iNodeType::iNode);
    _switchNode->insertNode(_scaleModifier);

    iNodeTransform* xTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
    _scaleModifier->insertNode(xTransform);

    iNodeTransform* yTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _scaleModifier->insertNode(yTransform);

    iNodeTransform* zTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    zTransform->rotate(M_PI * 0.5, iaAxis::X);
    _scaleModifier->insertNode(zTransform);

    iNodeMesh* xCube = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    xCube->setMesh(scaleMesh);
    xCube->setMaterial(_material);
    xCube->setTargetMaterial(_red);
    xTransform->insertNode(xCube);

    iNodeMesh* yCube = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    yCube->setMesh(scaleMesh);
    yCube->setMaterial(_material);
    yCube->setTargetMaterial(_green);
    yTransform->insertNode(yCube);

    iNodeMesh* zCube = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    zCube->setMesh(scaleMesh);
    zCube->setMaterial(_material);
    zCube->setTargetMaterial(_blue);
    zTransform->insertNode(zCube);
}

void Manipulator::createLocatorModifier(shared_ptr<iMesh> &locatorMesh)
{
    _locatorModifier = iNodeFactory::getInstance().createNode(iNodeType::iNode);
    _switchNode->insertNode(_locatorModifier);

    iNodeTransform* xTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    xTransform->rotate(-M_PI * 0.5, iaAxis::Z);    
    _locatorModifier->insertNode(xTransform);

    iNodeTransform* yTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    _locatorModifier->insertNode(yTransform);

    iNodeTransform* zTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    zTransform->rotate(M_PI * 0.5, iaAxis::X);
    _locatorModifier->insertNode(zTransform);

    iNodeMesh* xCylinder = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    xCylinder->setMesh(locatorMesh);
    xCylinder->setMaterial(_material);
    xCylinder->setTargetMaterial(_red);
    xTransform->insertNode(xCylinder);

    iNodeMesh* yCylinder = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    yCylinder->setMesh(locatorMesh);
    yCylinder->setMaterial(_material);
    yCylinder->setTargetMaterial(_green);
    yTransform->insertNode(yCylinder);

    iNodeMesh* zCylinder = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    zCylinder->setMesh(locatorMesh);
    zCylinder->setMaterial(_material);
    zCylinder->setTargetMaterial(_blue);
    zTransform->insertNode(zCylinder);
}

void Manipulator::updateCamMatrix(const iaMatrixd& camMatrix)
{
    float64 distanceToCam = camMatrix._pos.distance(_modifierMatrix._pos) * 0.1;    
    _rootTransform->setMatrix(_modifierMatrix);
    _rootTransform->scale(distanceToCam, distanceToCam, distanceToCam);

    _rotateBillboardTransform->identity();
    iaMatrixd matrix;
    _rotateBillboardTransform->getMatrix(matrix);
    matrix._right = camMatrix._right;
    matrix._top = camMatrix._top;
    matrix._depth = camMatrix._depth;
    _rotateBillboardTransform->setMatrix(matrix);
    _rotateBillboardTransform->rotate(M_PI * 0.5, iaAxis::X);
    _rotateBillboardTransform->scale(2.1, 2.1, 2.1);
}

void Manipulator::deinit()
{
    iMaterialResourceFactory::getInstance().destroyTargetMaterial(_red);
    iMaterialResourceFactory::getInstance().destroyTargetMaterial(_green);
    iMaterialResourceFactory::getInstance().destroyTargetMaterial(_blue);
    iMaterialResourceFactory::getInstance().destroyTargetMaterial(_cyan);
}

void Manipulator::setVisible(bool visible)
{
    _visible = visible;
    _rootTransform->setActive(_visible);
}

bool Manipulator::isVisible() const
{
    return _visible;
}

shared_ptr<iMesh> Manipulator::createRingMesh()
{
    iMeshBuilder meshBuilder;
    iMeshBuilderUtils::addCylinder(meshBuilder, 1, 1, 64, false);
    meshBuilder.calcNormals(true);
    return meshBuilder.createMesh();
}

shared_ptr<iMesh> Manipulator::create2DRingMesh()
{
    iMeshBuilder meshBuilder;
    iMeshBuilderUtils::addRing(meshBuilder, 0.99, 1, 64);
    meshBuilder.calcNormals(true);
    return meshBuilder.createMesh();
}

shared_ptr<iMesh> Manipulator::createLocatorMesh()
{
    iMeshBuilder meshBuilder;

    iaMatrixf matrix;
    matrix.scale(0.02, 2.0, 0.02);
    meshBuilder.setMatrix(matrix);
    iMeshBuilderUtils::addCylinder(meshBuilder, 1, 1, 8);
    meshBuilder.calcNormals(true);
    return meshBuilder.createMesh();
}

shared_ptr<iMesh> Manipulator::createScaleMesh()
{
    iMeshBuilder meshBuilder;

    iaMatrixf matrix;    
    matrix.scale(0.02, 1.5, 0.02);
    meshBuilder.setMatrix(matrix);
    iMeshBuilderUtils::addCylinder(meshBuilder, 1, 1, 8);

    matrix.identity();
    matrix.translate(0, 1.5, 0);
    matrix.scale(0.25, 0.25, 0.25);
    meshBuilder.setMatrix(matrix);
    iMeshBuilderUtils::addBox(meshBuilder, 1, 1, 1);

    meshBuilder.calcNormals(true);
    return meshBuilder.createMesh();
}

shared_ptr<iMesh> Manipulator::createTranslateMesh()
{
    iMeshBuilder meshBuilder;
    meshBuilder.setJoinVertexes(false);

    iaMatrixf matrix;
    
    matrix.translate(0, 1.5, 0);
    matrix.scale(0.1, 0.5, 0.1);
    meshBuilder.setMatrix(matrix);
    iMeshBuilderUtils::addCone(meshBuilder, 1, 1, 8);

    matrix.identity();
    matrix.scale(0.02, 1.5, 0.02);
    meshBuilder.setMatrix(matrix);
    iMeshBuilderUtils::addCylinder(meshBuilder, 1, 1, 8);

    meshBuilder.calcNormals(true);

    return meshBuilder.createMesh();
}

void Manipulator::setModifierMode(ModifierMode modifierMode)
{
    _modifierMode = modifierMode;

    switch (_modifierMode)
    {
    case ModifierMode::Locator:
        _switchNode->setActiveChild(_locatorModifier);
        break;

    case ModifierMode::Translate:
        _switchNode->setActiveChild(_translateModifier);
        break;

    case ModifierMode::Scale:
        _switchNode->setActiveChild(_scaleModifier);
        break;

    case ModifierMode::Rotate:
        _switchNode->setActiveChild(_roateModifier);
        break;
    }
}

ModifierMode Manipulator::getModifierMode() const
{
    return _modifierMode;
}

void Manipulator::setMatrix(const iaMatrixd& matrix)
{
    _modifierMatrix = matrix;
}

void Manipulator::getMatrix(iaMatrixd& matrix) const
{
    matrix = _modifierMatrix;
}

void Manipulator::setSelected(uint64 nodeID)
{
    _selectedNodeID = nodeID;
}

uint64 Manipulator::getSelected() const
{
    return _selectedNodeID;
}

void Manipulator::transform(float64 dx, float64 dy, iaMatrixd& matrix)
{
    matrix.translate(dx,dy,0);
}