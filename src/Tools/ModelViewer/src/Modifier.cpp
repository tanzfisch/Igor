// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "Modifier.h"

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
using namespace Igor;

Modifier::Modifier(iNode* parent)
{
    con_assert(parent != nullptr, "zero pointer");

    _parent = parent;
    init();
}

void Modifier::init()
{
    _material = iMaterialResourceFactory::getInstance().createMaterial("Modifier");
    iMaterialResourceFactory::getInstance().getMaterial(_material)->addShaderSource("igor_default.vert", iShaderObjectType::Vertex);
    iMaterialResourceFactory::getInstance().getMaterial(_material)->addShaderSource("igor_default_directional_light.frag", iShaderObjectType::Fragment);
    iMaterialResourceFactory::getInstance().getMaterial(_material)->compileShader();
    iMaterialResourceFactory::getInstance().getMaterialGroup(_material)->setOrder(iMaterial::RENDER_ORDER_MAX);

    shared_ptr<iMesh> umbrellaMesh = createUmbrella();
    shared_ptr<iMesh> cylinderMesh = createCylinder();
    shared_ptr<iMesh> cubeMesh = createCube();

    _rootTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    
    _switchNode = static_cast<iNodeSwitch*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeSwitch));
    _rootTransform->insertNode(_switchNode);

    createLocatorModifier(cylinderMesh);
    createTranslateModifier(cylinderMesh, umbrellaMesh);
    createScaleModifier(cylinderMesh, cubeMesh);
    // rotate

    _parent->insertNode(_rootTransform);

    setModifierMode(_modifierMode);
}

