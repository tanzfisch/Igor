// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iEvaluationTransformLinear.h>

#include <iNodeManager.h>
#include <iTimer.h>
#include <iNodeTransform.h>

#include <iaConsole.h>
using namespace IgorAux;

#include<algorithm>

namespace Igor
{

    iEvaluationTransformLinear::iEvaluationTransformLinear(uint64 nodeID)
        :iEvaluation(nodeID)
    {
    }

    void iEvaluationTransformLinear::setTarget(const iaMatrixd& source, const iaMatrixd& destination, float64 startTime, float64 duration)
    {
        con_assert(startTime < startTime + duration, "invalid start stop values");

        float64 currentTime = iTimer::getInstance().getSeconds();
        setStart(startTime);
        setStop(startTime + duration);

        _source.setMatrix(source);
        _destination.setMatrix(destination);
    }

    void iEvaluationTransformLinear::evaluate(float64 time)
    {
        if (_start > time)
        {
            return;
        }

        iNodeTransformPtr transformNode = static_cast<iNodeTransformPtr>(iNodeManager::getInstance().getNode(_nodeID));
        if (transformNode == nullptr)
        {
            return;
        }
        
        const float64 delta = _stop - _start;
        const float64 t = std::min(1.0, std::max(0.0, (time - _start) / delta));
        const iaTransformd transform = lerp(_source, _destination, t);

        iaMatrixd nodeMatrix;
        transform.getMatrix(nodeMatrix);
        transformNode->setMatrix(nodeMatrix);
    }



}