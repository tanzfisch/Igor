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
        //iRenderer::getInstance().drawBox(iaVector3f(-1, -1, -1), iaVector3f(1, 1, 1));
        iRenderer::getInstance().drawParticles(&(_particleSystem.getCurrentFrame()), iaMatrixf(), nullptr);
    }

};
