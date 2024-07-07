// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/environment/iSkyBox.h>

#include <igor/renderer/iRenderer.h>
#include <igor/resources/shader/iShader.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/resources/mesh/iMeshBuilder.h>
#include <igor/resources/iResourceManager.h>

namespace igor
{
    iSkyBox::iSkyBox()
    {
        _mesh = buildMesh();
        iShaderPtr skyboxShader = iResourceManager::getInstance().loadResource<iShader>("igor_shader_material_skybox");
        iParameters paramSkybox({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
                                 {IGOR_RESOURCE_PARAM_GENERATE, true},
                                 {IGOR_RESOURCE_PARAM_SHADER, skyboxShader}});
        _material = iResourceManager::getInstance().loadResource<iMaterial>(paramSkybox);
    }

    void iSkyBox::setTexture(iTexturePtr texture)
    {
        _material->setTexture(texture, 0);
    }

    iMeshPtr iSkyBox::buildMesh()
    {
        iMeshBuilder meshBuilder;
        // manually set triangle indices
        meshBuilder.setJoinVertices(false);

        // top
        uint32 a = meshBuilder.addVertex(iaVector3f(-0.5, 0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.25, 0.3333), 0);
        uint32 b = meshBuilder.addVertex(iaVector3f(0.5, 0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.5, 0.3333), 0);
        uint32 c = meshBuilder.addVertex(iaVector3f(0.5, 0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.5, 0.0), 0);
        uint32 d = meshBuilder.addVertex(iaVector3f(-0.5, 0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.25, 0.0), 0);
        meshBuilder.addTriangle(a, b, c);
        meshBuilder.addTriangle(a, c, d);

        // left
        a = meshBuilder.addVertex(iaVector3f(-0.5, 0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.25, 0.3333), 0);
        b = meshBuilder.addVertex(iaVector3f(-0.5, 0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.0, 0.3333), 0);
        c = meshBuilder.addVertex(iaVector3f(-0.5, -0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.0, 0.6666), 0);
        d = meshBuilder.addVertex(iaVector3f(-0.5, -0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.25, 0.6666), 0);

        meshBuilder.addTriangle(a, b, d);
        meshBuilder.addTriangle(b, c, d);

        // front
        a = meshBuilder.addVertex(iaVector3f(-0.5, 0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.25, 0.3333), 0);
        b = meshBuilder.addVertex(iaVector3f(0.5, 0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.5, 0.3333), 0);
        c = meshBuilder.addVertex(iaVector3f(0.5, -0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.5, 0.6666), 0);
        d = meshBuilder.addVertex(iaVector3f(-0.5, -0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.25, 0.6666), 0);
        meshBuilder.addTriangle(a, d, b);
        meshBuilder.addTriangle(d, c, b);

        // right
        a = meshBuilder.addVertex(iaVector3f(0.5, 0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.5, 0.3333), 0);
        b = meshBuilder.addVertex(iaVector3f(0.5, 0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.75, 0.3333), 0);
        c = meshBuilder.addVertex(iaVector3f(0.5, -0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.75, 0.6666), 0);
        d = meshBuilder.addVertex(iaVector3f(0.5, -0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.5, 0.6666), 0);
        meshBuilder.addTriangle(a, d, b);
        meshBuilder.addTriangle(b, d, c);

        // back
        a = meshBuilder.addVertex(iaVector3f(-0.5, 0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(1.0, 0.3333), 0);
        b = meshBuilder.addVertex(iaVector3f(0.5, 0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.75, 0.3333), 0);
        c = meshBuilder.addVertex(iaVector3f(0.5, -0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.75, 0.6666), 0);
        d = meshBuilder.addVertex(iaVector3f(-0.5, -0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(1.0, 0.6666), 0);
        meshBuilder.addTriangle(a, b, d);
        meshBuilder.addTriangle(b, c, d);

        // bottom
        a = meshBuilder.addVertex(iaVector3f(-0.5, -0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.25, 0.6666), 0);
        b = meshBuilder.addVertex(iaVector3f(0.5, -0.5, -0.5));
        meshBuilder.setTexCoord(iaVector2f(0.5, 0.6666), 0);
        c = meshBuilder.addVertex(iaVector3f(0.5, -0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.5, 1.0), 0);
        d = meshBuilder.addVertex(iaVector3f(-0.5, -0.5, 0.5));
        meshBuilder.setTexCoord(iaVector2f(0.25, 1.0), 0);
        meshBuilder.addTriangle(a, d, b);
        meshBuilder.addTriangle(d, c, b);

        return meshBuilder.createMesh();
    }

    void iSkyBox::setBoxSize(float32 boxSize)
    {
        _boxSize = boxSize;
    }

    float32 iSkyBox::getBoxSize() const
    {
        return _boxSize;
    }

    void iSkyBox::render()
    {
        const iaMatrixd &camMatrix = iRenderer::getInstance().getCamMatrix();

        iaMatrixd model;
        model._pos = camMatrix._pos;
        model.scale(_boxSize, _boxSize, _boxSize);
        iRenderer::getInstance().setModelMatrix(model);
        iRenderer::getInstance().setShader(_material->getShader());
        iRenderer::getInstance().drawMesh(_mesh, _material);
    }

} // namespace igor