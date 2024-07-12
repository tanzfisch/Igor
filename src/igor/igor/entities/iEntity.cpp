// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntity.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntitySystemModule.h>

#include <igor/entities/components/iTransformComponent.h>
#include <igor/entities/components/iCameraComponent.h>
#include <igor/entities/components/iCircleCollision2DComponent.h>
#include <igor/entities/components/iLightComponent.h>
#include <igor/entities/components/iMeshRenderComponent.h>
#include <igor/entities/components/iOctreeComponent.h>
#include <igor/entities/components/iQuadtreeComponent.h>
#include <igor/entities/components/iSphereCollision3DComponent.h>
#include <igor/entities/components/iSpriteRenderComponent.h>

#include <igor/entities/components/iComponents.h>

#include <algorithm>

namespace igor
{

    iEntity::iEntity(const iaString &name)
        : _name(name)
    {
    }

    iEntity::~iEntity()
    {
        removeParent();

        const auto children = _children;
        for (auto child : children)
        {
            child->removeParent();
            _scene->destroyEntity(child);
        }

        const auto inactiveChildren = _inactiveChildren;
        for (auto child : inactiveChildren)
        {
            child->removeParent();
            _scene->destroyEntity(child);
        }

        clearComponents();
    }

    void iEntity::componentToAdd(const std::type_index &typeID)
    {
        _scene->onComponentToAdd(this, typeID);
    }

    void iEntity::processComponents()
    {
        if (!isActive() ||
            _addedComponents.empty())
        {
            return;
        }

        for (const auto &pair : _addedComponents)
        {
            if (pair.second->onLoad(this))
            {
                pair.second->_state = iEntityComponentState::Loaded;
                pair.second->onActivate(this);
                pair.second->_state = iEntityComponentState::Active;
            }
            else
            {
                pair.second->_state = iEntityComponentState::LoadFailed;
                con_err("load of component \"" << pair.second->getName() << "\" type(" << pair.first.name() << ") failed");
            }

            _scene->onComponentAdded(this, pair.first);
        }

        _addedComponents.clear();

        _componentMask = calcComponentMask();

        onEntityChanged();
    }

    void iEntity::destroyComponent(const std::type_index &typeID)
    {
        _scene->onComponentToRemove(this, typeID);

        auto iter = _components.find(typeID);
        if (iter == _components.end())
        {
            con_err("trying to remove component that does not exist");
            return;
        }

        if (iter->second->_state == iEntityComponentState::Active)
        {
            iter->second->onDeactivate(this);
            iter->second->onUnLoad(this);
        }

        delete iter->second;
        _components.erase(iter);

        _componentMask = calcComponentMask();

        _scene->onComponentRemoved(this, typeID);
    }

    void iEntity::onEntityChanged()
    {
        _scene->onEntityChanged(this);
    }

    void iEntity::clearComponents()
    {
        const auto components = _components;
        for (const auto &pair : components)
        {
            destroyComponent(pair.first);
        }

        _components.clear();

        onEntityChanged();
    }

    const iEntityID &iEntity::getID() const
    {
        return _id;
    }

    const iaString &iEntity::getName() const
    {
        return _name;
    }

    void iEntity::addBehaviour(const iBehaviourDelegate &delegate, const std::any &userData, const iaString &name)
    {
        iBehaviourComponent *behaviourComponent = getComponent<iBehaviourComponent>();
        if (behaviourComponent == nullptr)
        {
            behaviourComponent = static_cast<iBehaviourComponent *>(addComponent(new iBehaviourComponent()));
        }

        behaviourComponent->_behaviors.push_back({delegate, userData, name});
    }

    void iEntity::removeBehaviour(const iBehaviourDelegate &delegate)
    {
        iBehaviourComponent *behaviourComponent = getComponent<iBehaviourComponent>();

        if (behaviourComponent == nullptr)
        {
            con_err("no behaviour component available");
            return;
        }

        auto &behaviors = behaviourComponent->_behaviors;

        auto iter = std::find_if(behaviors.begin(), behaviors.end(), [delegate](const iBehaviourData &behaviourData)
                                 { return behaviourData._delegate == delegate; });

        if (iter == behaviors.end())
        {
            con_err("can't remove given behavior");
            return;
        }

        behaviors.erase(iter);

        if (!behaviors.empty())
        {
            return;
        }

        destroyComponent<iBehaviourComponent>();
    }

    iEntityScenePtr iEntity::getScene() const
    {
        return _scene;
    }

    void iEntity::setParent(iEntityPtr parent)
    {
        con_assert_sticky(parent != nullptr, "null pointer");
        con_assert_sticky(parent->getScene() == getScene(), "incompatible scene");

        removeParent();

        _parent = parent;
        _parent->_children.push_back(this);
    }

    void iEntity::setParent(const iEntityID &parentID)
    {
        removeParent();

        iEntityPtr parent = _scene->getEntity(parentID);
        if (parent == nullptr)
        {
            con_err("can't find parent with id:" << parentID);
            return;
        }

        _parent = parent;
        _parent->_children.push_back(this);
    }

    void iEntity::removeParent()
    {
        if (_parent == nullptr)
        {
            return;
        }

        auto &children = _parent->_children;
        auto iter = std::find(children.begin(), children.end(), this);
        if (iter != children.end())
        {
            children.erase(iter);
        }

        _parent = _scene->_root;
    }

    iEntityPtr iEntity::getParent() const
    {
        if (_parent == _scene->_root || _parent == nullptr)
        {
            return nullptr;
        }

        return _parent;
    }

    bool iEntity::hasParent() const
    {
        return _parent != _scene->_root && _parent != nullptr;
    }

    const std::vector<iEntityPtr> &iEntity::getChildren() const
    {
        return _children;
    }

