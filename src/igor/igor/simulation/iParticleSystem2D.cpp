// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/simulation/iParticleSystem2D.h>

#include <igor/resources/iResourceManager.h>

#include <igor/renderer/iRenderer.h>

namespace igor
{

    iParticleSystem2D::iParticleSystem2D()
    {
        init();
    }

    void iParticleSystem2D::init()
    {
        _particleSystem.setMaxParticleCount(100);
        _particleSystem.start();

        iShaderMaterialPtr shaderMaterial = iResourceManager::getInstance().loadResource<iShaderMaterial>("igor_shader_material_textured_particles_ortho");
        iParameters param({
            {IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
            {IGOR_RESOURCE_PARAM_GENERATE, true},
            {IGOR_RESOURCE_PARAM_SHADER_MATERIAL, shaderMaterial},
            {IGOR_RESOURCE_PARAM_TEXTURE0, "igor_texture_white"},
            {IGOR_RESOURCE_PARAM_TEXTURE1, "igor_texture_white"},
            {IGOR_RESOURCE_PARAM_TEXTURE2, "igor_texture_white"},
        });
        _material = iResourceManager::getInstance().loadResource<iMaterial>(param);
    }

    void iParticleSystem2D::update()
    {
        _particleSystem.onUpdate(_particleEmitter);
    }

    void iParticleSystem2D::draw(const iaMatrixd &matrix)
    {
        update();

        iaMatrixd modelMatrix = iRenderer::getInstance().getModelMatrix();

        iRenderer::getInstance().setModelMatrix(modelMatrix * matrix);

        _material->setVelocityOriented(_particleSystem.getVelocityOriented());
        _material->setTiling(iaVector2f(_particleSystem.getTextureColumns(), _particleSystem.getTextureRows()));
        iRenderer::getInstance().drawBuffer(_particleSystem.getVertexArray(), iRenderPrimitive::Points, _material);

        iRenderer::getInstance().setModelMatrix(modelMatrix);
    }

    iParticleSystem &iParticleSystem2D::getSystem()
    {
        return _particleSystem;
    }

    iParticleEmitter &iParticleSystem2D::getEmitter()
    {
        return _particleEmitter;
    }

    iMaterialPtr &iParticleSystem2D::getMaterial()
    {
        return _material;
    }

}; // namespace igor
