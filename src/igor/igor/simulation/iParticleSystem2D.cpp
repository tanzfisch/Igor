// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/simulation/iParticleSystem2D.h>

#include <igor/resources/material/iMaterialResourceFactory.h>
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

        _particleTargetMaterial = iTargetMaterial::create();
        _particleTargetMaterial->setTexture(iResourceManager::getInstance().requestResource<iTexture>("white_texture"), 0);
        _particleTargetMaterial->setTexture(iResourceManager::getInstance().requestResource<iTexture>("white_texture"), 1);
        _particleTargetMaterial->setTexture(iResourceManager::getInstance().requestResource<iTexture>("white_texture"), 2);
        _particlesMaterial = iMaterialResourceFactory::getInstance().loadMaterial("igor/materials/particles_ortho_projection.mat");
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

    iTargetMaterialPtr &iParticleSystem2D::getTargetMaterial()
    {
        return _particleTargetMaterial;
    }

}; // namespace igor
