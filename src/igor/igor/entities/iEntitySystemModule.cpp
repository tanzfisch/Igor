// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntitySystemModule.h>

#include <igor/entities/systems/iAnimationSystem.h>
#include <igor/entities/systems/iBehaviourSystem.h>
#include <igor/entities/systems/iSpriteRenderSystem.h>
#include <igor/entities/systems/iTransformSystem.h>
#include <igor/entities/systems/iVelocitySystem.h>
#include <igor/entities/systems/iCameraSystem.h>
#include <igor/entities/systems/iMeshRenderSystem.h>
#include <igor/entities/systems/iLightSystem.h>

#include <igor/entities/components/iQuadtreeComponent.h>
#include <igor/entities/components/iOctreeComponent.h>
#include <igor/entities/components/iSpriteRenderComponent.h>
#include <igor/entities/components/iTransformComponent.h>
#include <igor/entities/components/iLightComponent.h>
#include <igor/entities/components/iMeshRenderComponent.h>
#include <igor/entities/components/iCameraComponent.h>
#include <igor/entities/components/iCircleCollision2DComponent.h>
#include <igor/entities/components/iSphereCollision3DComponent.h>

#include <igor/resources/profiler/iProfiler.h>

#include <igor/system/iTimer.h>

namespace igor
{

    iEntitySystemModule::iEntitySystemModule()
    {
        registerComponentType<iSpriteRenderComponent>();
        registerComponentType<iTransformComponent>();
        registerComponentType<iLightComponent>();
        registerComponentType<iQuadtreeComponent>();
        registerComponentType<iOctreeComponent>();
        registerComponentType<iCircleCollision2DComponent>();
        registerComponentType<iSphereCollision3DComponent>();
        registerComponentType<iVelocityComponent>();
        registerComponentType<iBehaviourComponent>();
        registerComponentType<iGlobalBoundaryComponent>();
        registerComponentType<iMotionInteractionResolverComponent>();
        registerComponentType<iCameraComponent>();
        registerComponentType<iRenderDebugComponent>();
        registerComponentType<iPartyComponent>();
        registerComponentType<iAnimationComponent>();
        registerComponentType<iMeshRenderComponent>();
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
        scene->addSystem(new iVelocitySystem());

        scene->addSystem(new iTransformSystem());

        scene->addSystem(new iCameraSystem());
        scene->addSystem(new iMeshRenderSystem());

        scene->addSystem(new iLightSystem());
        scene->addSystem(new iSpriteRenderSystem());
        
        _activeScenes.push_back(scene);

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

        iEntityScenePtr scene = iter->second;

        auto activeIter = std::find(_activeScenes.begin(), _activeScenes.end(), scene);
        if (activeIter != _activeScenes.end())
        {
            _activeScenes.erase(activeIter);
        }
        auto inactiveIter = std::find(_inactiveScenes.begin(), _inactiveScenes.end(), scene);
        if (inactiveIter != _inactiveScenes.end())
        {
            _inactiveScenes.erase(inactiveIter);
        }

        delete scene;
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

        if(maxUpdateCount <= 0)
        {
            _simulationFrameTime = currentTime;
            con_trace("Loosing frames");
        }

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

    void iEntitySystemModule::activateScene(iEntityScenePtr scene)
    {
        auto iter = std::find(_inactiveScenes.begin(), _inactiveScenes.end(), scene);
        if (iter != _inactiveScenes.end())
        {
            _inactiveScenes.erase(iter);
            _activeScenes.push_back(scene);
        }
    }

    void iEntitySystemModule::deactivateScene(iEntityScenePtr scene)
    {
        auto iter = std::find(_activeScenes.begin(), _activeScenes.end(), scene);
        if (iter != _activeScenes.end())
        {
            _activeScenes.erase(iter);
            _inactiveScenes.push_back(scene);
        }
    }

    const std::vector<iEntityScenePtr> &iEntitySystemModule::getActiveScenes() const
    {
        return _activeScenes;
    }

    const std::vector<iEntityScenePtr> &iEntitySystemModule::getInactiveScenes() const
    {
        return _inactiveScenes;
    }

    iCreatedEntityEvent &iEntitySystemModule::getCreatedEntityEvent()
    {
        return _createdEntityEvent;
    }

    iDestroyEntityEvent &iEntitySystemModule::getDestroyEntityEvent()
    {
        return _destroyEntityEvent;
    }

} // namespace igor