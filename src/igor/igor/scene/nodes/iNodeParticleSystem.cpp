// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeParticleSystem.h>

#include <igor/renderer/iRenderer.h>
#include <igor/resources/iResourceManager.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/nodes/iNodeEmitter.h>
#include <igor/system/iApplication.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iNodeParticleSystem::iNodeParticleSystem()
    {
        setName(L"iNodeParticleSystem");
        _nodeType = iNodeType::iNodeParticleSystem;
        _nodeKind = iNodeKind::Volume;
    }

    iNodeParticleSystem::iNodeParticleSystem(iNodeParticleSystem *node)
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = node->_nodeType;
        _nodeKind = node->_nodeKind;

        setName(node->getName());

        _particleSystem = node->_particleSystem;

        setMaterial(node->getMaterial());
        setEmitter(node->getEmitter());
    }

    iNodeParticleSystem::~iNodeParticleSystem()
    {
        setScene(nullptr);
    }

    void iNodeParticleSystem::registerParticleSystemFinishedDelegate(iParticleSystemFinishedDelegate delegate)
    {
        _fishedEvent.add(delegate);
    }

    void iNodeParticleSystem::unregisterParticleSystemFinishedDelegate(iParticleSystemFinishedDelegate delegate)
    {
        _fishedEvent.remove(delegate);
    }

    uint32 iNodeParticleSystem::getParticleCount()
    {
        return _particleSystem.getMaxParticleCount();
    }

    void iNodeParticleSystem::onPostCopyLink(std::map<uint32, uint32> &nodeIDMap)
    {
        uint32 oldEmitterID = getEmitter();

        auto iter = nodeIDMap.find(oldEmitterID);
        if (iter != nodeIDMap.end())
        {
            setEmitter((*iter).second);
        }
        else
        {
            setEmitter(iNode::INVALID_NODE_ID);
        }
    }

    void iNodeParticleSystem::setColorGradient(const iaKeyFrameGraphColor4f &colorGradient)
    {
        _particleSystem.setColorGradient(colorGradient);
    }

    void iNodeParticleSystem::getColorGradient(iaKeyFrameGraphColor4f &colorGradient) const
    {
        _particleSystem.getColorGradient(colorGradient);
    }

    void iNodeParticleSystem::handle()
    {
        iNodeEmitter *emitter = static_cast<iNodeEmitter *>(iNodeManager::getInstance().getNode(_emitterID));
        if (emitter != nullptr)
        {
            _particleSystem.onUpdate(emitter->getParticleEmitter());
            setBoundingBox(_particleSystem.getBoundingBox());
        }

        if (_particleSystem.isFinished())
        {
            _fishedEvent(getID());
        }
    }

    void iNodeParticleSystem::draw()
    {
        handle();

        if(_particleSystem.getVertexArray() == nullptr)
        {
            return;
        }

        iRenderer::getInstance().setModelMatrix(_worldMatrix);
        iRenderer::getInstance().drawBuffer(_particleSystem.getVertexArray(), iRenderPrimitive::Points, getMaterial());
    }

    void iNodeParticleSystem::setStartVelocityGradient(const iaKeyFrameGraphVector2f &velocityGradient)
    {
        _particleSystem.setStartVelocityGradient(velocityGradient);
    }

    void iNodeParticleSystem::getStartVelocityGradient(iaKeyFrameGraphVector2f &velocityGradient) const
    {
        _particleSystem.getStartVelocityGradient(velocityGradient);
    }

    void iNodeParticleSystem::setVortexToParticleRate(float32 rate)
    {
        con_assert(rate >= 0.0 && rate <= 1.0, "out of range");

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

    bool iNodeParticleSystem::isLooped() const
    {
        return _particleSystem.isLooped();
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

    void iNodeParticleSystem::setStartLiftGradient(const iaKeyFrameGraphVector2f &liftGradient)
    {
        _particleSystem.setStartLiftGradient(liftGradient);
    }

    void iNodeParticleSystem::getStartLiftGradient(iaKeyFrameGraphVector2f &liftGradient) const
    {
        _particleSystem.getStartLiftGradient(liftGradient);
    }

    void iNodeParticleSystem::setSizeScaleGradient(const iaKeyFrameGraphf &sizeScaleGradient)
    {
        _particleSystem.setSizeScaleGradient(sizeScaleGradient);
    }

    void iNodeParticleSystem::getSizeScaleGradient(iaKeyFrameGraphf &sizeScaleGradient) const
    {
        _particleSystem.getSizeScaleGradient(sizeScaleGradient);
    }

    void iNodeParticleSystem::setStartSizeGradient(const iaKeyFrameGraphVector2f &sizeGradient)
    {
        _particleSystem.setStartSizeGradient(sizeGradient);
    }

    void iNodeParticleSystem::getStartSizeGradient(iaKeyFrameGraphVector2f &sizeGradient) const
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

    void iNodeParticleSystem::onUpdateTransform(iaMatrixd &matrix)
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

    void iNodeParticleSystem::setVortexCheckRange(uint8 particles)
    {
        _particleSystem.setVortexCheckRange(particles);
    }

    uint8 iNodeParticleSystem::getVortexCheckRange()
    {
        return _particleSystem.getVortexCheckRange();
    }

    void iNodeParticleSystem::setMaxParticleCount(uint16 max)
    {
        _particleSystem.setMaxParticleCount(max);
    }

    uint16 iNodeParticleSystem::getMaxParticleCount() const
    {
        return _particleSystem.getMaxParticleCount();
    }

    void iNodeParticleSystem::setEmissionGradient(const iaKeyFrameGraphf &emissionGradient)
    {
        _particleSystem.setEmissionGradient(emissionGradient);
    }

    void iNodeParticleSystem::getEmissionGradient(iaKeyFrameGraphf &emissionGradient) const
    {
        _particleSystem.getEmissionGradient(emissionGradient);
    }

    void iNodeParticleSystem::setStartAgeGradient(const iaKeyFrameGraphVector2f &visibleGradient)
    {
        _particleSystem.setStartAgeGradient(visibleGradient);
    }

    void iNodeParticleSystem::getStartAgeGradient(iaKeyFrameGraphVector2f &visibleGradient) const
    {
        _particleSystem.getStartAgeGradient(visibleGradient);
    }

    void iNodeParticleSystem::setStartOrientationGradient(const iaKeyFrameGraphVector2f &orientationGradient)
    {
        _particleSystem.setStartOrientationGradient(orientationGradient);
    }

    void iNodeParticleSystem::getStartOrientationGradient(iaKeyFrameGraphVector2f &orientationGradient) const
    {
        _particleSystem.getStartOrientationGradient(orientationGradient);
    }

    void iNodeParticleSystem::setStartOrientationRateGradient(const iaKeyFrameGraphVector2f &orientationRateGradient)
    {
        _particleSystem.setStartOrientationRateGradient(orientationRateGradient);
    }

    void iNodeParticleSystem::getStartOrientationRateGradient(iaKeyFrameGraphVector2f &orientationRateGradient) const
    {
        _particleSystem.getStartOrientationRateGradient(orientationRateGradient);
    }

    void iNodeParticleSystem::setVelocityOriented(bool velocityOriented)
    {
        _particleSystem.setVelocityOriented(velocityOriented);
        getMaterial()->setVelocityOriented(velocityOriented);
    }

    bool iNodeParticleSystem::getVelocityOriented() const
    {
        return _particleSystem.getVelocityOriented();
    }

    void iNodeParticleSystem::setTextureTiling(uint32 columns, uint32 rows)
    {
        _particleSystem.setTextureTiling(columns, rows);
        getMaterial()->setTiling(iaVector2f(static_cast<float32>(columns), static_cast<float32>(rows)));
    }

    uint32 iNodeParticleSystem::getTextureColumns() const
    {
        return _particleSystem.getTextureColumns();
    }

    uint32 iNodeParticleSystem::getTextureRows() const
    {
        return _particleSystem.getTextureRows();
    }

    void iNodeParticleSystem::setTileIncrement(float32 tileIncrement)
    {
        _particleSystem.setTileIncrement(tileIncrement);
    }

    float32 iNodeParticleSystem::getTileIncrement() const
    {
        return _particleSystem.getTileIncrement();
    }

} // namespace igor