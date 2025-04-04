// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntityScene.h>

#include <igor/entities/iEntitySystemModule.h>
#include <igor/entities/systems/iCameraSystem.h>
#include <igor/renderer/iRenderer.h>
#include <igor/renderer/iRenderEngine.h>

namespace igor
{
    iEntityScene::iEntityScene(const iaString &name)
    {
        setName(name);

        _root = new iEntity("root");
        _root->_scene = this;
    }

    iEntityScene::~iEntityScene()
    {
        _deleteQueueMutex.lock();
        _deleteQueue.clear();
        _deleteQueueMutex.unlock();

        delete _root;

        _systemsMutex.lock();
        for (int i = 0; i < (int)iEntitySystemStage::StageCount; ++i)
        {
            for (auto system : _systems[i])
            {
                delete system;
            }
        }
        _systemsMutex.unlock();
    }

    void iEntityScene::initializeQuadtree(const iaRectangled &rect, const uint32 splitThreshold, const uint32 maxDepth)
    {
        con_assert(_quadtree == nullptr, "Quadtree already initialized");
        _quadtree = new iQuadtreed(rect, splitThreshold, maxDepth);
    }

    iQuadtreed &iEntityScene::getQuadtree() const
    {
        con_assert(_quadtree != nullptr, "Quadtree was not initialized");
        return *_quadtree;
    }

    bool iEntityScene::hasQuadtree() const
    {
        return _quadtree != nullptr;
    }

    void iEntityScene::initializeOctree(const iAACubed &cube, const uint32 splitThreshold, const uint32 maxDepth)
    {
        con_assert(_octree == nullptr, "Octree already initialized");
        _octree = new iOctreed(cube, splitThreshold, maxDepth);
    }

    iOctreed &iEntityScene::getOctree() const
    {
        con_assert(_octree != nullptr, "Octree was not initialized");
        return *_octree;
    }

    bool iEntityScene::hasOctree() const
    {
        return _octree != nullptr;
    }

    void iEntityScene::onUpdate(const iaTime &time, iEntitySystemStage stage)
    {
        con_assert((int)stage < (int)iEntitySystemStage::StageCount, "out of range stage");

        if (stage == iEntitySystemStage::Update)
        {
            flushQueues();
        }

        _mutex.lock();
        const auto systems = _systems[(int)stage];
        _mutex.unlock();

        for (auto system : systems)
        {
            system->onUpdate(iEntitySceneUpdateContext{time, stage, this, _renderEngine});
        }
    }

    void iEntityScene::setRenderEngine(iRenderEnginePtr renderEngine)
    {
        _renderEngine = renderEngine;
        if (_renderEngine == nullptr)
        {
            return;
        }

        _renderEngine->setScene(this);
    }

    void iEntityScene::flushQueues()
    {
        _deleteQueueMutex.lock();
        const auto deleteQueue = std::move(_deleteQueue);
        _deleteQueueMutex.unlock();

        for (auto entity : deleteQueue)
        {
            _mutex.lock();
            auto iter = _entities.find(entity->getID());
            if (iter == _entities.end())
            {
                _mutex.unlock();
                continue;
            }
            _entities.erase(iter);
            _mutex.unlock();

            iEntitySystemModule::getInstance().getDestroyEntityEvent()(entity);
            delete entity;
        }

        _processQueueMutex.lock();
        const auto processQueue = std::move(_processQueue);
        _processQueueMutex.unlock();

        std::vector<iEntityPtr> processAgain;

        for (auto entity : processQueue)
        {
            if(!entity->processComponents())
            {
                processAgain.push_back(entity);
            }
        }

        _processQueueMutex.lock();
        _processQueue.insert(_processQueue.end(), processAgain.begin(), processAgain.end());
        _processQueueMutex.unlock();        
    }

    void iEntityScene::setName(const iaString &name)
    {
        _name = name;
    }

    const iaString &iEntityScene::getName() const
    {
        return _name;
    }

    iEntityPtr iEntityScene::createEntity(iEntityPtr srcEntity)
    {
        iEntityPtr entity = new iEntity(srcEntity->getName());
        entity->_id = srcEntity->getID();

        _mutex.lock();
        con_assert(_entities.find(entity->getID()) == _entities.end(), "id collision");

        _entities[entity->getID()] = entity;
        entity->_scene = this;
        entity->setParent(_root);
        _mutex.unlock();

        iEntitySystemModule::getInstance().getCreatedEntityEvent()(entity);

        const auto components = srcEntity->getComponents();
        for (const auto &pair : components)
        {
            const auto &typeIndex = pair.first;
            const iEntityComponentPtr component = pair.second->getCopy();
            entity->addComponent(typeIndex, component);
        }

        return entity;
    }

