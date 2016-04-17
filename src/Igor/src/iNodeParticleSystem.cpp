// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeParticleSystem.h>

#include <iRenderer.h>
#include <iTextureResourceFactory.h>
#include <iRainbow.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iNodeParticleSystem::iNodeParticleSystem()
	{
        
	}

    iNodeParticleSystem::iNodeParticleSystem(iNodeParticleSystem* node)
    {

    }

    iNodeParticleSystem::~iNodeParticleSystem()
	{
	}

    iParticleSystem3D& iNodeParticleSystem::getParticleSystem()
    {
        return _particleSystem;
    }

    void iNodeParticleSystem::draw()
    {
        _particleSystem.calcNextFrame();
        iRenderer::getInstance().setColor(1,1,1,1);
        iRenderer::getInstance().setModelMatrix(_worldMatrix);
        shared_ptr<iTexture> textureA = _particleSystem.getTextureA();
        shared_ptr<iTexture> textureB = _particleSystem.getTextureB();
        shared_ptr<iTexture> textureC = _particleSystem.getTextureC();
        if (textureA != nullptr)
        {
            iRenderer::getInstance().bindTexture(textureA, 0);
        }
        if (textureB != nullptr)
        {
            iRenderer::getInstance().bindTexture(textureB, 1);
        }
        if (textureC != nullptr)
        {
            iRenderer::getInstance().bindTexture(textureC, 2);
        }
        iRenderer::getInstance().drawParticles(&(_particleSystem.getCurrentFrame()), nullptr);
    }

};
