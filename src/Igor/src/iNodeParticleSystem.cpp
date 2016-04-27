// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeParticleSystem.h>

#include <iRenderer.h>
#include <iTextureResourceFactory.h>
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

        _particleSystem = node->_particleSystem;

        setTextureA(node->getTextureA());
        setTextureB(node->getTextureB());
        setTextureC(node->getTextureC());
    }

    void iNodeParticleSystem::setColor(const iaColor4f& color)
    {
        _color = color;
        _rainbowActive = false;
    }

    const iaColor4f& iNodeParticleSystem::getColor() const
    {
        return _color;
    }

    void iNodeParticleSystem::setRainbowAcive(bool flag)
    {
        _rainbowActive = flag;
    }

    bool iNodeParticleSystem::isRainbowActive() const
    {
        return _rainbowActive;
    }

    void iNodeParticleSystem::setRainbow(const iGradientColor4f& rainbow)
    {
        _rainbow = rainbow;
        _rainbowActive = true;
    }

    void iNodeParticleSystem::getRainbow(iGradientColor4f& rainbow) const
    {
      rainbow = _rainbow;
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
                if (_rainbowActive)
                {
                    iRenderer::getInstance().drawParticles(&(_particleSystem.getCurrentFrame()), _rainbow);
                }
                else
                {
                    iRenderer::getInstance().setColor(_color);
                    iRenderer::getInstance().drawParticles(&(_particleSystem.getCurrentFrame()));
                }
            }
        }
    }

    void iNodeParticleSystem::setParticleCount(uint32 count)
    {
        _particleSystem.setParticleCount(count);
    }

    uint32 iNodeParticleSystem::getParticleCount()
    {
        return _particleSystem.getParticleCount();
    }

    void iNodeParticleSystem::setParticleLifeTime(uint32 frames)
    {
        _particleSystem.setParticleLifeTime(frames);
    }

    uint32 iNodeParticleSystem::getParticleLifeTime()
    {
        return _particleSystem.getParticleLifeTime();
    }

    void iNodeParticleSystem::setVortexParticleCount(uint32 count)
    {
        _particleSystem.setVortexParticleCount(count);
    }

    uint32 iNodeParticleSystem::getVortexParticleCount()
    {
        return _particleSystem.getVortexParticleCount();
    }

    void iNodeParticleSystem::setVortexTorque(float32 min, float32 max)
    {
        _particleSystem.setVortexTorque(min, max);
    }

    float32 iNodeParticleSystem::getVortexTorqueMin()
    {
        return _particleSystem.getVortexTorqueMin();
    }

    float32 iNodeParticleSystem::getVortexTorqueMax()
    {
        return _particleSystem.getVortexTorqueMax();
    }

    void iNodeParticleSystem::setVortexRange(float32 min, float32 max)
    {
        _particleSystem.setVortexRange(min, max);
    }

    float32 iNodeParticleSystem::getVortexRangeMin()
    {
        return _particleSystem.getVortexRangeMin();
    }

    float32 iNodeParticleSystem::getVortexRangeMax()
    {
        return _particleSystem.getVortexRangeMax();
    }

    void iNodeParticleSystem::setLift(float32 min, float32 max)
    {
        _particleSystem.setLift(min, max);
    }

    float32 iNodeParticleSystem::getLiftMin()
    {
        return _particleSystem.getLiftMin();
    }

    float32 iNodeParticleSystem::getLiftMax()
    {
        return _particleSystem.getLiftMax();
    }

    void iNodeParticleSystem::setLiftDecrease(float32 decrease)
    {
        _particleSystem.setLiftDecrease(decrease);
    }

    float32 iNodeParticleSystem::getLiftDecrease()
    {
        return _particleSystem.getLiftDecrease();
    }

    void iNodeParticleSystem::setWeight(float32 min, float32 max)
    {
        _particleSystem.setWeight(min, max);
    }

    float32 iNodeParticleSystem::getWeightMin()
    {
        return _particleSystem.getWeightMin();
    }

    float32 iNodeParticleSystem::getWeightMax()
    {
        return _particleSystem.getWeightMax();
    }

    void iNodeParticleSystem::setSize(float32 min, float32 max)
    {
        _particleSystem.setSize(min, max);
    }

    float32 iNodeParticleSystem::getSizeMin()
    {
        return _particleSystem.getSizeMin();
    }

    float32 iNodeParticleSystem::getSizeMax()
    {
        return _particleSystem.getSizeMax();
    }

    void iNodeParticleSystem::setSizeIncrease(float32 increment)
    {
        _particleSystem.setSizeIncrease(increment);
    }

    float32 iNodeParticleSystem::getSizeIncrease()
    {
        return _particleSystem.getSizeIncrease();
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
