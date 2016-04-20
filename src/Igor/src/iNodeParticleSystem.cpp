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

            if (emitter != nullptr)
            {
                _particleSystem.setParticleSystemMatrix(_worldMatrixInv);
                _particleSystem.calcNextFrame(emitter->getParticleEmitter());

                iRenderer::getInstance().bindTexture(_textureA, 0);
                iRenderer::getInstance().bindTexture(_textureB, 1);
                iRenderer::getInstance().bindTexture(_textureC, 2);

                iRenderer::getInstance().setModelMatrix(_worldMatrix);
                iRenderer::getInstance().setColor(1, 1, 1, 1);
                iRenderer::getInstance().drawParticles(&(_particleSystem.getCurrentFrame()), nullptr);
            }
        }
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


    iaString iNodeParticleSystem::getTextureA() const
    {
        iaString result;
        if (_textureA != nullptr)
        {
            result = _textureA->getFilename();
        }
        return result;
    }

    iaString iNodeParticleSystem::getTextureB() const
    {
        iaString result;
        if (_textureB != nullptr)
        {
            result = _textureB->getFilename();
        }
        return result;
    }

    iaString iNodeParticleSystem::getTextureC() const
    {
        iaString result;
        if (_textureC != nullptr)
        {
            result = _textureC->getFilename();
        }
        return result;
    }

    void iNodeParticleSystem::setTextureA(const iaString& texture)
    {
        _textureA = iTextureResourceFactory::getInstance().requestFile(texture);
    }

    void iNodeParticleSystem::setTextureB(const iaString& texture)
    {
        _textureB = iTextureResourceFactory::getInstance().requestFile(texture);
    }

    void iNodeParticleSystem::setTextureC(const iaString& texture)
    {
        _textureC = iTextureResourceFactory::getInstance().requestFile(texture);
    }

};
