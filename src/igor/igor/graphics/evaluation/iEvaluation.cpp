// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/graphics/evaluation/iEvaluation.h>
#include <igor/graphics/scene/nodes/iNodeTransform.h>
#include <igor/graphics/scene/nodes/iNodeManager.h>
#include <igor/graphics/evaluation/iEvaluationManager.h>

#include <iaux/system/iaConsole.h>
using namespace IgorAux;

namespace Igor
{
    iaIDGenerator64 iEvaluation::_idGenerator;

    iEvaluation::iEvaluation(uint64 nodeID)
    {
        _nodeID = nodeID;
        _evaluatorID = iEvaluation::_idGenerator.createID();
    }

    void iEvaluation::reset()
    {
        _easingFunction = Easing::EasingFunction::Linear;
        _start = 0;
        _stop = 0;
        _amplitude = 0.5;
        _period = 0.25;
        _overshoot = 1.70158;
        _looped = false;
        iEvaluationManager::getInstance().setEvaluationDirty(_evaluatorID);
    }

    uint64 iEvaluation::getID() const
    {
        return _evaluatorID;
    }

    void iEvaluation::setLooped(bool loop)
    {
        _looped = loop;
    }

    bool iEvaluation::isLooped() const
    {
        return _looped;
    }

    void iEvaluation::setStart(const iaTime &start)
    {
        _start = start;
        iEvaluationManager::getInstance().setEvaluationDirty(_evaluatorID);
    }

    const iaTime &iEvaluation::getStart() const
    {
        return _start;
    }

    void iEvaluation::setStop(const iaTime &stop)
    {
        _stop = stop;
    }

    const iaTime &iEvaluation::getStop() const
    {
        return _stop;
    }

    void iEvaluation::setDuration(const iaTime &duration)
    {
        _stop = _start + duration;
    }

    const iaTime &iEvaluation::getDuration() const
    {
        return _stop - _start;
    }

    void iEvaluation::setEasingFunction(Easing::EasingFunction easingFunction)
    {
        _easingFunction = easingFunction;
    }

    Easing::EasingFunction iEvaluation::getEasingFunction() const
    {
        return _easingFunction;
    }

    void iEvaluation::setAmplitude(float64 amplitude)
    {
        _amplitude = amplitude;
    }

    float64 iEvaluation::getAmplitude() const
    {
        return _amplitude;
    }

    void iEvaluation::setPeriod(float64 period)
    {
        _period = period;
    }

    float64 iEvaluation::getPeriod() const
    {
        return _period;
    }

    void iEvaluation::setOvershoot(float64 overshoot)
    {
        _overshoot = overshoot;
    }

    float64 iEvaluation::getOvershoot() const
    {
        return _overshoot;
    }

}; // namespace Igor
