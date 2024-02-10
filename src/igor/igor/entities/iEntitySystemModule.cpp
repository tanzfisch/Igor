// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntitySystemModule.h>

#include <igor/system/iTimer.h>

namespace igor
{

    iEntityScenePtr iEntitySystemModule::createScene(const iaString &name)
    {
        // TODO create unique names
        iEntityScenePtr scene = new iEntityScene(name);
        _scenes[scene->getID()] = scene;
        return scene;
    }

    iEntityScenePtr iEntitySystemModule::getScene(const iEntitySceneID &sceneID)
    {
        auto iter = _scenes.find(sceneID);
        if (iter == _scenes.end())
        {
            con_err("scene with id " << sceneID << " not found");
            return nullptr;
        }

        return iter->second;
    }

    void iEntitySystemModule::destroyScene(const iEntitySceneID &sceneID)
    {
        auto iter = _scenes.find(sceneID);
        if (iter == _scenes.end())
        {
            con_warn("scene with id " << sceneID << " not found");
            return;
        }

        delete iter->second;
        _scenes.erase(iter);
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
        const uint32 maxUpdateCount = 10;
        const iaTime timeDelta = iaTime::fromSeconds(1.0 / _simulationRate);

        uint32 updateCount = 0;
        iaTime currentTime = iTimer::getInstance().getTime();

        while ((_simulationFrameTime + timeDelta < currentTime) &&
               (updateCount < maxUpdateCount))
        {
            for (auto pair : _scenes)
            {
                pair.second->onUpdate(_simulationFrameTime);
            }
            _simulationFrameTime += timeDelta;
            updateCount++;
        };
    }

    void iEntitySystemModule::onRender(float32 clientWidth, float32 clientHeight)
    {
        con_endl("iEntitySystemModule::onRender");
    }

} // namespace igor