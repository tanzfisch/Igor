// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntityScene.h>

#include <igor/renderer/iRenderer.h>

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
    }

    void iEntityScene::initializeQuadtree(const iaRectangled &box, const uint32 splitThreshold, const uint32 maxDepth)
    {
        con_assert(_quadtree == nullptr, "Quadtree already initialized");

        _quadtree = new iQuadtreed(box, splitThreshold, maxDepth);
    }

    iQuadtreed &iEntityScene::getQuadtree() const
    {
        con_assert(_quadtree != nullptr, "Quadtree not initialized");
        return *_quadtree;
    }

    bool iEntityScene::hasQuadtree() const
    {
        return _quadtree != nullptr;
    }

    void iEntityScene::onUpdate(const iaTime &time)
    {
        // destroyEntities();

        for (auto system : _systems)
        {
            system->update(time, this);
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

    void iEntityScene::destroyEntity(iEntityID entityID)
    {
        auto iter = _entities.find(entityID);
        if (iter == _entities.end())
        {
            return;
        }

        iter->second->_scene = nullptr;
        delete iter->second;
        _entities.erase(iter);
    }

    const iEntitySceneID &iEntityScene::getID() const
    {
        return _id;
    }

    void iEntityScene::onComponentsChanged(iEntityPtr entity)
    {
        for (auto system : _systems)
        {
            system->onComponentsChanged(entity);
        }
    }

    void iEntityScene::addSystem(iEntitySystemPtr system)
    {
        con_assert(std::find(_systems.begin(), _systems.end(), system) == _systems.end(), "system already added");
        _systems.push_back(system);

        for (const auto &pair : _entities)
        {
            system->onComponentsChanged(pair.second);
        }
    }

    void iEntityScene::removeSystem(iEntitySystemPtr system)
    {
        auto iter = std::find(_systems.begin(), _systems.end(), system);
        if (iter == _systems.end())
        {
            return;
        }

        _systems.erase(iter);
    }

} // igor