void Modifier::createTranslateModifier(shared_ptr<iMesh> &cylinderMesh, shared_ptr<iMesh> &umbrellaMesh)
{
    _translateModifier = iNodeFactory::getInstance().createNode(iNodeType::iNode);
    _switchNode->insertNode(_translateModifier);

    iNodeTransform* xTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    xTransform->scale(1.5, 0.02, 0.02);
    _translateModifier->insertNode(xTransform);

    iNodeTransform* yTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    yTransform->rotate(M_PI * 0.5, iaAxis::Z);
    yTransform->scale(1.5, 0.02, 0.02);
    _translateModifier->insertNode(yTransform);

    iNodeTransform* zTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    zTransform->rotate(-M_PI * 0.5, iaAxis::Y);
    zTransform->scale(1.5, 0.02, 0.02);
    _translateModifier->insertNode(zTransform);

    iNodeMesh* xCylinder = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    xCylinder->setMesh(cylinderMesh);
    xCylinder->setMaterial(_material);
    xCylinder->getTargetMaterial()->setEmissive(iaColor3f(1.0f, 0.0f, 0.0f));
    xCylinder->getTargetMaterial()->setSpecular(iaColor3f(0.0f, 0.0f, 0.0f));
    xCylinder->getTargetMaterial()->setDiffuse(iaColor3f(0.0f, 0.0f, 0.0f));
    xCylinder->getTargetMaterial()->setAmbient(iaColor3f(0.0f, 0.0f, 0.0f));
    xTransform->insertNode(xCylinder);

    iNodeMesh* yCylinder = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    yCylinder->setMesh(cylinderMesh);
    yCylinder->setMaterial(_material);
    yCylinder->getTargetMaterial()->setEmissive(iaColor3f(0.0f, 1.0f, 0.0f));
    yCylinder->getTargetMaterial()->setSpecular(iaColor3f(0.0f, 0.0f, 0.0f));
    yCylinder->getTargetMaterial()->setDiffuse(iaColor3f(0.0f, 0.0f, 0.0f));
    yCylinder->getTargetMaterial()->setAmbient(iaColor3f(0.0f, 0.0f, 0.0f));
    yTransform->insertNode(yCylinder);

    iNodeMesh* zCylinder = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    zCylinder->setMesh(cylinderMesh);
    zCylinder->setMaterial(_material);
    zCylinder->getTargetMaterial()->setEmissive(iaColor3f(0.0f, 0.0f, 1.0f));
    zCylinder->getTargetMaterial()->setSpecular(iaColor3f(0.0f, 0.0f, 0.0f));
    zCylinder->getTargetMaterial()->setDiffuse(iaColor3f(0.0f, 0.0f, 0.0f));
    zCylinder->getTargetMaterial()->setAmbient(iaColor3f(0.0f, 0.0f, 0.0f));
    zTransform->insertNode(zCylinder);

    xTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    xTransform->translate(1.5, 0, 0);
    xTransform->scale(0.5, 0.1, 0.1);
    _translateModifier->insertNode(xTransform);

    yTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    yTransform->rotate(M_PI * 0.5, iaAxis::Z);
    yTransform->translate(1.5, 0, 0);
    yTransform->scale(0.5, 0.1, 0.1);
    _translateModifier->insertNode(yTransform);

    zTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    zTransform->rotate(-M_PI * 0.5, iaAxis::Y);
    zTransform->translate(1.5, 0, 0);
    zTransform->scale(0.5, 0.1, 0.1);
    _translateModifier->insertNode(zTransform);

    iNodeMesh* xUmbrella = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    xUmbrella->setMesh(umbrellaMesh);
    xUmbrella->setMaterial(_material);
    xUmbrella->getTargetMaterial()->setEmissive(iaColor3f(1.0f, 0.0f, 0.0f));
    xUmbrella->getTargetMaterial()->setSpecular(iaColor3f(0.0f, 0.0f, 0.0f));
    xUmbrella->getTargetMaterial()->setDiffuse(iaColor3f(0.0f, 0.0f, 0.0f));
    xUmbrella->getTargetMaterial()->setAmbient(iaColor3f(0.0f, 0.0f, 0.0f));
    xTransform->insertNode(xUmbrella);

    iNodeMesh* yUmbrella = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    yUmbrella->setMesh(umbrellaMesh);
    yUmbrella->setMaterial(_material);
    yUmbrella->getTargetMaterial()->setEmissive(iaColor3f(0.0f, 1.0f, 0.0f));
    yUmbrella->getTargetMaterial()->setSpecular(iaColor3f(0.0f, 0.0f, 0.0f));
    yUmbrella->getTargetMaterial()->setDiffuse(iaColor3f(0.0f, 0.0f, 0.0f));
    yUmbrella->getTargetMaterial()->setAmbient(iaColor3f(0.0f, 0.0f, 0.0f));
    yTransform->insertNode(yUmbrella);

    iNodeMesh* zUmbrella = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    zUmbrella->setMesh(umbrellaMesh);
    zUmbrella->setMaterial(_material);
    zUmbrella->getTargetMaterial()->setEmissive(iaColor3f(0.0f, 0.0f, 1.0f));
    zUmbrella->getTargetMaterial()->setSpecular(iaColor3f(0.0f, 0.0f, 0.0f));
    zUmbrella->getTargetMaterial()->setDiffuse(iaColor3f(0.0f, 0.0f, 0.0f));
    zUmbrella->getTargetMaterial()->setAmbient(iaColor3f(0.0f, 0.0f, 0.0f));
    zTransform->insertNode(zUmbrella);
}

