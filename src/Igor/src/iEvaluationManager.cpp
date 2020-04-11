// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iEvaluationManager.h>
#include <iTimer.h>

#include <iaConsole.h>
using namespace IgorAux;

#include<algorithm>

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
        for (auto pair : _evaluations)
        {
            iEvaluationPtr evaluation = pair.second;

            if (evaluation->_start > time)
            {
                continue;
            }

            const float64 delta = evaluation->_stop - evaluation->_start;
            float64 t = std::min(1.0, std::max(0.0, (time - evaluation->_start) / delta));

            switch (evaluation->getInterpolationFunction())
            {
            case InterpolationFunction::Accelerate:
                t = t * t;
                break;

            case InterpolationFunction::Decelerate:
                t = 1.0 - (1.0 - t) * (1.0 - t);
                break;

            case InterpolationFunction::SmoothStep:
                t = (t * t) * (3.0 - (2.0 * t));
                break;
            }

            evaluation->evaluate(t);
        }
    }

}