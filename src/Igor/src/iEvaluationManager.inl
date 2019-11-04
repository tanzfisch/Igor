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
	T* result = new T(nodeID);

	_evaluations[static_cast<iEvaluationPtr>(result)->getID()] = static_cast<iEvaluationPtr>(result);

	return result;
}