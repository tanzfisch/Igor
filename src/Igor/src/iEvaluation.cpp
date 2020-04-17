// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iEvaluation.h>
#include <iNodeTransform.h>
#include <iNodeManager.h>
#include <iEvaluationManager.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
    iaIDGenerator64 iEvaluation::_idGenerator;

    iEvaluation::iEvaluation(uint64 nodeID)
    {
        _nodeID = nodeID;
        _evaluatorID = iEvaluation::_idGenerator.createID();

        reset();
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

    void iEvaluation::setStart(float64 start)
    {
        _start = start;
        iEvaluationManager::getInstance().setEvaluationDirty(_evaluatorID);
    }

    float64 iEvaluation::getStart() const
    {
        return _start;
    }

    void iEvaluation::setStop(float64 stop)
    {
        _stop = stop;
    }

    float64 iEvaluation::getStop() const
    {
        return _stop;
    }

    void iEvaluation::setDuration(float64 duration)
    {
        _stop = _start + duration;
    }

    float64 iEvaluation::getDuration() const
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

};

