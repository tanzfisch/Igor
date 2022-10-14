// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/evaluation/iEvaluationManager.h>
#include <igor/system/iTimer.h>

#include <iaux/math/iaEasing.h>
#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <algorithm>

namespace igor
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

        std::sort(_dirtyEvaluations.begin(), _dirtyEvaluations.end(), [](const uint64 a, const uint64 b)
                  {
            auto evaluationA = iEvaluationManager::getInstance().getEvaluation(a);
            auto evaluationB = iEvaluationManager::getInstance().getEvaluation(b);
            return evaluationA->getStart() < evaluationB->getStart(); });
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
        IGOR_PROFILER(eval);
        
        iaTime time = iTimer::getInstance().getTime();

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

            if (!evaluation->_looped &&
                evaluation->_stop < time)
            {
                iterActive = _activeEvaluations.erase(iterActive);
                continue;
            }

            const iaTime interval = evaluation->_stop - evaluation->_start;
            const uint64 intervalCount = (time - evaluation->_start).getMilliseconds() / interval.getMilliseconds();
            const float64 in = ((time - evaluation->_start).getMilliseconds() / interval.getMilliseconds()) - float64(intervalCount);
            float64 t;

            switch (evaluation->getEasingFunction())
            {
            case Easing::EasingFunction::InSine:
                t = Easing::inSine(in);
                break;
            case Easing::EasingFunction::OutSine:
                t = Easing::outSine(in);
                break;
            case Easing::EasingFunction::InOutSine:
                t = Easing::inOutSine(in);
                break;
            case Easing::EasingFunction::OutInSine:
                t = Easing::outInSine(in);
                break;

            case Easing::EasingFunction::InQuad:
                t = Easing::inQuad(in);
                break;
            case Easing::EasingFunction::OutQuad:
                t = Easing::outQuad(in);
                break;
            case Easing::EasingFunction::InOutQuad:
                t = Easing::inOutQuad(in);
                break;
            case Easing::EasingFunction::OutInQuad:
                t = Easing::outInQuad(in);
                break;

            case Easing::EasingFunction::InCubic:
                t = Easing::inCubic(in);
                break;
            case Easing::EasingFunction::OutCubic:
                t = Easing::outCubic(in);
                break;
            case Easing::EasingFunction::InOutCubic:
                t = Easing::inOutCubic(in);
                break;
            case Easing::EasingFunction::OutInCubic:
                t = Easing::outInCubic(in);
                break;

            case Easing::EasingFunction::InQuart:
                t = Easing::inQuart(in);
                break;
            case Easing::EasingFunction::OutQuart:
                t = Easing::outQuart(in);
                break;
            case Easing::EasingFunction::InOutQuart:
                t = Easing::inOutQuart(in);
                break;
            case Easing::EasingFunction::OutInQuart:
                t = Easing::outInQuart(in);
                break;

            case Easing::EasingFunction::InQuint:
                t = Easing::inQuint(in);
                break;
            case Easing::EasingFunction::OutQuint:
                t = Easing::outQuint(in);
                break;
            case Easing::EasingFunction::InOutQuint:
                t = Easing::inOutQuint(in);
                break;
            case Easing::EasingFunction::OutInQuint:
                t = Easing::outInQuint(in);
                break;

            case Easing::EasingFunction::InExpo:
                t = Easing::inExpo(in);
                break;
            case Easing::EasingFunction::OutExpo:
                t = Easing::outExpo(in);
                break;
            case Easing::EasingFunction::InOutExpo:
                t = Easing::inOutExpo(in);
                break;
            case Easing::EasingFunction::OutInExpo:
                t = Easing::outInExpo(in);
                break;

            case Easing::EasingFunction::InCirc:
                t = Easing::inCirc(in);
                break;
            case Easing::EasingFunction::OutCirc:
                t = Easing::outCirc(in);
                break;
            case Easing::EasingFunction::InOutCirc:
                t = Easing::inOutCirc(in);
                break;
            case Easing::EasingFunction::OutInCirc:
                t = Easing::outInCirc(in);
                break;

            case Easing::EasingFunction::InBack:
                t = Easing::inBack(in, 0.0, 1.0, 1.0, evaluation->getOvershoot());
                break;
            case Easing::EasingFunction::OutBack:
                t = Easing::outBack(in, 0.0, 1.0, 1.0, evaluation->getOvershoot());
                break;
            case Easing::EasingFunction::InOutBack:
                t = Easing::inOutBack(in, 0.0, 1.0, 1.0, evaluation->getOvershoot());
                break;
            case Easing::EasingFunction::OutInBack:
                t = Easing::outInBack(in, 0.0, 1.0, 1.0, evaluation->getOvershoot());
                break;

            case Easing::EasingFunction::InElastic:
                t = Easing::inElastic(in, 0.0, 1.0, 1.0, evaluation->getAmplitude(), evaluation->getPeriod());
                break;
            case Easing::EasingFunction::OutElastic:
                t = Easing::outElastic(in, 0.0, 1.0, 1.0, evaluation->getAmplitude(), evaluation->getPeriod());
                break;
            case Easing::EasingFunction::InOutElastic:
                t = Easing::inOutElastic(in, 0.0, 1.0, 1.0, evaluation->getAmplitude(), evaluation->getPeriod());
                break;
            case Easing::EasingFunction::OutInElastic:
                t = Easing::outInElastic(in, 0.0, 1.0, 1.0, evaluation->getAmplitude(), evaluation->getPeriod());
                break;

            case Easing::EasingFunction::InBounce:
                t = Easing::inBounce(in);
                break;
            case Easing::EasingFunction::OutBounce:
                t = Easing::outBounce(in);
                break;
            case Easing::EasingFunction::InOutBounce:
                t = Easing::inOutBounce(in);
                break;
            case Easing::EasingFunction::OutInBounce:
                t = Easing::outInBounce(in);
                break;

            default:
                t = in;
                break;
            }

            evaluation->evaluate(t);

            ++iterActive;
        }
    }

} // namespace igor