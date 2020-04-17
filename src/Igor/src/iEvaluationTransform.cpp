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

    void iEvaluationTransform::addKeyframe(const iaTransformd& transform)
    {
        _keyframes.push_back(transform);
    }

    void iEvaluationTransform::addKeyframe(const iaMatrixd& matrix)
    {
        _keyframes.push_back(matrix);
    }

    void iEvaluationTransform::evaluate(float64 t)
    {
        con_assert(_keyframes.size() >= 2, "not enough keyframes");
        if (_keyframes.size() < 2)
        {
            return;
        }

        iNodeTransformPtr transformNode = static_cast<iNodeTransformPtr>(iNodeManager::getInstance().getNode(_nodeID));
        if (transformNode == nullptr)
        {
            return;
        }

        iaTransformd transform;

        const float64 frameLength = 1.0 / (_keyframes.size() - 1);
        int64 frame = t / frameLength;
        float64 newT = (t / frameLength) - float64(frame);

        if (frame == _keyframes.size() - 1)
        {
            // allowing to overshoot to Elastic and Back are working too
            transform = lerp(_keyframes[frame - 1], _keyframes[frame], newT + 1.0);
        }
        else
        {
            transform = lerp(_keyframes[frame], _keyframes[frame + 1], newT);
        }

        iaMatrixd nodeMatrix;
        transform.getMatrix(nodeMatrix);
        transformNode->setMatrix(nodeMatrix);
    }

    void iEvaluationTransform::reset()
    {
        iEvaluation::reset();
    }

}