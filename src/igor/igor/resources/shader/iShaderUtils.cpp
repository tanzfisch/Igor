// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/shader/iShaderUtils.h>

#include <igor/resources/iResourceManager.h>
#include <igor/resources/mesh/iMeshBuilderUtils.h>
#include <igor/renderer/iRenderer.h>

#include <iaux/data/iaConvert.h>
using namespace iaux;

namespace igor
{
    iPixmapPtr iShaderUtils::materialToPixmap(iMaterialPtr material, uint32 width, uint32 height)
    {
        auto shader = material->getShader();

        // store current render states
        iRenderer::getInstance().flush();
        const iaRectanglei viewport = iRenderer::getInstance().getViewport();
        const iaMatrixd projectionMatrix = iRenderer::getInstance().getProjectionMatrix();
        const iaMatrixd modelMatrix = iRenderer::getInstance().getModelMatrix();

        uint32 renderTarget = iRenderer::getInstance().createRenderTarget(width, height, iColorFormat::RGBA, iRenderTargetType::ToRenderBuffer, true);
        iRenderer::getInstance().setRenderTarget(renderTarget);
        iRenderer::getInstance().clearColorBuffer(iaColor4f::transparent);

        iRenderer::getInstance().setViewport(0, 0, width, height);
        iRenderer::getInstance().setPerspective(45.0, 0.00001, 10.0);

        iMeshBuilder meshBuilder;
        iMeshBuilderUtils::addSphere(meshBuilder, 0.5, 32, material->hasTextures());
        meshBuilder.calcNormals(true);
        iMeshPtr sphere = meshBuilder.createMesh();

        iaMatrixd matrix;
        matrix.translate(0, 0, -1.31);
        iRenderer::getInstance().setModelMatrix(matrix);

        iRenderer::getInstance().setShader(shader);

        if (shader->getRenderState(iRenderState::Instanced) == iRenderStateValue::On)
        {
            iaMatrixf matrixf;
            iaConvert::convert(matrix, matrixf);
            iInstancingBufferPtr instancingBuffer = iInstancingBuffer::create(std::vector<iBufferLayoutEntry>{{iShaderDataType::Matrix4x4}});
            instancingBuffer->addInstance(sizeof(iaMatrixf), matrixf.getData());
            iRenderer::getInstance().drawMeshInstanced(sphere, instancingBuffer, material);
        }
        else
        {
            iRenderer::getInstance().drawMesh(sphere, material);
        }

        iPixmapPtr pixmap = iPixmap::createPixmap(width, height, iColorFormat::RGBA);

        iRenderer::getInstance().readPixels(0, 0, width, height, iColorFormat::RGBA, pixmap->getData());

        iRenderer::getInstance().setRenderTarget();
        iRenderer::getInstance().destroyRenderTarget(renderTarget);

        // restore everything
        iRenderer::getInstance().setModelMatrix(modelMatrix);
        iRenderer::getInstance().setProjectionMatrix(projectionMatrix);
        iRenderer::getInstance().setViewport(viewport);

        return pixmap;
    }

    iPixmapPtr iShaderUtils::shaderToPixmap(iShaderPtr shader, uint32 width, uint32 height)
    {
        if (shader == nullptr)
        {
            return nullptr;
        }

        iParameters param({
            {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
            {IGOR_RESOURCE_PARAM_GENERATE, true},
            {IGOR_RESOURCE_PARAM_SHADER, shader},
            {IGOR_RESOURCE_PARAM_CACHE_MODE, iResourceCacheMode::Free}, // drop it right after this use
            {IGOR_RESOURCE_PARAM_AMBIENT, iaColor3f(0.5f, 0.5f, 0.5f)},
            {IGOR_RESOURCE_PARAM_DIFFUSE, iaColor3f(0.5f, 0.5f, 0.5f)},
            {IGOR_RESOURCE_PARAM_SPECULAR, iaColor3f(0.5f, 0.5f, 0.5f)},
            {IGOR_RESOURCE_PARAM_EMISSIVE, iaColor3f(0.0f, 0.0f, 0.0f)},
            {IGOR_RESOURCE_PARAM_ALPHA, 1.0f},
        });
        iMaterialPtr material = iResourceManager::getInstance().loadResource<iMaterial>(param);

        return materialToPixmap(material, width, height);
    }
}