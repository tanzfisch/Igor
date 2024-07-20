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
#include <igor/entities/components/iMeshReferenceComponent.h>
#include <igor/entities/components/iCameraComponent.h>
#include <igor/entities/components/iCircleCollision2DComponent.h>
#include <igor/entities/components/iSphereCollision3DComponent.h>
#include <igor/entities/components/iPrefabComponent.h>

#include <igor/entities/traversal/iEntityCopyTraverser.h>

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
        registerComponentType<iMeshReferenceComponent>();
        registerComponentType<iPrefabComponent>();

        _simulationFrameTime = iTimer::getInstance().getTime();
    }

    iEntityScenePtr iEntitySystemModule::createScene(const iaString &name, const iEntitySceneID &id, bool addIgorSystems)
    {
        iEntityScenePtr scene = new iEntityScene(name);
        if (id.isValid())
        {
            scene->_id = id;
        }

        _mutex.lock();
        con_assert(_scenes.find(scene->getID()) == _scenes.end(), "id collision");

        _scenes[scene->getID()] = scene;
        _inactiveScenes.push_back(scene);
        _mutex.unlock();

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

        return scene;
    }

    iEntityComponentMask iEntitySystemModule::getComponentMask(const std::type_index &typeID)
    {
        iEntityComponentMask result;

        _mutex.lock();
        auto iter = _registeredComponentTypes.find(typeID);
        if (iter != _registeredComponentTypes.end())
        {
            result = iter->second;
        }
        else
        {
            con_err("component type not registered " << typeID.name());
        }
        _mutex.unlock();

        return result;
    }

    iEntityScenePtr iEntitySystemModule::getScene(const iEntitySceneID &sceneID)
    {
        iEntityScenePtr result = nullptr;

        _mutex.lock();
        auto iter = _scenes.find(sceneID);
        if (iter != _scenes.end())
        {
            result = iter->second;
        }
        else
        {
            con_err("scene with id " << sceneID << " not found");            
        }
        _mutex.unlock();

        return result;
    }

    void iEntitySystemModule::destroyScene(const iEntitySceneID &sceneID)
    {
        _mutex.lock();
        auto iter = _scenes.find(sceneID);
        if (iter == _scenes.end())
        {
            _mutex.unlock();
            con_warn("scene with id " << sceneID << " not found");
            return;
        }

        iEntityScenePtr scene = iter->second;
        _scenes.erase(iter);

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

        _mutex.unlock();

        delete scene;
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

        uint64 entityCount = 0;
        std::vector<iEntityScenePtr> scenes;

        _mutex.lock();
        for (auto pair : _scenes)
        {
            scenes.push_back(pair.second);
            entityCount += pair.second->_entities.size();
        }
        _mutex.unlock();

        iProfiler::setValue("entity count", entityCount);

        while ((_simulationFrameTime + timeDelta < currentTime) &&
               maxUpdateCount > 0)
        {
            for (auto scene : scenes)
            {
                scene->onUpdate(_simulationFrameTime, iEntitySystemStage::Update);
            }
            _simulationFrameTime += timeDelta;
            maxUpdateCount--;
        };

        if (maxUpdateCount <= 0)
        {
            _simulationFrameTime = currentTime;
            con_trace("Loosing frames");
        }
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
        _mutex.lock();
        auto iter = std::find(_inactiveScenes.begin(), _inactiveScenes.end(), scene);
        if (iter != _inactiveScenes.end())
        {
            _inactiveScenes.erase(iter);
            _activeScenes.push_back(scene);
        }
        _mutex.unlock();
    }

    void iEntitySystemModule::deactivateScene(iEntityScenePtr scene)
    {
        _mutex.lock();
        auto iter = std::find(_activeScenes.begin(), _activeScenes.end(), scene);
        if (iter != _activeScenes.end())
        {
            _activeScenes.erase(iter);
            _inactiveScenes.push_back(scene);
        }
        _mutex.unlock();
    }

    std::vector<iEntityScenePtr> iEntitySystemModule::getActiveScenes()
    {
        std::vector<iEntityScenePtr> result;
        _mutex.lock();
        result = _activeScenes;
        _mutex.unlock();
        return result;
    }

    std::vector<iEntityScenePtr> iEntitySystemModule::getInactiveScenes()
    {
        std::vector<iEntityScenePtr> result;
        _mutex.lock();
        result = _inactiveScenes;
        _mutex.unlock();
        return result;
    }

    iCreatedEntityEvent &iEntitySystemModule::getCreatedEntityEvent()
    {
        return _createdEntityEvent;
    }

    iDestroyEntityEvent &iEntitySystemModule::getDestroyEntityEvent()
    {
        return _destroyEntityEvent;
    }

    void iEntitySystemModule::clear()
    {
        _mutex.lock();
        std::vector<iEntitySceneID> ids;

        for (const auto &pair : _scenes)
        {
            ids.push_back(pair.first);
        }
        _mutex.unlock();

        for (const auto &id : ids)
        {
            destroyScene(id);
        }

        con_assert(_scenes.empty() && _activeScenes.empty() && _inactiveScenes.empty(), "clean up failed");
    }

    void iEntitySystemModule::insert(iPrefabPtr prefab, iEntityPtr entity)
    {
        _createdEntityEvent.block();
        iEntityCopyTraverser traverser(prefab, entity);
        iEntityScenePtr scene = getScene(prefab->getSceneID());
        traverser.traverse(scene);
        _createdEntityEvent.unblock();
    }

} // namespace igor