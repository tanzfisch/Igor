// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iEvaluationManager.h>
#include <iTimer.h>

#include <iaEasing.h>
#include <iaConsole.h>
using namespace IgorAux;

#include<algorithm>

namespace Igor
{
    iEvaluationManager::~iEvaluationManager()
    {
        _evaluations.clear();
    }

    void iEvaluationManager::setEvaluationDirty(uint64 id)
    {
        auto iterActive = std::find(_activeEvaluations.begin(), _activeEvaluations.end(), id);
        if (iterActive != _activeEvaluations.end())
        {
            _activeEvaluations.erase(iterActive);
        }

        auto iter = std::find(_dirtyEvaluations.begin(), _dirtyEvaluations.end(), id);
        if (iter == _dirtyEvaluations.end())
        {
            _dirtyEvaluations.push_back(id);
        }

        std::sort(_dirtyEvaluations.begin(), _dirtyEvaluations.end(), [](const uint64 a, const uint64 b) {
            auto evaluationA = iEvaluationManager::getInstance().getEvaluation(a);
            auto evaluationB = iEvaluationManager::getInstance().getEvaluation(b);
            return evaluationA->getStart() < evaluationB->getStart();
            });
    }

    void iEvaluationManager::destroyEvaluation(uint64 id)
    {
        auto iterDirty = std::find(_dirtyEvaluations.begin(), _dirtyEvaluations.end(), id);
        if (iterDirty != _dirtyEvaluations.end())
        {
            _dirtyEvaluations.erase(iterDirty);
        }

        auto iterActive = std::find(_activeEvaluations.begin(), _activeEvaluations.end(), id);
        if (iterActive != _activeEvaluations.end())
        {
            _activeEvaluations.erase(iterActive);
        }

        auto iterEval = _evaluations.find(id);
        if (iterEval != _evaluations.end())
        {
            _evaluations.erase(iterEval);
        }
    }