void Modifier::createScaleModifier(shared_ptr<iMesh> &cylinderMesh, shared_ptr<iMesh> &cubeMesh)
{
    _scaleModifier = iNodeFactory::getInstance().createNode(iNodeType::iNode);
    _switchNode->insertNode(_scaleModifier);

    iNodeTransform* xTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    xTransform->scale(1.5, 0.02, 0.02);
    _scaleModifier->insertNode(xTransform);

    iNodeTransform* yTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    yTransform->rotate(M_PI * 0.5, iaAxis::Z);
    yTransform->scale(1.5, 0.02, 0.02);
    _scaleModifier->insertNode(yTransform);

    iNodeTransform* zTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    zTransform->rotate(-M_PI * 0.5, iaAxis::Y);
    zTransform->scale(1.5, 0.02, 0.02);
    _scaleModifier->insertNode(zTransform);

    iNodeMesh* xCylinder = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    xCylinder->setMesh(cylinderMesh);
    xCylinder->setMaterial(_material);
    xCylinder->getTargetMaterial()->setEmissive(iaColor3f(1.0f, 0.0f, 0.0f));
    xCylinder->getTargetMaterial()->setSpecular(iaColor3f(0.0f, 0.0f, 0.0f));
    xCylinder->getTargetMaterial()->setDiffuse(iaColor3f(0.0f, 0.0f, 0.0f));
    xCylinder->getTargetMaterial()->setAmbient(iaColor3f(0.0f, 0.0f, 0.0f));
    xTransform->insertNode(xCylinder);

    iNodeMesh* yCylinder = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    yCylinder->setMesh(cylinderMesh);
    yCylinder->setMaterial(_material);
    yCylinder->getTargetMaterial()->setEmissive(iaColor3f(0.0f, 1.0f, 0.0f));
    yCylinder->getTargetMaterial()->setSpecular(iaColor3f(0.0f, 0.0f, 0.0f));
    yCylinder->getTargetMaterial()->setDiffuse(iaColor3f(0.0f, 0.0f, 0.0f));
    yCylinder->getTargetMaterial()->setAmbient(iaColor3f(0.0f, 0.0f, 0.0f));
    yTransform->insertNode(yCylinder);

    iNodeMesh* zCylinder = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    zCylinder->setMesh(cylinderMesh);
    zCylinder->setMaterial(_material);
    zCylinder->getTargetMaterial()->setEmissive(iaColor3f(0.0f, 0.0f, 1.0f));
    zCylinder->getTargetMaterial()->setSpecular(iaColor3f(0.0f, 0.0f, 0.0f));
    zCylinder->getTargetMaterial()->setDiffuse(iaColor3f(0.0f, 0.0f, 0.0f));
    zCylinder->getTargetMaterial()->setAmbient(iaColor3f(0.0f, 0.0f, 0.0f));
    zTransform->insertNode(zCylinder);

    xTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    xTransform->translate(1.5, 0, 0);
    xTransform->scale(0.25, 0.25, 0.25);
    _scaleModifier->insertNode(xTransform);

    yTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    yTransform->rotate(M_PI * 0.5, iaAxis::Z);
    yTransform->translate(1.5, 0, 0);
    yTransform->scale(0.25, 0.25, 0.25);
    _scaleModifier->insertNode(yTransform);

    zTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    zTransform->rotate(-M_PI * 0.5, iaAxis::Y);
    zTransform->translate(1.5, 0, 0);
    zTransform->scale(0.25, 0.25, 0.25);
    _scaleModifier->insertNode(zTransform);

    iNodeMesh* xCube = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    xCube->setMesh(cubeMesh);
    xCube->setMaterial(_material);
    xCube->getTargetMaterial()->setEmissive(iaColor3f(1.0f, 0.0f, 0.0f));
    xCube->getTargetMaterial()->setSpecular(iaColor3f(0.0f, 0.0f, 0.0f));
    xCube->getTargetMaterial()->setDiffuse(iaColor3f(0.0f, 0.0f, 0.0f));
    xCube->getTargetMaterial()->setAmbient(iaColor3f(0.0f, 0.0f, 0.0f));
    xTransform->insertNode(xCube);

    iNodeMesh* yCube = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    yCube->setMesh(cubeMesh);
    yCube->setMaterial(_material);
    yCube->getTargetMaterial()->setEmissive(iaColor3f(0.0f, 1.0f, 0.0f));
    yCube->getTargetMaterial()->setSpecular(iaColor3f(0.0f, 0.0f, 0.0f));
    yCube->getTargetMaterial()->setDiffuse(iaColor3f(0.0f, 0.0f, 0.0f));
    yCube->getTargetMaterial()->setAmbient(iaColor3f(0.0f, 0.0f, 0.0f));
    yTransform->insertNode(yCube);

    iNodeMesh* zCube = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    zCube->setMesh(cubeMesh);
    zCube->setMaterial(_material);
    zCube->getTargetMaterial()->setEmissive(iaColor3f(0.0f, 0.0f, 1.0f));
    zCube->getTargetMaterial()->setSpecular(iaColor3f(0.0f, 0.0f, 0.0f));
    zCube->getTargetMaterial()->setDiffuse(iaColor3f(0.0f, 0.0f, 0.0f));
    zCube->getTargetMaterial()->setAmbient(iaColor3f(0.0f, 0.0f, 0.0f));
    zTransform->insertNode(zCube);
}


