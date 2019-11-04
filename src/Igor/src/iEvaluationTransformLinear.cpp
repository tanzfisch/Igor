// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iEvaluationTransformLinear.h>

#include <iNodeManager.h>
#include <iTimer.h>
#include <iNodeTransform.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iEvaluationTransformLinear::iEvaluationTransformLinear(uint64 nodeID)
        :iEvaluation(nodeID)
    {

    }

    void iEvaluationTransformLinear::setTarget(const iaMatrixd& matrix, float64 time)
    {
        float64 currentTime = iTimer::getInstance().getSeconds();
        setStart(currentTime);
        setStop(currentTime + time);

        _targetTransform.setMatrix(matrix);
    }

    void iEvaluationTransformLinear::evaluate(float64 time)
    {
        iNodeTransformPtr transformNode = static_cast<iNodeTransformPtr>(iNodeManager::getInstance().getNode(_nodeID));

        if (transformNode == nullptr)
        {
            return;
        }

        float64 timeFrame = getStop() - getStart();
        float64 t = (time - getStart()) / timeFrame;

        iaMatrixd nodeMatrix;
        transformNode->getMatrix(nodeMatrix);
        iaTransformd transform(nodeMatrix);

        transform = lerp(transform, _targetTransform, t);

        transform.getMatrix(nodeMatrix);

        transformNode->setMatrix(nodeMatrix);
    }



}