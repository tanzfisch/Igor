// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iEvaluationTransform.h>

#include <iNodeManager.h>
#include <iTimer.h>
#include <iNodeTransform.h>

#include <iaConsole.h>
using namespace IgorAux;

#include<algorithm>

namespace Igor
{

    iEvaluationTransform::iEvaluationTransform(uint64 nodeID)
        :iEvaluation(nodeID)
    {
    }

    void iEvaluationTransform::setSource(const iaMatrixd& source)
    {
        _source.setMatrix(source);
    }

    void iEvaluationTransform::setDestination(const iaMatrixd& destination)
    {
        _destination.setMatrix(destination);
    }

    void iEvaluationTransform::evaluate(float64 time)
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