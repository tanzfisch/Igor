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
        float64 currentTime = iTimer::getInstance().getSeconds();
        setStart(currentTime);
        setStop(currentTime + duration);

        _targetTransform.setMatrix(matrix);
    }

    void iEvaluationTransformLinear::setTarget(const iaMatrixd& matrix, float64 startTime, float64 duration)
    {
        float64 currentTime = iTimer::getInstance().getSeconds();
        setStart(startTime);
        setStop(startTime + duration);

        _targetTransform.setMatrix(matrix);
    }

    void iEvaluationTransformLinear::evaluate(float64 time)
    {
        con_assert(getStart() < getStop(), "invalid start stop values");

        if (getStart() > time)
        {
            return;
        }

        iNodeTransformPtr transformNode = static_cast<iNodeTransformPtr>(iNodeManager::getInstance().getNode(_nodeID));

        if (transformNode == nullptr)
        {
            return;
        }

        float64 delta = getStop() - getStart();
        float64 t = (time - getStart()) / delta;

        t = std::min(1.0, std::max(0.0, t));

        iaMatrixd nodeMatrix;
        transformNode->getMatrix(nodeMatrix);
        iaTransformd transform(nodeMatrix);

        transform = lerp(transform, _targetTransform, t);

        transform.getMatrix(nodeMatrix);

        transformNode->setMatrix(nodeMatrix);
    }



}