    iEntityPtr iEntityScene::createEntity(const iaString &name, const iEntityID &id)
    {
        iEntityPtr entity = new iEntity(name);
        if (id.isValid())
        {
            entity->_id = id;
        }
        _mutex.lock();
        con_assert(_entities.find(entity->getID()) == _entities.end(), "id collision");

        _entities[entity->getID()] = entity;
        entity->_scene = this;
        entity->setParent(_root);
        _mutex.unlock();

        iEntitySystemModule::getInstance().getCreatedEntityEvent()(entity);

        return entity;
    }

    iEntityPtr iEntityScene::getRootEntity() const
    {
        return _root;
    }

    iEntityPtr iEntityScene::getEntity(iEntityID entityID)
    {
        iEntityPtr result = nullptr;

        _mutex.lock();
        auto iter = _entities.find(entityID);
        if (iter != _entities.end())
        {
            result = iter->second;
        }
        _mutex.unlock();

        return result;
    }

    void iEntityScene::destroyEntity(iEntityPtr entity)
    {
        con_assert(entity != nullptr, "zero pointer");
        _deleteQueueMutex.lock();
        _deleteQueue.push_back(entity);
        _deleteQueueMutex.unlock();
    }

    void iEntityScene::destroyEntity(iEntityID entityID)
    {
        auto entity = getEntity(entityID);
        con_assert(entity != nullptr, "entity not found");
        destroyEntity(entity);
    }

    const iEntitySceneID &iEntityScene::getID() const
    {
        return _id;
    }

    void iEntityScene::onComponentToProcess(iEntityPtr entity, const std::type_index &typeID)
    {
        _processQueueMutex.lock();
        _processQueue.push_back(entity);
        _processQueueMutex.unlock();
    }

    void iEntityScene::onComponentAdded(iEntityPtr entity, const std::type_index &typeID)
    {
        // TODO this seems such a waste
        _systemsMutex.lock();
        auto systemPhases = _systems;
        _systemsMutex.unlock();
        for (auto systems : systemPhases)
        {
            for (auto system : systems)
            {
                system->onComponentAdded(entity, typeID);
            }
        }
    }

    void iEntityScene::onComponentRemoved(iEntityPtr entity, const std::type_index &typeID)
    {
        // TODO this seems such a waste
        _systemsMutex.lock();
        auto systemPhases = _systems;
        _systemsMutex.unlock();
        for (auto systems : systemPhases)
        {
            for (auto system : systems)
            {
                system->onComponentRemoved(entity, typeID);
            }
        }
    }

    void iEntityScene::onComponentToRemove(iEntityPtr entity, const std::type_index &typeID)
    {
        // TODO this seems such a waste
        _systemsMutex.lock();
        auto systemPhases = _systems;
        _systemsMutex.unlock();
        for (auto systems : systemPhases)
        {
            for (auto system : systems)
            {
                system->onComponentToRemove(entity, typeID);
            }
        }
    }

    void iEntityScene::onEntityChanged(iEntityPtr entity)
    {
        // TODO this seems such a waste
        _systemsMutex.lock();
        auto systemPhases = _systems;
        _systemsMutex.unlock();
        for (auto systems : systemPhases)
        {
            for (auto system : systems)
            {
                system->onEntityChanged(entity);
            }
        }
    }

    void iEntityScene::addSystem(iEntitySystemPtr system)
    {
        _systemsMutex.lock();
        auto &systems = _systems[(int)system->getStage()];
        con_assert(std::find(systems.begin(), systems.end(), system) == systems.end(), "system already added");
        systems.push_back(system);
        system->_scene = this;

        iCameraSystem *cameraSystem = dynamic_cast<iCameraSystem *>(system);
        if (cameraSystem != nullptr)
        {
            _cameraSystem = cameraSystem;
        }
        _systemsMutex.unlock();

        _mutex.lock();
        auto entities = _entities;
        _mutex.unlock();

        for (const auto &pair : entities)
        {
            system->onEntityChanged(pair.second);
        }
    }

    void iEntityScene::removeSystem(iEntitySystemPtr system)
    {
        _systemsMutex.lock();
        auto &systems = _systems[(int)system->getStage()];
        auto iter = std::find(systems.begin(), systems.end(), system);
        if (iter == systems.end())
        {
            _systemsMutex.unlock();
            return;
        }

        systems.erase(iter);

        system->_scene = nullptr;

        if (system == _cameraSystem)
        {
            _cameraSystem = nullptr;
        }
        _systemsMutex.unlock();
    }

    std::vector<iEntityPtr> iEntityScene::getCameras() const
    {
        if (_cameraSystem != nullptr)
        {
            return _cameraSystem->getCameras();
        }

        return std::vector<iEntityPtr>();
    }

    iEntityPtr iEntityScene::getActiveCamera() const
    {
        if (_cameraSystem != nullptr)
        {
            return _cameraSystem->getActiveCamera();
        }

        return nullptr;
    }

} // igor
