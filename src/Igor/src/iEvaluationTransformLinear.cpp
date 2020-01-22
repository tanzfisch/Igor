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

    void iEvaluationTransformLinear::setTarget(const iaMatrixd& matrix, float64 duration)
    {
        con_assert(duration > 0, "invalid start stop values");

        float64 currentTime = iTimer::getInstance().getSeconds();
        setStart(currentTime);
        setStop(currentTime + duration);

        iNodeTransformPtr transformNode = static_cast<iNodeTransformPtr>(iNodeManager::getInstance().getNode(_nodeID));
        if (transformNode != nullptr)
        {
            iaMatrixd nodeMatrix;
            transformNode->getMatrix(nodeMatrix);
            _sourceTransform.setMatrix(nodeMatrix);
            _sourceIsSet = true;
        }

        _targetTransform.setMatrix(matrix);
    }

    void iEvaluationTransformLinear::setTarget(const iaMatrixd& matrix, float64 startTime, float64 duration)
    {
        con_assert(startTime < startTime + duration, "invalid start stop values");

        float64 currentTime = iTimer::getInstance().getSeconds();
        setStart(startTime);
        setStop(startTime + duration);

        _targetTransform.setMatrix(matrix);
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

        iaMatrixd nodeMatrix;

        if (!_sourceIsSet)
        {
            transformNode->getMatrix(nodeMatrix);
            _sourceTransform.setMatrix(nodeMatrix);
            _sourceIsSet = true;
        }

        float64 delta = _stop - _start;
        float64 t = (time - _start) / delta;
        t = std::min(1.0, std::max(0.0, t));

        iaTransformd transform = lerp(_sourceTransform, _targetTransform, t);

        transform.getMatrix(nodeMatrix);
        transformNode->setMatrix(nodeMatrix);
    }



}