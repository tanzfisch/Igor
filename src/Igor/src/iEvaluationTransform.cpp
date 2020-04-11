// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iEvaluationTransform.h>

#include <iNodeManager.h>
#include <iTimer.h>
#include <iNodeTransform.h>

#include <iaConsole.h>
using namespace IgorAux;


namespace Igor
{

    iEvaluationTransform::iEvaluationTransform(uint64 nodeID)
        :iEvaluation(nodeID)
    {
        iNodeTransformPtr transformNode = static_cast<iNodeTransformPtr>(iNodeManager::getInstance().getNode(_nodeID));
        con_assert(transformNode != nullptr && transformNode->getKind() == iNodeKind::Transformation, "invalid node");
    }

    void iEvaluationTransform::setSource(const iaMatrixd& source)
    {
        _source.setMatrix(source);
    }

    void iEvaluationTransform::setDestination(const iaMatrixd& destination)
    {
        _destination.setMatrix(destination);
    }

    void iEvaluationTransform::evaluate(float64 t)
    {
        iNodeTransformPtr transformNode = static_cast<iNodeTransformPtr>(iNodeManager::getInstance().getNode(_nodeID));
        if (transformNode == nullptr)
        {
            return;
        }

        const iaTransformd transform = lerp(_source, _destination, t);

        iaMatrixd nodeMatrix;
        transform.getMatrix(nodeMatrix);
        transformNode->setMatrix(nodeMatrix);
    }



}