// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iEvaluationManager.h>
#include <iTimer.h>

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
        float64 time = iTimer::getInstance().getSeconds();
        for (auto evaluation : _evaluations)
        {
            evaluation.second->evaluate(time);
        }
    }

}