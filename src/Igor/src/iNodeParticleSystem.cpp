// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeParticleSystem.h>

#include <iRenderer.h>
#include <iTextureResourceFactory.h>
#include <iNodeFactory.h>
#include <iNodeEmitter.h>
#include <iApplication.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iNodeParticleSystem::iNodeParticleSystem()
	{
        setName(L"iNodeParticleSystem");
        _nodeType = iNodeType::iNodeParticleSystem;
        _nodeKind = iNodeKind::Volume;

		iApplication::getInstance().registerApplicationHandleDelegate(iApplicationHandleDelegate(this, &iNodeParticleSystem::handle));
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

		iApplication::getInstance().registerApplicationHandleDelegate(iApplicationHandleDelegate(this, &iNodeParticleSystem::handle));
    }

	iNodeParticleSystem::~iNodeParticleSystem()
	{
        setScene(nullptr);
		iApplication::getInstance().unregisterApplicationHandleDelegate(iApplicationHandleDelegate(this, &iNodeParticleSystem::handle));
	}

    uint32 iNodeParticleSystem::getParticleCount()
    {
        return _particleSystem.getParticleCount();
    }

    void iNodeParticleSystem::setColorGradient(const iGradientColor4f& colorGradient)
    {
        _particleSystem.setColorGradient(colorGradient);
    }

    void iNodeParticleSystem::getColorGradient(iGradientColor4f& colorGradient) const
    {
        _particleSystem.getColorGradient(colorGradient);
    }

	void iNodeParticleSystem::handle()
	{
		iNodeEmitter* emitter = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().getNode(_emitterID));
		if (emitter != nullptr)
		{
			_particleSystem.calcNextFrame(emitter->getParticleEmitter());
			setBoundingSphere(_particleSystem.getBoundingSphere());
		}
	}

    void iNodeParticleSystem::draw()
    {
        iRenderer::getInstance().bindTexture(_textureA, 0);
        iRenderer::getInstance().bindTexture(_textureB, 1);
        iRenderer::getInstance().bindTexture(_textureC, 2);

        iRenderer::getInstance().setModelMatrix(_worldMatrix);
        if (_particleSystem.getVelocityOriented())
        {
            iRenderer::getInstance().drawVelocityOrientedParticles(_particleSystem.getCurrentFrame(), _particleSystem.getColorGradient());
        }
        else
        {
            iRenderer::getInstance().drawParticles(_particleSystem.getCurrentFrame(), _particleSystem.getColorGradient());
        }
    }

    void iNodeParticleSystem::setStartVelocityGradient(const iGradientVector2f& velocityGradient)
    {
        _particleSystem.setStartVelocityGradient(velocityGradient);
    }

    void iNodeParticleSystem::getStartVelocityGradient(iGradientVector2f& velocityGradient) const
    {
        _particleSystem.getStartVelocityGradient(velocityGradient);
    }

    void iNodeParticleSystem::setVortexToParticleRate(float32 rate)
    {
        _particleSystem.setVortexToParticleRate(rate);
    }

    float32 iNodeParticleSystem::getVortexToParticleRate() const
    {
        return _particleSystem.getVortexToParticleRate();
    }

    void iNodeParticleSystem::reset()
    {
        _particleSystem.reset();
    }

    void iNodeParticleSystem::start()
    {
        _particleSystem.start();
    }

    void iNodeParticleSystem::stop()
    {
        _particleSystem.stop();
    }

	bool iNodeParticleSystem::isFinished() const
	{
		return _particleSystem.isFinished();
	}

    void iNodeParticleSystem::setLoop(bool loop)
    {
        _particleSystem.setLoop(loop);
    }

    bool iNodeParticleSystem::getLoop() const
    {
        return _particleSystem.getLoop();
    }

    void iNodeParticleSystem::setPeriodTime(float32 periodTime)
    {
        _particleSystem.setPeriodTime(periodTime);
    }

    float32 iNodeParticleSystem::getPeriodTime() const
    {
        return _particleSystem.getPeriodTime();
    }

    bool iNodeParticleSystem::isRunning()
    {
        return _particleSystem.isRunning();
    }

    void iNodeParticleSystem::setVorticityConfinement(float32 vorticityConfinement)
    {
        _particleSystem.setVorticityConfinement(vorticityConfinement);
    }

    float32 iNodeParticleSystem::getVorticityConfinement()
    {
        return _particleSystem.getVorticityConfinement();
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

    void iNodeParticleSystem::setStartLiftGradient(const iGradientVector2f& liftGradient)
    {
        _particleSystem.setStartLiftGradient(liftGradient);
    }
    
    void iNodeParticleSystem::getStartLiftGradient(iGradientVector2f& liftGradient) const
    {
        _particleSystem.getStartLiftGradient(liftGradient);
    }
    
    void iNodeParticleSystem::setSizeScaleGradient(const iGradientf& sizeScaleGradient)
    {
        _particleSystem.setSizeScaleGradient(sizeScaleGradient);
    }

    void iNodeParticleSystem::getSizeScaleGradient(iGradientf& sizeScaleGradient) const
    {
        _particleSystem.getSizeScaleGradient(sizeScaleGradient);
    }

    void iNodeParticleSystem::setStartSizeGradient(const iGradientVector2f& sizeGradient)
    {
        _particleSystem.setStartSizeGradient(sizeGradient);
    }

    void iNodeParticleSystem::getStartSizeGradient(iGradientVector2f& sizeGradient) const
    {
        _particleSystem.getStartSizeGradient(sizeGradient);
    }

    void iNodeParticleSystem::setAirDrag(float32 airDrag)
    {
        _particleSystem.setAirDrag(airDrag);
    }

    float32 iNodeParticleSystem::getAirDrag() const
    {
        return _particleSystem.getAirDrag();
    }

    void iNodeParticleSystem::onUpdateTransform(iaMatrixf& matrix)
    {
		if (_worldMatrix != matrix)
		{
			_worldMatrix = matrix;
			_worldMatrixInv = matrix;
			_worldMatrixInv.invert();
			_particleSystem.setParticleSystemMatrix(_worldMatrixInv);
		}

		updateTree();
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

    void iNodeParticleSystem::setVortexCheckRange(uint32 particles)
    {
        _particleSystem.setVortexCheckRange(particles);
    }

    uint32 iNodeParticleSystem::getVortexCheckRange()
    {
        return _particleSystem.getVortexCheckRange();
    }

    void iNodeParticleSystem::setMaxParticleCount(uint32 max)
    {
        _particleSystem.setMaxParticleCount(max);
    }

    uint32 iNodeParticleSystem::getMaxParticleCount() const
    {
        return _particleSystem.getMaxParticleCount();
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

    void iNodeParticleSystem::setEmissionGradient(const iGradientf& emissionGradient)
    {
        _particleSystem.setEmissionGradient(emissionGradient);
    }

    void iNodeParticleSystem::getEmissionGradient(iGradientf& emissionGradient) const
    {
        _particleSystem.getEmissionGradient(emissionGradient);
    }

    void iNodeParticleSystem::setStartVisibleTimeGradient(const iGradientVector2f& visibleGradient)
    {
        _particleSystem.setStartVisibleTimeGradient(visibleGradient);
    }

    void iNodeParticleSystem::getStartVisibleTimeGradient(iGradientVector2f& visibleGradient) const
    {
        _particleSystem.getStartVisibleTimeGradient(visibleGradient);
    }

    void iNodeParticleSystem::setStartOrientationGradient(const iGradientVector2f& orientationGradient)
    {
        _particleSystem.setStartOrientationGradient(orientationGradient);
    }

    void iNodeParticleSystem::getStartOrientationGradient(iGradientVector2f& orientationGradient) const
    {
        _particleSystem.getStartOrientationGradient(orientationGradient);
    }

    void iNodeParticleSystem::setStartOrientationRateGradient(const iGradientVector2f& orientationRateGradient)
    {
        _particleSystem.setStartOrientationRateGradient(orientationRateGradient);
    }

    void iNodeParticleSystem::getStartOrientationRateGradient(iGradientVector2f& orientationRateGradient) const
    {
        _particleSystem.getStartOrientationRateGradient(orientationRateGradient);
    }

    void iNodeParticleSystem::setVelocityOriented(bool velocityOriented)
    {
        _particleSystem.setVelocityOriented(velocityOriented);
    }

    bool iNodeParticleSystem::getVelocityOriented() const
    {
        return _particleSystem.getVelocityOriented();
    }

    void iNodeParticleSystem::setFirstTextureTiling(uint32 columns, uint32 rows)
    {
        _particleSystem.setFirstTextureTiling(columns, rows);
    }

    uint32 iNodeParticleSystem::getFirstTextureColumns() const
    {
        return _particleSystem.getFirstTextureColumns();
    }

    uint32 iNodeParticleSystem::getFirstTextureRows() const
    {
        return _particleSystem.getFirstTextureRows();
    }
}