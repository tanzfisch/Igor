// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntitySystemModule.h>

#include <igor/entities/systems/iQuadtreeSystem.h>
#include <igor/entities/systems/iAnimationSystem.h>
#include <igor/entities/systems/iBehaviourSystem.h>
#include <igor/entities/systems/iSpriteRenderSystem.h>
#include <igor/entities/systems/iTransformSystem.h>
#include <igor/entities/systems/iVelocitySystem.h>
#include <igor/entities/systems/iCameraSystem.h>

#include <igor/entities/components/iBody2DComponent.h>

#include <igor/resources/profiler/iProfiler.h>

#include <igor/system/iTimer.h>

namespace igor
{

    iEntitySystemModule::iEntitySystemModule()
    {
        registerComponentType<iSpriteRendererComponent>();
        registerComponentType<iTransformComponent>();
        registerComponentType<iBody2DComponent>();
        registerComponentType<iCircleCollision2DComponent>();
        registerComponentType<iVelocityComponent>();
        registerComponentType<iBehaviourComponent>();
        registerComponentType<iGlobalBoundaryComponent>();
        registerComponentType<iMotionInteractionResolverComponent>();
        registerComponentType<iCameraComponent>();
        registerComponentType<iRenderDebugComponent>();
        registerComponentType<iPartyComponent>();
        registerComponentType<iAnimationComponent>();
    }

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
        scene->addSystem(new iVelocitySystem());

        scene->addSystem(new iTransformSystem());

        scene->addSystem(new iCameraSystem());
        scene->addSystem(new iSpriteRenderSystem());

        return scene;
    }

    iEntityComponentMask iEntitySystemModule::getComponentMask(const std::type_index &typeID) const
    {
        auto iter = _registeredComponentTypes.find(typeID);
        if (iter == _registeredComponentTypes.end())
        {
            con_err("component type not registered " << typeID.name());
            return iEntityComponentMask(0);
        }

        return iter->second;
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
        const iaTime timeDelta = iaTime::fromSeconds(1.0 / _simulationRate);
        const iaTime currentTime = iTimer::getInstance().getTime();

        int32 maxUpdateCount = 10;

        while ((_simulationFrameTime + timeDelta < currentTime) &&
               maxUpdateCount > 0)
        {
            for (auto pair : _scenes)
            {
                pair.second->onUpdate(_simulationFrameTime, iEntitySystemStage::Update);
            }
            _simulationFrameTime += timeDelta;
            maxUpdateCount--;
        };

        uint64 entityCount = 0;
        for (auto pair : _scenes)
        {
            entityCount += pair.second->_entities.size();
        }
        iProfiler::setValue("entity count", entityCount);
    }

    void iEntitySystemModule::onPreRender(iEntityScenePtr scene)
    {
        scene->onUpdate(_simulationFrameTime, iEntitySystemStage::PreRender);
    }

    void iEntitySystemModule::onRender(iEntityScenePtr scene)
    {
        scene->onUpdate(_simulationFrameTime, iEntitySystemStage::Render);
    }

} // namespace igor