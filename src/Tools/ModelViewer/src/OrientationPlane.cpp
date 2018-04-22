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

#include <iaRandomNumberGenerator.h>
using namespace IgorAux;

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
    material->addShaderSource("igor/textured.vert", iShaderObjectType::Vertex);
    material->addShaderSource("igor/textured_directional_light.frag", iShaderObjectType::Fragment);
    material->compileShader();
    material->getRenderStateSet().setRenderState(iRenderState::CullFace, iRenderStateValue::Off);
    material->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    material->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    material->setOrder(iMaterial::RENDER_ORDER_MAX);

    _white = iMaterialResourceFactory::getInstance().createTargetMaterial();
    _white->setEmissive(iaColor3f(0.8f, 0.8f, 0.8f));
    _white->setSpecular(iaColor3f(0.8f, 0.8f, 0.8f));
    _white->setDiffuse(iaColor3f(0.8f, 0.8f, 0.8f));
    _white->setAmbient(iaColor3f(0.8f, 0.8f, 0.8f));
    _white->setTexture(iTextureResourceFactory::getInstance().requestFile("lineGradient.png"), 0);

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

void addLine(iMeshBuilder& meshBuilder, float32 x, float32 z, float32 width, float32 length, float32 angle)
{
    uint32 firstIndex = meshBuilder.getVertexCount();
    float32 halfWidth = width * 0.5f;

    iaMatrixf matrixCopy;
    meshBuilder.getMatrix(matrixCopy);

    iaMatrixf matrix;
    matrix.rotate(angle, iaAxis::Y);
    meshBuilder.multMatrix(matrix);

    meshBuilder.addVertex(iaVector3f(x, 0, z + halfWidth));
    meshBuilder.setTexCoord(iaVector2f(0, 0), 0);
    meshBuilder.addVertex(iaVector3f(x + length, 0, z + halfWidth));
    meshBuilder.setTexCoord(iaVector2f(1, 0), 0);
    meshBuilder.addVertex(iaVector3f(x + length, 0, z - halfWidth));
    meshBuilder.setTexCoord(iaVector2f(1, 1), 0);
    meshBuilder.addVertex(iaVector3f(x, 0, z - halfWidth));
    meshBuilder.setTexCoord(iaVector2f(0, 1), 0);

    meshBuilder.addTriangle(0, 1, 2, firstIndex);
    meshBuilder.addTriangle(2, 3, 0, firstIndex);

    firstIndex = meshBuilder.getVertexCount();

    meshBuilder.addVertex(iaVector3f(x, -halfWidth, z));
    meshBuilder.setTexCoord(iaVector2f(0, 0), 0);
    meshBuilder.addVertex(iaVector3f(x + length, -halfWidth, z));
    meshBuilder.setTexCoord(iaVector2f(1, 0), 0);
    meshBuilder.addVertex(iaVector3f(x + length, halfWidth, z));
    meshBuilder.setTexCoord(iaVector2f(1, 1), 0);
    meshBuilder.addVertex(iaVector3f(x, halfWidth, z));
    meshBuilder.setTexCoord(iaVector2f(0, 1), 0);
    
    meshBuilder.addTriangle(0, 1, 2, firstIndex);
    meshBuilder.addTriangle(2, 3, 0, firstIndex);

    meshBuilder.setMatrix(matrixCopy);
}

shared_ptr<iMesh> OrientationPlane::createGridMesh()
{
    iaRandomNumberGenerator rand;
    iMeshBuilder meshBuilder;
    meshBuilder.setJoinVertexes(false);

    const float32 size = 20.0f;
    const float32 lineWidth = 0.025f;
    const float32 thinLineWidth = lineWidth * 0.25f;
    const float32 step = 1.0f;

    addLine(meshBuilder, -size * 0.5f, 0, lineWidth, size, 0);
    addLine(meshBuilder, -size * 0.5f, 0, lineWidth, size, M_PI_2);

    for (int i = 0; i < 9; ++i)
    {
        addLine(meshBuilder, -size * 0.5f, step + i * step, thinLineWidth, size, 0);
        addLine(meshBuilder, -size * 0.5f, -step - i * step, thinLineWidth, size, 0);

        addLine(meshBuilder, -size * 0.5f, step + i * step, thinLineWidth, size, M_PI_2);
        addLine(meshBuilder, -size * 0.5f, -step - i * step, thinLineWidth, size, M_PI_2);
    }

    return meshBuilder.createMesh();
}


