// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntitySystemModule.h>

#include <igor/resources/profiler/iProfiler.h>
#include <igor/system/iTimer.h>

namespace igor
{

    class iEntitySceneDeleter
    {
    public:
        void operator()(iEntityScene *p) { delete p; }
    };

    iEntityScenePtr iEntitySystemModule::createScene()
    {
        iEntityScenePtr scene = iEntityScenePtr(new iEntityScene(), iEntitySceneDeleter());
        _scenes.push_back(scene);
        return scene;
    }

    void iEntitySystemModule::start()
    {
        if (!_running)
        {
            _simulationFrameTime = iTimer::getInstance().getTime();
            _running = true;
        }
    }

    void iEntitySystemModule::stop()
    {
        if (_running)
        {
            _running = false;
        }
    }

    void iEntitySystemModule::setSimulationRate(float64 simulationRate)
    {
        _simulationRate = simulationRate;
    }

    float64 iEntitySystemModule::getSimulationRate()
    {
        return _simulationRate;
    }

    void iEntitySystemModule::onUpdate()
    {
        if (_running)
        {
            const uint32 maxUpdateCount = 10;
            const iaTime timeDelta = iaTime::fromSeconds(1.0 / _simulationRate);

            uint32 updateCount = 0;
            iaTime currentTime = iTimer::getInstance().getTime();

            while ((_simulationFrameTime + timeDelta < currentTime) &&
                   (updateCount < maxUpdateCount))
            {
                for (auto scene : _scenes)
                {
                    scene->onUpdate(_simulationFrameTime);
                }
                _simulationFrameTime += timeDelta;
                updateCount++;
            };
        }
    }

    void iEntitySystemModule::onRender(float32 clientWidth, float32 clientHeight)
    {
        for (auto scene : _scenes)
        {
            scene->onRender(clientWidth, clientHeight);
        }
    }

} // namespace igor