    void iEvaluationManager::handle()
    {
        con_endl("eval " << _dirtyEvaluations.size() << " -> " << _activeEvaluations.size());

        float64 time = iTimer::getInstance().getSeconds();

        if (!_dirtyEvaluations.empty())
        {
            int move = 0;
            for (auto id : _dirtyEvaluations)
            {
                if (_evaluations[id]->_start >= time)
                {
                    break;
                }

                ++move;
            }

            if (move > 0)
            {
                _activeEvaluations.insert(_activeEvaluations.end(), _dirtyEvaluations.begin(), _dirtyEvaluations.begin() + move);
                _dirtyEvaluations.erase(_dirtyEvaluations.begin(), _dirtyEvaluations.begin() + move);
            }
        }

        auto iterActive = _activeEvaluations.begin();
        while (iterActive != _activeEvaluations.end())
        {
            auto evaluation = _evaluations[*iterActive];

            if (evaluation->_stop < time)
            {
                iterActive = _activeEvaluations.erase(iterActive);
                continue;
            }

            const float64 delta = evaluation->_stop - evaluation->_start;
            float64 t = std::min(1.0, std::max(0.0, (time - evaluation->_start) / delta));

            switch (evaluation->getEasingFunction())
            {
            case Easing::EasingFunction::InSine:
                t = Easing::inSine(t);
                break;
            case Easing::EasingFunction::OutSine:
                t = Easing::outSine(t);
                break;
            case Easing::EasingFunction::InOutSine:
                t = Easing::inOutSine(t);
                break;
            case Easing::EasingFunction::OutInSine:
                t = Easing::outInSine(t);
                break;

            case Easing::EasingFunction::InQuad:
                t = Easing::inQuad(t);
                break;
            case Easing::EasingFunction::OutQuad:
                t = Easing::outQuad(t);
                break;
            case Easing::EasingFunction::InOutQuad:
                t = Easing::inOutQuad(t);
                break;
            case Easing::EasingFunction::OutInQuad:
                t = Easing::outInQuad(t);
                break;

            case Easing::EasingFunction::InCubic:
                t = Easing::inCubic(t);
                break;
            case Easing::EasingFunction::OutCubic:
                t = Easing::outCubic(t);
                break;
            case Easing::EasingFunction::InOutCubic:
                t = Easing::inOutCubic(t);
                break;
            case Easing::EasingFunction::OutInCubic:
                t = Easing::outInCubic(t);
                break;

            case Easing::EasingFunction::InQuart:
                t = Easing::inQuart(t);
                break;
            case Easing::EasingFunction::OutQuart:
                t = Easing::outQuart(t);
                break;
            case Easing::EasingFunction::InOutQuart:
                t = Easing::inOutQuart(t);
                break;
            case Easing::EasingFunction::OutInQuart:
                t = Easing::outInQuart(t);
                break;

            case Easing::EasingFunction::InQuint:
                t = Easing::inQuint(t);
                break;
            case Easing::EasingFunction::OutQuint:
                t = Easing::outQuint(t);
                break;
            case Easing::EasingFunction::InOutQuint:
                t = Easing::inOutQuint(t);
                break;
            case Easing::EasingFunction::OutInQuint:
                t = Easing::outInQuint(t);
                break;

            case Easing::EasingFunction::InExpo:
                t = Easing::inExpo(t);
                break;
            case Easing::EasingFunction::OutExpo:
                t = Easing::outExpo(t);
                break;
            case Easing::EasingFunction::InOutExpo:
                t = Easing::inOutExpo(t);
                break;
            case Easing::EasingFunction::OutInExpo:
                t = Easing::outInExpo(t);
                break;

            case Easing::EasingFunction::InCirc:
                t = Easing::inCirc(t);
                break;
            case Easing::EasingFunction::OutCirc:
                t = Easing::outCirc(t);
                break;
            case Easing::EasingFunction::InOutCirc:
                t = Easing::inOutCirc(t);
                break;
            case Easing::EasingFunction::OutInCirc:
                t = Easing::outInCirc(t);
                break;

            case Easing::EasingFunction::InBack:
                t = Easing::inBack(t, 0.0, 1.0, 1.0, evaluation->getOvershoot());
                break;
            case Easing::EasingFunction::OutBack:
                t = Easing::outBack(t, 0.0, 1.0, 1.0, evaluation->getOvershoot());
                break;
            case Easing::EasingFunction::InOutBack:
                t = Easing::inOutBack(t, 0.0, 1.0, 1.0, evaluation->getOvershoot());
                break;
            case Easing::EasingFunction::OutInBack:
                t = Easing::outInBack(t, 0.0, 1.0, 1.0, evaluation->getOvershoot());
                break;

            case Easing::EasingFunction::InElastic:
                t = Easing::inElastic(t, 0.0, 1.0, 1.0, evaluation->getAmplitude(), evaluation->getPeriod());
                break;
            case Easing::EasingFunction::OutElastic:
                t = Easing::outElastic(t, 0.0, 1.0, 1.0, evaluation->getAmplitude(), evaluation->getPeriod());
                break;
            case Easing::EasingFunction::InOutElastic:
                t = Easing::inOutElastic(t, 0.0, 1.0, 1.0, evaluation->getAmplitude(), evaluation->getPeriod());
                break;
            case Easing::EasingFunction::OutInElastic:
                t = Easing::outInElastic(t, 0.0, 1.0, 1.0, evaluation->getAmplitude(), evaluation->getPeriod());
                break;

            case Easing::EasingFunction::InBounce:
                t = Easing::inBounce(t);
                break;
            case Easing::EasingFunction::OutBounce:
                t = Easing::outBounce(t);
                break;
            case Easing::EasingFunction::InOutBounce:
                t = Easing::inOutBounce(t);
                break;
            case Easing::EasingFunction::OutInBounce:
                t = Easing::outInBounce(t);
                break;
            }

            evaluation->evaluate(t);

            ++iterActive;
        }
    }

}