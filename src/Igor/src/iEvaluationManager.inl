// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file


__IGOR_INLINE__ iEvaluationPtr iEvaluationManager::getEvaluation(uint64 id) const
{
    iEvaluationPtr result = nullptr;

    auto iter = _evaluations.find(id);
    if (_evaluations.end() != iter)
    {
        result = (*iter).second;
    }

    return result;
}

__IGOR_INLINE__ bool iEvaluationManager::isEvaluation(uint64 id) const
{
    bool result = false;;

    auto iter = _evaluations.find(id);
    if (_evaluations.end() != iter)
    {
        result = true;
    }

    return result;
}

template<class T>
T* iEvaluationManager::createEvaluation(uint64 nodeID)
{
    iNodePtr node = iNodeManager::getInstance().getNode(nodeID);

    if (node == nullptr)
    {
        con_err("invalid node id " << nodeID);
        return nullptr;
    }

    if (node->getKind() != iNodeKind::Transformation)
    {
        con_err("invalid node kind " << node->getKind());
        return nullptr;
    }

	T* result = new T(nodeID);
	_evaluations[result->getID()] = result;

	return result;
}