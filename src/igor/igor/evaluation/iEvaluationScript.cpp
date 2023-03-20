// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/evaluation/iEvaluationScript.h>

#include <igor/scene/nodes/iNodeManager.h>
#include <igor/system/iTimer.h>
#include <igor/scene/nodes/iNodeTransform.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iEvaluationScript::iEvaluationScript(uint64 nodeID)
        : iEvaluation(nodeID)
    {
        iNodeTransformPtr transformNode = static_cast<iNodeTransformPtr>(iNodeManager::getInstance().getNode(_nodeID));
        con_assert(transformNode != nullptr && transformNode->getKind() == iNodeKind::Transformation, "invalid node");
    }

    void iEvaluationScript::setScript(iEvaluationScriptDelegate evaluationScriptDelegate)
    {
        _evaluationScriptDelegate = evaluationScriptDelegate;
    }

    void iEvaluationScript::evaluate(float64 t)
    {
        iNodeTransformPtr transformNode = static_cast<iNodeTransformPtr>(iNodeManager::getInstance().getNode(_nodeID));
        if (transformNode == nullptr)
        {
            return;
        }

        _evaluationScriptDelegate(transformNode, t);
    }
} // namespace igor