void Modifier::createLocatorModifier(shared_ptr<iMesh> &cylinderMesh)
{
    _locatorModifier = iNodeFactory::getInstance().createNode(iNodeType::iNode);
    _switchNode->insertNode(_locatorModifier);

    iNodeTransform* xTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    xTransform->scale(2.0, 0.02, 0.02);
    _locatorModifier->insertNode(xTransform);

    iNodeTransform* yTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    yTransform->rotate(M_PI * 0.5, iaAxis::Z);
    yTransform->scale(2.0, 0.02, 0.02);
    _locatorModifier->insertNode(yTransform);

    iNodeTransform* zTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    zTransform->rotate(-M_PI * 0.5, iaAxis::Y);
    zTransform->scale(2.0, 0.02, 0.02);
    _locatorModifier->insertNode(zTransform);

    iNodeMesh* xCylinder = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    xCylinder->setMesh(cylinderMesh);
    xCylinder->setMaterial(_material);
    xCylinder->getTargetMaterial()->setEmissive(iaColor3f(1.0f, 0.0f, 0.0f));
    xCylinder->getTargetMaterial()->setSpecular(iaColor3f(0.0f, 0.0f, 0.0f));
    xCylinder->getTargetMaterial()->setDiffuse(iaColor3f(0.0f, 0.0f, 0.0f));
    xCylinder->getTargetMaterial()->setAmbient(iaColor3f(0.0f, 0.0f, 0.0f));
    xTransform->insertNode(xCylinder);

    iNodeMesh* yCylinder = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    yCylinder->setMesh(cylinderMesh);
    yCylinder->setMaterial(_material);
    yCylinder->getTargetMaterial()->setEmissive(iaColor3f(0.0f, 1.0f, 0.0f));
    yCylinder->getTargetMaterial()->setSpecular(iaColor3f(0.0f, 0.0f, 0.0f));
    yCylinder->getTargetMaterial()->setDiffuse(iaColor3f(0.0f, 0.0f, 0.0f));
    yCylinder->getTargetMaterial()->setAmbient(iaColor3f(0.0f, 0.0f, 0.0f));
    yTransform->insertNode(yCylinder);

    iNodeMesh* zCylinder = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    zCylinder->setMesh(cylinderMesh);
    zCylinder->setMaterial(_material);
    zCylinder->getTargetMaterial()->setEmissive(iaColor3f(0.0f, 0.0f, 1.0f));
    zCylinder->getTargetMaterial()->setSpecular(iaColor3f(0.0f, 0.0f, 0.0f));
    zCylinder->getTargetMaterial()->setDiffuse(iaColor3f(0.0f, 0.0f, 0.0f));
    zCylinder->getTargetMaterial()->setAmbient(iaColor3f(0.0f, 0.0f, 0.0f));
    zTransform->insertNode(zCylinder);
}

void Modifier::updateCamMatrix(const iaMatrixd& camMatrix)
{
    float64 distanceToCam = camMatrix._pos.distance(_modifierMatrix._pos) * 0.1;
    _rootTransform->setMatrix(_modifierMatrix);
    _rootTransform->scale(distanceToCam, distanceToCam, distanceToCam);
}

void Modifier::deinit()
{
}

void Modifier::setVisible(bool visible)
{
    _visible = visible;
    _rootTransform->setActive(_visible);
}

bool Modifier::isVisible() const
{
    return _visible;
}

void Modifier::updateMatrices()
{
    if (_dirtyMatrices)
    {
        _umbrellaMatrix.identity();
        _umbrellaMatrix.translate(1.5, 0, 0);
        _umbrellaMatrix.scale(0.5, 0.1, 0.1);

        if (_modifierMode == ModifierMode::Locator)
        {
            _cylinderMatrix.identity();
            _cylinderMatrix.scale(2.0, 0.02, 0.02);
        }
        else
        {
            _cylinderMatrix.identity();
            _cylinderMatrix.scale(1.5, 0.02, 0.02);
        }

        _cubeMatrix.identity();
        _cubeMatrix.translate(1.5, 0, 0);
        _cubeMatrix.scale(0.2, 0.2, 0.2);

        _dirtyMatrices = false;
    }
}

