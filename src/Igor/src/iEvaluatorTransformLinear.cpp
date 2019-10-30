#include <iEvaluatorTransformLinear.h>

#include <iNodeManager.h>
#include <iTimer.h>
#include <iNodeTransform.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    void iEvaluatorTransformLinear::evaluate()
    {
        float64 time = iTimer::getInstance().getSeconds();

        auto &nodeManager = iNodeManager::getInstance();

        std::vector<uint64> removeEntries;

        for (auto pair : _transforms)
        {
            iNodeTransformPtr transformNode = static_cast<iNodeTransformPtr>(nodeManager.getNode(pair.first));
            Transform& transform = pair.second;
            if (transformNode != nullptr)
            {
                if (time < transform._startTime)
                {
                    continue
                }

                if (time < transform._startTime)
                {
                    continue
                }
            }
            else
            {
                removeEntries.push_back(pair.first);
            }
        }

        for (auto key : removeEntries)
        {
            _transforms.
        }
    }

    void iEvaluatorTransformLinear::setTransform(const iNodePtr node, float64 startTime, float64 stopTime, const iaMatrixd& matrix)
    {
        con_assert(node != nullptr, "zero pointer");

        if (node == nullptr)
        {
            return;
        }

        auto iter = _transforms.find(node->getID());
        if (iter == _transforms.end())
        {
            return;
        }

        Transform& transform = _transforms[node->getID()];

        transform._startTime = startTime;
        transform._stopTime = stopTime;
        transform._matrix = matrix;
    }

    void iEvaluatorTransformLinear::addNode(const iNodePtr node)
    {
        con_assert(node != nullptr, "zero pointer");

        if (node == nullptr)
        {
            return;
        }

        if (_transforms.contains(node->getID()))
        {
            return;
        }

        if (node->getKind() != iNodeKind::Transformation)
        {
            return;
        }
    
        _transforms[node->getID()] = Transform();
    }

    void iEvaluatorTransformLinear::removeNode(const iNodePtr node)
    {
        con_assert(node != nullptr, "zero pointer");

        if (node == nullptr)
        {
            return;
        }

        auto iter = _transforms.find(node->getID());
        if(iter == _transforms.end())
        {
            return;
        }

        _transforms.erase(iter);

    }

}