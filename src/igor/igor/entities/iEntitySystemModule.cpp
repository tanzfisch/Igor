// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntitySystemModule.h>

#include <igor/entities/systems/iQuadtreeSystem.h>
#include <igor/entities/systems/iAnimationSystem.h>
#include <igor/entities/systems/iBehaviourSystem.h>
#include <igor/entities/systems/iSpriteRenderSystem.h>
#include <igor/entities/systems/iTransformHierarchySystem.h>
#include <igor/entities/systems/iVelocitySystem.h>

#include <igor/system/iTimer.h>

namespace igor
{

    iEntityScenePtr iEntitySystemModule::createScene(const iaString &name, bool addIgorSystems)
    {
        // TODO create unique names
        iEntityScenePtr scene = new iEntityScene(name);
        _scenes[scene->getID()] = scene;

        if (!addIgorSystems)
        {
            return scene;
        }

        scene->addSystem(new iAnimationSystem());
        scene->addSystem(new iBehaviourSystem());
        scene->addSystem(new iQuadtreeSystem());
        scene->addSystem(new iSpriteRenderSystem());
        scene->addSystem(new iTransformHierarchySystem());
        scene->addSystem(new iVelocitySystem());

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
        const iaTime currentTime = iTimer::getInstance().getTime();

        while ((_simulationFrameTime + timeDelta < currentTime) &&
               (updateCount < maxUpdateCount))
        {
            for (auto pair : _scenes)
            {
                pair.second->onUpdate(_simulationFrameTime, iEntitySystemStage::Update);
            }
            _simulationFrameTime += timeDelta;
            updateCount++;
        };
    }

    void iEntitySystemModule::onRender(float32 clientWidth, float32 clientHeight)
    {
        const iaTime currentTime = iTimer::getInstance().getTime();
        for (auto pair : _scenes)
        {
            pair.second->onUpdate(_simulationFrameTime, iEntitySystemStage::Render);
        }
    }

} // namespace igor