shared_ptr<iMesh> Modifier::createCylinder()
{
    iMeshBuilder meshBuilder;

    const uint32 segments = 8;
    const float32 step = (2.0f * M_PI) / segments;

    for (uint32 i = 0; i < segments; ++i)
    {
        meshBuilder.addVertex(iaVector3f(0, sin(i*step), cos(i*step)));
        meshBuilder.addVertex(iaVector3f(1, sin(i*step), cos(i*step)));
    }

    meshBuilder.addVertex(iaVector3f(0, 0, 0));
    meshBuilder.addVertex(iaVector3f(1, 0, 0));

    const uint32 segmentVertices = 2;
    const uint32 moduloValue = segments * segmentVertices;

    for (uint32 segment = 0; segment < segments; ++segment)
    {
        uint32 segmentIndex = segment * segmentVertices;
        meshBuilder.addTriangle((segmentIndex + 0) % moduloValue, (segmentIndex + 1) % moduloValue, (segmentIndex + 2) % moduloValue);
        meshBuilder.addTriangle((segmentIndex + 1) % moduloValue, (segmentIndex + 3) % moduloValue, (segmentIndex + 2) % moduloValue);

        meshBuilder.addTriangle((segmentIndex + 2) % moduloValue, meshBuilder.getVertexCount() - 2, (segmentIndex + 0) % moduloValue);
        meshBuilder.addTriangle((segmentIndex + 1) % moduloValue, meshBuilder.getVertexCount() - 1, (segmentIndex + 3) % moduloValue);
    }

    return meshBuilder.createMesh();
}

shared_ptr<iMesh> Modifier::createCube()
{
    iMeshBuilder meshBuilder;

    meshBuilder.addVertex(iaVector3f(0, -0.5, 0.5));
    meshBuilder.addVertex(iaVector3f(1, -0.5, 0.5));
    meshBuilder.addVertex(iaVector3f(1, 0.5, 0.5));
    meshBuilder.addVertex(iaVector3f(0, 0.5, 0.5));
    meshBuilder.addVertex(iaVector3f(0, -0.5, -0.5));
    meshBuilder.addVertex(iaVector3f(1, -0.5, -0.5));
    meshBuilder.addVertex(iaVector3f(1, 0.5, -0.5));
    meshBuilder.addVertex(iaVector3f(0, 0.5, -0.5));

    meshBuilder.addTriangle(0, 1, 2);
    meshBuilder.addTriangle(0, 2, 3);
    meshBuilder.addTriangle(5, 4, 7);
    meshBuilder.addTriangle(5, 7, 6);
    meshBuilder.addTriangle(1, 5, 2);
    meshBuilder.addTriangle(5, 6, 2);
    meshBuilder.addTriangle(4, 0, 3);
    meshBuilder.addTriangle(4, 3, 7);
    meshBuilder.addTriangle(3, 2, 7);
    meshBuilder.addTriangle(7, 2, 6);
    meshBuilder.addTriangle(1, 0, 4);
    meshBuilder.addTriangle(1, 4, 5);

    return meshBuilder.createMesh();
}

shared_ptr<iMesh> Modifier::createUmbrella()
{
    iMeshBuilder meshBuilder;

    const uint32 segments = 16;
    const float32 step = (2.0f * M_PI) / segments;

    for (uint32 i = 0; i < segments; ++i)
    {
        meshBuilder.addVertex(iaVector3f(0, sin(i*step), cos(i*step)));
    }

    meshBuilder.addVertex(iaVector3f(0, 0, 0));
    meshBuilder.addVertex(iaVector3f(1, 0, 0));

    const uint32 moduloValue = segments;

    for (uint32 segmentIndex = 0; segmentIndex < segments; ++segmentIndex)
    {
        meshBuilder.addTriangle((segmentIndex + 0) % moduloValue, meshBuilder.getVertexCount() - 1, (segmentIndex + 1) % moduloValue);
        meshBuilder.addTriangle((segmentIndex + 1) % moduloValue, meshBuilder.getVertexCount() - 2, (segmentIndex + 0) % moduloValue);
    }

    return meshBuilder.createMesh();
}

void Modifier::setModifierMode(ModifierMode modifierMode)
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
        // TODO
        break;
    }
}

ModifierMode Modifier::getModifierMode() const
{
    return _modifierMode;
}

void Modifier::setMatrix(const iaMatrixd& matrix)
{
    _modifierMatrix = matrix;
}

void Modifier::getMatrix(iaMatrixd& matrix) const
{
    matrix = _modifierMatrix;
}
