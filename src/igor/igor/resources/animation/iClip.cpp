// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/animation/iClip.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <functional>

namespace igor
{

    iClipPtr iClip::createClip(const std::vector<iAnimationPtr> &animations, bool looped, bool randomStart, iaEasing::iaEasingFunction easingFunction)
    {
        iClipPtr result(new iClip());

        for (auto animation : animations)
        {
            result->addAnimation(animation);
        }

        result->setLooped(looped);
        result->setRandomStart(randomStart);
        result->setEasingFunction(easingFunction);

        return result;
    }

    iClip::iClip()
    {
    }

    void iClip::setLooped(bool loop)
    {
        _looped = loop;
    }

    bool iClip::isLooped() const
    {
        return _looped;
    }

    void iClip::setRandomStart(bool randomStart)
    {
        _randomStart = randomStart;
    }

    bool iClip::hasRandomStart() const
    {
        return _randomStart;
    }

    const std::vector<iAnimationPtr> &iClip::getAnimations() const
    {
        return _animations;
    }

    void iClip::addAnimation(iAnimationPtr animation)
    {
        con_assert(animation != nullptr, "zero pointer");

        auto iter = std::find(_animations.begin(), _animations.end(), animation);
        if (iter != _animations.end())
        {
            return;
        }

        _animations.push_back(animation);

        _dirtyStartStop = true;
    }

    void iClip::removeAnimation(iAnimationPtr animation)
    {
        auto iter = std::find(_animations.begin(), _animations.end(), animation);
        if (iter != _animations.end())
        {
            _animations.erase(iter);
        }

        _dirtyStartStop = true;
    }

    void iClip::updateStartStop()
    {
        if (!_dirtyStartStop)
        {
            return;
        }

        bool first = true;
        for (auto &animation : _animations)
        {
            if (!animation->isProcessed() ||
                !animation->isValid())
            {
                return;
            }

            if (first)
            {
                _start = animation->getStart();
                _stop = animation->getStop();
                first = false;
            }
            else
            {
                _start = std::min(_start, animation->getStart());
                _stop = std::max(_stop, animation->getStop());
            }
        }

        _duration = _stop - _start;
        _dirtyStartStop = false;
    }

    const iaTime &iClip::getStart()
    {
        updateStartStop();
        return _start;
    }

    const iaTime &iClip::getStop()
    {
        updateStartStop();
        return _stop;
    }

    const iaTime &iClip::getDuration()
    {
        updateStartStop();
        return _duration;
    }

    void iClip::setEasingFunction(iaEasing::iaEasingFunction easingFunction)
    {
        _easingFunction = easingFunction;
    }

    iaEasing::iaEasingFunction iClip::getEasingFunction() const
    {
        return _easingFunction;
    }

    static float64 linear(float64 t)
    {
        return t;
    }

    static float64 inSine(float64 t)
    {
        return iaEasing::inSine(t, 0, 1, 1);
    }

    static float64 outSine(float64 t)
    {
        return iaEasing::outSine(t, 0, 1, 1);
    }

    static float64 inOutSine(float64 t)
    {
        return iaEasing::inOutSine(t, 0, 1, 1);
    }

    static float64 outInSine(float64 t)
    {
        return iaEasing::outInSine(t, 0, 1, 1);
    }

    static float64 inQuad(float64 t)
    {
        return iaEasing::inQuad(t, 0, 1, 1);
    }

    static float64 outQuad(float64 t)
    {
        return iaEasing::outQuad(t, 0, 1, 1);
    }

    static float64 inOutQuad(float64 t)
    {
        return iaEasing::inOutQuad(t, 0, 1, 1);
    }

    static float64 outInQuad(float64 t)
    {
        return iaEasing::outInQuad(t, 0, 1, 1);
    }

    static float64 inCubic(float64 t)
    {
        return iaEasing::inCubic(t, 0, 1, 1);
    }

    static float64 outCubic(float64 t)
    {
        return iaEasing::outCubic(t, 0, 1, 1);
    }

