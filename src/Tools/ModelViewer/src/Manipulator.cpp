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

void Manipulator::createTranslateModifier(shared_ptr<iMesh> &cylinderMesh, shared_ptr<iMesh> &umbrellaMesh)
{
    _translateModifier = iNodeFactory::getInstance().createNode(iNodeType::iNode);
    _switchNode->insertNode(_translateModifier);

    iNodeTransform* xTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
    xTransform->scale(0.02, 1.5, 0.02);
    _translateModifier->insertNode(xTransform);

    iNodeTransform* yTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    yTransform->scale(0.02, 1.5, 0.02);
    _translateModifier->insertNode(yTransform);

    iNodeTransform* zTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    zTransform->rotate(M_PI * 0.5, iaAxis::X);
    zTransform->scale(0.02, 1.5, 0.02);
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
    xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
    xTransform->translate(0, 1.5, 0);
    xTransform->scale(0.1, 0.5, 0.1);
    _translateModifier->insertNode(xTransform);

    yTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    yTransform->translate(0, 1.5, 0);
    yTransform->scale(0.1, 0.5, 0.1);
    _translateModifier->insertNode(yTransform);

    zTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    zTransform->rotate(M_PI * 0.5, iaAxis::X);
    zTransform->translate(0, 1.5, 0);
    zTransform->scale(0.1, 0.5, 0.1);
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

void Manipulator::createScaleModifier(shared_ptr<iMesh> &cylinderMesh, shared_ptr<iMesh> &cubeMesh)
{
    _scaleModifier = iNodeFactory::getInstance().createNode(iNodeType::iNode);
    _switchNode->insertNode(_scaleModifier);

    iNodeTransform* xTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
    xTransform->scale(0.02, 1.5, 0.02);
    _scaleModifier->insertNode(xTransform);

    iNodeTransform* yTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    yTransform->scale(0.02, 1.5, 0.02);
    _scaleModifier->insertNode(yTransform);

    iNodeTransform* zTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    zTransform->rotate(M_PI * 0.5, iaAxis::X);
    zTransform->scale(0.02, 1.5, 0.02);
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
    xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
    xTransform->translate(0, 1.5, 0);
    xTransform->scale(0.25, 0.35, 0.25);
    _scaleModifier->insertNode(xTransform);

    yTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    yTransform->translate(0, 1.5, 0);
    yTransform->scale(0.25, 0.25, 0.25);
    _scaleModifier->insertNode(yTransform);

    zTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    zTransform->rotate(M_PI * 0.5, iaAxis::X);
    zTransform->translate(0, 1.5, 0);
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

void Manipulator::createLocatorModifier(shared_ptr<iMesh> &cylinderMesh)
{
    _locatorModifier = iNodeFactory::getInstance().createNode(iNodeType::iNode);
    _switchNode->insertNode(_locatorModifier);

    iNodeTransform* xTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    xTransform->rotate(-M_PI * 0.5, iaAxis::Z);
    xTransform->scale(0.02, 2.0, 0.02);
    _locatorModifier->insertNode(xTransform);

    iNodeTransform* yTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    yTransform->scale(0.02, 2.0, 0.02);
    _locatorModifier->insertNode(yTransform);

    iNodeTransform* zTransform = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    zTransform->rotate(M_PI * 0.5, iaAxis::X);
    zTransform->scale(0.02, 2.0, 0.02);
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

void Manipulator::updateCamMatrix(const iaMatrixd& camMatrix)
{
    float64 distanceToCam = camMatrix._pos.distance(_modifierMatrix._pos) * 0.1;
    _rootTransform->setMatrix(_modifierMatrix);
    _rootTransform->scale(distanceToCam, distanceToCam, distanceToCam);
}

void Manipulator::deinit()
{
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

void Manipulator::updateMatrices()
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

shared_ptr<iMesh> Manipulator::createCylinder()
{
    iMeshBuilder meshBuilder;
    iMeshBuilderUtils::addCylinder(meshBuilder, 1, 1, 8);
    return meshBuilder.createMesh();
}

shared_ptr<iMesh> Manipulator::createCube()
{
    iMeshBuilder meshBuilder;
    iMeshBuilderUtils::addBox(meshBuilder, 1, 1, 1);
    return meshBuilder.createMesh();
}

shared_ptr<iMesh> Manipulator::createUmbrella()
{
    iMeshBuilder meshBuilder;
    iMeshBuilderUtils::addCone(meshBuilder, 1, 1, 8);
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
        // TODO
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
