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

/*        for (iEntitySystemPtr &system : _systems)
        {
            system->update(time, shared_from_this());
        }*/
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

    void iEntityScene::addEntity(iEntityPtr entity)
    {
        con_assert(entity != nullptr, "zero pointer");
        con_assert(_entities.find(entity->getID()) != _entities.end(), "entity was already added to scene " << entity->getID());

        _entities[entity->getID()] = entity;
        entity->_scene = this;
    }

    iEntityPtr iEntityScene::removeEntity(iEntityID entityID)
    {
        auto iter = _entities.find(entityID);
        if (iter == _entities.end())
        {
            return nullptr;
        }

        iEntityPtr entity = iter->second;
        _entities.erase(iter);
        entity->_scene = nullptr;

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

} // igor
