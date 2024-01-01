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

        _particleTargetMaterial = iMaterial::create();
        _particleTargetMaterial->setTexture(iResourceManager::getInstance().requestResource<iTexture>("igor_texture_white"), 0);
        _particleTargetMaterial->setTexture(iResourceManager::getInstance().requestResource<iTexture>("igor_texture_white"), 1);
        _particleTargetMaterial->setTexture(iResourceManager::getInstance().requestResource<iTexture>("igor_texture_white"), 2);
        _particlesMaterial = iResourceManager::getInstance().loadResource<iShaderMaterial>("igor_material_textured_particles_ortho");
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

        iRenderer::getInstance().setMaterial(_particlesMaterial);
        _particleTargetMaterial->setVelocityOriented(_particleSystem.getVelocityOriented());
        _particleTargetMaterial->setTilingConfig(iaVector2f(_particleSystem.getTextureColumns(), _particleSystem.getTextureRows()));
        iRenderer::getInstance().drawBuffer(_particleSystem.getVertexArray(), iRenderPrimitive::Points, _particleTargetMaterial);

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

    iMaterialPtr &iParticleSystem2D::getTargetMaterial()
    {
        return _particleTargetMaterial;
    }

}; // namespace igor