    const std::vector<iEntityPtr> &iEntity::getInactiveChildren() const
    {
        return _inactiveChildren;
    }

    void iEntity::setActive(bool active)
    {
        if (_active == active)
        {
            return;
        }

        _active = active;

        if (hasParent())
        {
            if (!_active)
            {
                auto &children = _parent->_children;
                auto iter = std::find(children.begin(), children.end(), this);
                if (iter != children.end())
                {
                    children.erase(iter);
                    _parent->_inactiveChildren.push_back(this);
                }
            }
            else
            {
                auto &inactiveChildren = _parent->_inactiveChildren;
                auto iter = std::find(inactiveChildren.begin(), inactiveChildren.end(), this);
                if (iter != inactiveChildren.end())
                {
                    inactiveChildren.erase(iter);
                    _parent->_children.push_back(this);
                }
            }
        }

        if (_active)
        {
            // activate components
            for (const auto &pair : _components)
            {
                if (pair.second->_state == iEntityComponentState::Loaded ||
                    pair.second->_state == iEntityComponentState::Inactive)
                {
                    pair.second->onActivate(this);
                    pair.second->_state = iEntityComponentState::Active;
                }
            }

            // activate children
            auto children = getInactiveChildren(); // make copy
            for (auto child : children)
            {
                child->setActive(_active);
            }
        }
        else
        {
            // deactivate components
            for (const auto &pair : _components)
            {
                if (pair.second->_state == iEntityComponentState::Active)
                {
                    pair.second->onDeactivate(this);
                    pair.second->_state = iEntityComponentState::Inactive;
                }
            }

            // deactivate children
            auto children = getChildren(); // make copy
            for (auto child : children)
            {
                child->setActive(_active);
            }
        }

        onEntityChanged();
    }

    bool iEntity::isActive() const
    {
        return _active;
    }

    iEntityComponentMask iEntity::calcComponentMask(const std::vector<std::type_index> &types)
    {
        iEntityComponentMask result = 0;
        auto &esm = iEntitySystemModule::getInstance();

        for (const auto &type : types)
        {
            result |= esm.getComponentMask(type);
        }

        return result;
    }

    iEntityComponentMask iEntity::calcComponentMask()
    {
        iEntityComponentMask result = 0;
        auto &esm = iEntitySystemModule::getInstance();

        // adding types so the order does not matter
        for (const auto &pair : _components)
        {
            if (pair.second->_state == iEntityComponentState::Active)
            {
                result |= esm.getComponentMask(pair.first);
            }
        }

        return result;
    }

    iEntityComponentMask iEntity::getComponentMask() const
    {
        return _componentMask;
    }

    bool iEntity::isHierarchyDirty() const
    {
        return _dirtyHierarchy;
    }

    void iEntity::setDirtyHierarchy(bool dirty)
    {
        _dirtyHierarchy = dirty;

        if (_dirtyHierarchy)
        {
            if (hasParent())
            {
                getParent()->setDirtyHierarchyUp();
            }

            for (uint32 i = 0; i < _children.size(); ++i)
            {
                _children[i]->setDirtyHierarchyDown();
            }
        }
    }

    void iEntity::setDirtyHierarchyUp()
    {
        if (!_dirtyHierarchy)
        {
            _dirtyHierarchy = true;

            if (hasParent())
            {
                getParent()->setDirtyHierarchyUp();
            }
        }
    }

    void iEntity::setDirtyHierarchyDown()
    {
        _dirtyHierarchy = true;

        for (auto child : _children)
        {
            child->setDirtyHierarchyDown();
        }
    }

    bool iEntity::isRoot() const
    {
        return _scene->_root == this;
    }

    void to_json(json &j, const iEntity &entity)
    {
        j["name"] = entity.getName();
        j["entityID"] = entity.getID();
        j["active"] = entity.isActive();
        if (entity.hasParent())
        {
            j["parent"] = entity.getParent()->getID();
        }

        json childrenJson = json::array();
        const auto &activeChildren = entity.getChildren();
        for (const auto child : activeChildren)
        {
            childrenJson.push_back(child->getID());
        }

        const auto &inactiveChildren = entity.getInactiveChildren();
        for (const auto child : inactiveChildren)
        {
            childrenJson.push_back(child->getID());
        }

        if (!childrenJson.empty())
        {
            j["children"] = childrenJson;
        }

        json componentsJson = json::array();
        for (const auto &pair : entity._components)
        {
            iTransformComponent *transform = dynamic_cast<iTransformComponent *>(pair.second);
            if (transform != nullptr)
            {
                componentsJson.push_back({"transform", *transform});
                continue;
            }

            iCameraComponent *camera = dynamic_cast<iCameraComponent *>(pair.second);
            if (camera != nullptr)
            {
                componentsJson.push_back( {"camera", *camera});
                continue;
            }            

            iSphereCollision3DComponent *sphereCollision = dynamic_cast<iSphereCollision3DComponent *>(pair.second);
            if (sphereCollision != nullptr)
            {
                componentsJson.push_back({"sphere", *sphereCollision});
                continue;
            }    

            iOctreeComponent *octree = dynamic_cast<iOctreeComponent *>(pair.second);
            if (octree != nullptr)
            {
                componentsJson.push_back({"octree", *octree});
                continue;
            }             

            iMeshRenderComponent *meshRender = dynamic_cast<iMeshRenderComponent *>(pair.second);
            if (meshRender != nullptr)
            {
                componentsJson.push_back({"meshRender", *meshRender});
                continue;
            }  

            con_err("unknown component type");
        }

        if (!componentsJson.empty())
        {
            j["components"] = componentsJson;
        }
    }

    void from_json(const json &j, iEntity &entity)
    {
    }

}