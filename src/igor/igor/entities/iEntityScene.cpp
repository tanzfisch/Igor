// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntityScene.h>

#include <igor/entities/iEntitySystemModule.h>
#include <igor/entities/systems/iCameraSystem.h>
#include <igor/renderer/iRenderer.h>
#include <igor/renderer/iRenderEngine.h>

namespace igor
{

    // for debugging
    static void renderQuadtree(const iQuadtreed::NodePtr &node)
    {
        if (node == nullptr)
        {
            return;
        }

        iRenderer::getInstance().drawRectangle(node->_box._x, node->_box._y, node->_box._width, node->_box._height, iaColor4f::red);

        for (auto object : node->_objects)
        {
            iRenderer::getInstance().drawCircle(object->_circle._center._x, object->_circle._center._y, object->_circle._radius);
        }

        for (auto child : node->_children)
        {
            renderQuadtree(child);
        }
    }

    iEntityScene::iEntityScene(const iaString &name)
    {
        setName(name);

        _root = new iEntity("root");
        _root->_scene = this;
    }

    iEntityScene::~iEntityScene()
    {
        _deleteQueue.clear();

        delete _root;

        for (int i = 0; i < (int)iEntitySystemStage::StageCount; ++i)
        {
            for (auto system : _systems[i])
            {
                delete system;
            }
        }
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

        auto &systems = _systems[(int)stage];
        for (auto system : systems)
        {
            system->onUpdate(iEntitySceneUpdateContext{time, stage, this, _renderEngine});
        }
    }

    void iEntityScene::setRenderEngine(iRenderEnginePtr renderEngine)
    {
        _renderEngine = renderEngine;
        _renderEngine->setScene(this);
    }

    void iEntityScene::flushQueues()
    {
        const auto deleteQueue = std::move(_deleteQueue);
        for (auto entity : deleteQueue)
        {
            auto iter = _entities.find(entity->getID());
            if (iter == _entities.end())
            {
                continue;
            }

            iEntitySystemModule::getInstance().getDestroyEntityEvent()(iter->second);

            delete iter->second;
            _entities.erase(iter);
        }

        const auto processQueue = std::move(_processQueue);
        for (auto entity : processQueue)
        {
            entity->processComponents();
        }
    }

    void iEntityScene::setName(const iaString &name)
    {
        _name = name;
    }

    const iaString &iEntityScene::getName() const
    {
        return _name;
    }

    iEntityPtr iEntityScene::createEntity(const iaString &name)
    {
        iEntityPtr entity = new iEntity(name);
        _entities[entity->getID()] = entity;
        entity->_scene = this;
        entity->setParent(_root);

        iEntitySystemModule::getInstance().getCreatedEntityEvent()(entity);

        return entity;
    }

    iEntityPtr iEntityScene::getEntity(iEntityID entityID) const
    {
        auto iter = _entities.find(entityID);
        if (iter == _entities.end())
        {
            return nullptr;
        }

        return iter->second;
    }

    void iEntityScene::destroyEntity(iEntityPtr entity)
    {
        con_assert(entity != nullptr, "zero pointer");
        _deleteQueue.push_back(entity);
    }

    const iEntitySceneID &iEntityScene::getID() const
    {
        return _id;
    }

    void iEntityScene::onComponentToAdd(iEntityPtr entity, const std::type_index &typeID)
    {
        _processQueue.push_back(entity);
    }

    void iEntityScene::onComponentAdded(iEntityPtr entity, const std::type_index &typeID)
    {
        // TODO this seems such a waste
        for (auto systems : _systems)
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
        for (auto systems : _systems)
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
        for (auto systems : _systems)
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
        for (auto systems : _systems)
        {
            for (auto system : systems)
            {
                system->onEntityChanged(entity);
            }
        }
    }

    void iEntityScene::addSystem(iEntitySystemPtr system)
    {
        auto &systems = _systems[(int)system->getStage()];
        con_assert(std::find(systems.begin(), systems.end(), system) == systems.end(), "system already added");
        systems.push_back(system);

        system->_scene = this;

        for (const auto &pair : _entities)
        {
            system->onEntityChanged(pair.second);
        }

        iCameraSystem *cameraSystem = dynamic_cast<iCameraSystem *>(system);
        if (cameraSystem != nullptr)
        {
            _cameraSystem = cameraSystem;
        }
    }

    void iEntityScene::removeSystem(iEntitySystemPtr system)
    {
        auto &systems = _systems[(int)system->getStage()];
        auto iter = std::find(systems.begin(), systems.end(), system);
        if (iter == systems.end())
        {
            return;
        }

        systems.erase(iter);
        system->_scene = nullptr;

        if (system == _cameraSystem)
        {
            _cameraSystem = nullptr;
        }
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

    void to_json(json &j, const iEntityScene &scene)
    {
        j["name"] = scene.getName();
        j["sceneID"] = scene.getID();

        json entitiesJson = json::array();
        for (const auto &pair : scene._entities)
        {
            json entityJson = *pair.second;
            entitiesJson.push_back(entityJson);
        }

        j["entities"] = entitiesJson;
    }

    void from_json(const json &j, iEntityScene &scene)
    {
    }

} // igor
