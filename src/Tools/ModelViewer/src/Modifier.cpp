// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "Modifier.h"

#include <iRenderer.h>
#include <iMeshBuilder.h>
using namespace Igor;

void Modifier::init()
{
    initUmbrella();
    initCylinder();
}

void Modifier::initCylinder()
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

    shared_ptr<iMesh> mesh = meshBuilder.createMesh();

    _cylinder = iRenderer::getInstance().createBuffers(mesh);
}

void Modifier::initUmbrella()
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

    shared_ptr<iMesh> mesh = meshBuilder.createMesh();

    _umbrella = iRenderer::getInstance().createBuffers(mesh);
}

void Modifier::draw()
{
    iaMatrixd modelMatrixCopy;
    iRenderer::getInstance().getModelMatrix(modelMatrixCopy);

    iaMatrixd modelMatrix = modelMatrixCopy;
    iaMatrixd umbrellaMatrix;
    umbrellaMatrix.translate(2,0,0);
    umbrellaMatrix.scale(1.5, 0.5, 0.5);
    modelMatrix *= umbrellaMatrix;
    iRenderer::getInstance().setModelMatrix(modelMatrix);
    iRenderer::getInstance().drawMesh(_umbrella);

    modelMatrix = modelMatrixCopy;
    iaMatrixd cylinderMatrix;
    cylinderMatrix.scale(2.0, 0.1, 0.1);
    modelMatrix *= cylinderMatrix;
    iRenderer::getInstance().setModelMatrix(modelMatrix);
    iRenderer::getInstance().drawMesh(_cylinder);
}