    static float64 inOutCubic(float64 t)
    {
        return iaEasing::inOutCubic(t, 0, 1, 1);
    }

    static float64 outInCubic(float64 t)
    {
        return iaEasing::outInCubic(t, 0, 1, 1);
    }

    static float64 inQuart(float64 t)
    {
        return iaEasing::inQuart(t, 0, 1, 1);
    }

    static float64 outQuart(float64 t)
    {
        return iaEasing::outQuart(t, 0, 1, 1);
    }

    static float64 inOutQuart(float64 t)
    {
        return iaEasing::inOutQuart(t, 0, 1, 1);
    }

    static float64 outInQuart(float64 t)
    {
        return iaEasing::outInQuart(t, 0, 1, 1);
    }

    static float64 inQuint(float64 t)
    {
        return iaEasing::inQuint(t, 0, 1, 1);
    }

    static float64 outQuint(float64 t)
    {
        return iaEasing::outQuint(t, 0, 1, 1);
    }

    static float64 inOutQuint(float64 t)
    {
        return iaEasing::inOutQuint(t, 0, 1, 1);
    }

    static float64 outInQuint(float64 t)
    {
        return iaEasing::outInQuint(t, 0, 1, 1);
    }

    static float64 inExpo(float64 t)
    {
        return iaEasing::inExpo(t, 0, 1, 1);
    }

    static float64 outExpo(float64 t)
    {
        return iaEasing::outExpo(t, 0, 1, 1);
    }

    static float64 inOutExpo(float64 t)
    {
        return iaEasing::inOutExpo(t, 0, 1, 1);
    }

    static float64 outInExpo(float64 t)
    {
        return iaEasing::outInExpo(t, 0, 1, 1);
    }

    static float64 inCirc(float64 t)
    {
        return iaEasing::inCirc(t, 0, 1, 1);
    }

    static float64 outCirc(float64 t)
    {
        return iaEasing::outCirc(t, 0, 1, 1);
    }

    static float64 inOutCirc(float64 t)
    {
        return iaEasing::inOutCirc(t, 0, 1, 1);
    }

    static float64 outInCirc(float64 t)
    {
        return iaEasing::outInCirc(t, 0, 1, 1);
    }

    static float64 outBounce(float64 t)
    {
        return iaEasing::outBounce(t, 0, 1, 1);
    }

    static float64 inBounce(float64 t)
    {
        return iaEasing::inBounce(t, 0, 1, 1);
    }

    static float64 inOutBounce(float64 t)
    {
        return iaEasing::inOutBounce(t, 0, 1, 1);
    }

    static float64 outInBounce(float64 t)
    {
        return iaEasing::outInBounce(t, 0, 1, 1);
    }

    static std::vector<std::function<float64(float64)>> s_easingFunctionTable = {
        linear,
        inSine,
        outSine,
        inOutSine,
        outInSine,
        inQuad,
        outQuad,
        inOutQuad,
        outInQuad,
        inCubic,
        outCubic,
        inOutCubic,
        outInCubic,
        inQuart,
        outQuart,
        inOutQuart,
        outInQuart,
        inQuint,
        outQuint,
        inOutQuint,
        outInQuint,
        inExpo,
        outExpo,
        inOutExpo,
        outInExpo,
        inCirc,
        outCirc,
        inOutCirc,
        outInCirc,
        outBounce,
        inBounce,
        inOutBounce,
        outInBounce};

    float64 iClip::getNormalizedTime(const iaTime &startTime, const iaTime &time)
    {
        updateStartStop();

        if (time <= startTime + _start)
        {
            return 0.0;
        }

        if (time >= startTime + _stop)
        {
            return 1.0;
        }

        const float64 t = static_cast<float64>(time.getMicroseconds() - (startTime.getMicroseconds() + _start.getMicroseconds())) / static_cast<float64>(_duration.getMicroseconds());
        return s_easingFunctionTable[(int)_easingFunction](t);
    }

}; // namespace igor
