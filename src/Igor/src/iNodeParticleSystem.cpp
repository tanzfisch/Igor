// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeParticleSystem.h>

#include <iRenderer.h>
#include <iTextureResourceFactory.h>
#include <iRainbow.h>
#include <iNodeFactory.h>
#include <iNodeEmitter.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iNodeParticleSystem::iNodeParticleSystem()
	{
        setName(L"iNodeParticleSystem");
        _nodeType = iNodeType::iNodeParticleSystem;
        _nodeKind = iNodeKind::Volume;
	}

    iNodeParticleSystem::iNodeParticleSystem(iNodeParticleSystem* node)
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = node->_nodeType;
        _nodeKind = node->_nodeKind;

        setName(node->getName());

        // TODO copy particle system config
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
        iNodeEmitter* emitter = nullptr;
        if (_emitterID != iNode::INVALID_NODE_ID)
        {
            emitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().getNode(_emitterID));
        }
        if (emitter != nullptr)
        {
            _particleSystem.setParticleSystemMatrix(_worldMatrixInv);
            _particleSystem.setEmitterData(emitter->getTriangles(), emitter->getWorldMatrix());
            _particleSystem.calcNextFrame(); // todo does not belong in draw function
        }
        else
        {
            _particleSystem.calcNextFrame();
        }

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

    void iNodeParticleSystem::onUpdateTransform(iaMatrixf& matrix)
    {
        _worldMatrix = matrix;
        _worldMatrixInv = matrix;
        _worldMatrixInv.invert();
    }

    void iNodeParticleSystem::setEmitter(uint64 emitterID)
    {
        _emitterID = emitterID;
    }

    uint64 iNodeParticleSystem::getEmitter() const
    {
        return _emitterID;
    }

};
