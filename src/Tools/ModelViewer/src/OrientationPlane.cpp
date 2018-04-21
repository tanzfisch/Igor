// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "OrientationPlane.h"

#include <iMaterialResourceFactory.h>
#include <iTargetMaterial.h>
#include <iRenderer.h>
#include <iMeshBuilder.h>
#include <iNodeFactory.h>
#include <iRenderStateSet.h>
#include <iNodeTransform.h>
#include <iNodeMesh.h>
#include <iMeshBuilderUtils.h>
#include <iMesh.h>
#include <iScene.h>
#include <iTextureResourceFactory.h>
using namespace Igor;

OrientationPlane::OrientationPlane(iScene* scene)
{
    con_assert(scene != nullptr, "zero pointer");

    _scene = scene;

    init();
}

void OrientationPlane::init()
{
    _material = iMaterialResourceFactory::getInstance().createMaterial("OrientationPlane");
    auto material = iMaterialResourceFactory::getInstance().getMaterial(_material);
    material->addShaderSource("igor/default.vert", iShaderObjectType::Vertex);
    material->addShaderSource("igor/default_directional_light.frag", iShaderObjectType::Fragment);
    material->compileShader();
    material->getRenderStateSet().setRenderState(iRenderState::CullFace, iRenderStateValue::Off);
    material->setOrder(iMaterial::RENDER_ORDER_MAX);

    _white = iMaterialResourceFactory::getInstance().createTargetMaterial();
    _white->setEmissive(iaColor3f(0.0f, 0.0f, 0.0f));
    _white->setSpecular(iaColor3f(1.0f, 1.0f, 1.0f));
    _white->setDiffuse(iaColor3f(1.0f, 1.0f, 1.0f));
    _white->setAmbient(iaColor3f(1.0f, 1.0f, 1.0f));

    shared_ptr<iMesh> gridMesh = createGridMesh();

    iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));
    transformNode->setActive(_visible);
    _transformNodeID = transformNode->getID();

    iNodeMesh* meshNode = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    meshNode->setMesh(gridMesh);
    meshNode->setMaterial(_material);
    meshNode->setTargetMaterial(_white);

    transformNode->insertNode(meshNode);
    _scene->getRoot()->insertNode(transformNode);
}

void OrientationPlane::deinit()
{
    iMaterialResourceFactory::getInstance().destroyTargetMaterial(_white);
}

void OrientationPlane::setVisible(bool visible)
{
    _visible = visible;

    iNode* node = iNodeFactory::getInstance().getNode(_transformNodeID);
    node->setActive(_visible);
}

bool OrientationPlane::isVisible() const
{
    return _visible;
}

shared_ptr<iMesh> OrientationPlane::createGridMesh()
{
    iMeshBuilder meshBuilder;

    const float32 size = 10;

    iMeshBuilderUtils::addRectangle(meshBuilder, -size*0.5, -size*0.5, size, size);

    return meshBuilder.createMesh();
}

