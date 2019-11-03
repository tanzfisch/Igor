// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iEvaluationManager.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
    iEvaluationManager::~iEvaluationManager()
    {
        for (auto evaluation : _evaluations)
        {
            delete evaluation.second;
        }

        _evaluations.clear();
    }

    void iEvaluationManager::handle()
    {
        for (auto evaluation : _evaluations)
        {
            evaluation.second->evaluate();
        }
    }

}