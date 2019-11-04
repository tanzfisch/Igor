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

    void iEvaluationTransformLinear::evaluate(float64 time)
    {
        float64 delta = iTimer::getInstance().getSecondsDelta();

        // TODO

        iNodeTransformPtr transformNode = static_cast<iNodeTransformPtr>(iNodeManager::getInstance().getNode(_nodeID));

        if (transformNode == nullptr)
        {
            return;
        }

        transformNode->rotate(0.1 * delta, iaAxis::Y);
    }



}