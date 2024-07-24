// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntity.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntitySystemModule.h>

#include <algorithm>

namespace igor
{

    iEntity::iEntity(const iaString &name)
        : _name(name)
    {
    }

    std::unordered_map<std::type_index, iEntityComponentPtr> iEntity::getComponents()
    {
        std::unordered_map<std::type_index, iEntityComponentPtr> result;
        _mutex.lock();
        result = _components;
        _mutex.unlock();

        return result;
    }

    iEntityComponentPtr iEntity::getComponent(const std::type_index &typeID) const
    {
        auto iter = _components.find(typeID);
        if (iter == _components.end())
        {
            return nullptr;
        }

        return iter->second;
    }

    void iEntity::setName(const iaString &name)
    {
        _name = name;
    }

    void iEntity::addComponent(const std::type_index &typeID, iEntityComponentPtr component)
    {
        _mutex.lock();
        auto iter = _components.find(typeID);
        if (iter != _components.end())
        {
            con_err("component already exists");
            _mutex.unlock();
            return;
        }

        _components[typeID] = component;
        component->_entity = this;

        _addedComponents.emplace_back(typeID, component);
        _mutex.unlock();

        componentToAdd(typeID);
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

    bool iEntity::processComponents()
    {
        if (!isActive() ||
            _addedComponents.empty())
        {
            return true;
        }

        _mutex.lock();
        auto addedComponents = std::move(_addedComponents);
        _mutex.unlock();

        std::vector<std::pair<std::type_index, iEntityComponentPtr>> toKeep;

        bool changed = false;

        for (const auto &pair : addedComponents)
        {
            bool asyncLoad = false;
            bool success = pair.second->onLoad(this, asyncLoad);
            if (success)
            {
                pair.second->_state = iEntityComponentState::Loaded;
                pair.second->onActivate(this);
                pair.second->_state = iEntityComponentState::Active;

                changed = true;
                _scene->onComponentAdded(this, pair.first);
            }
            else
            {
                if (!asyncLoad)
                {
                    pair.second->_state = iEntityComponentState::LoadFailed;
                    con_err("load of component " << pair.first.name() << " failed");
                }
                else
                {
                    // keep in queue
                    toKeep.push_back(pair);
                }
            }
        }

        _mutex.lock();
        _addedComponents.insert(_addedComponents.end(), toKeep.begin(), toKeep.end());
        bool processAgain = !_addedComponents.empty();
        _mutex.unlock();

        if (changed)
        {
            _componentMask = calcComponentMask();
            onEntityChanged();
        }

        return !processAgain;
    }

    void iEntity::destroyComponent(const std::type_index &typeID)
    {
        _scene->onComponentToRemove(this, typeID);

        _mutex.lock();
        auto iter = _components.find(typeID);
        if (iter == _components.end())
        {
            _mutex.unlock();
            con_err("trying to remove component that does not exist");
            return;
        }

        auto component = iter->second;
        _components.erase(iter);
        _mutex.unlock();

        if (component->_state == iEntityComponentState::Active)
        {
            component->onDeactivate(this);
            component->onUnLoad(this);
        }

        delete component;

        _componentMask = calcComponentMask();

        _scene->onComponentRemoved(this, typeID);
    }

    void iEntity::onEntityChanged()
    {
        _scene->onEntityChanged(this);
    }

    void iEntity::clearComponents()
    {
        _mutex.lock();
        const auto components = _components;
        _mutex.unlock();
        for (const auto &pair : components)
        {
            destroyComponent(pair.first);
        }

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

        _mutex.lock();
        for (const auto &pair : _components)
        {
            if (pair.second->_state == iEntityComponentState::Active)
            {
                result |= esm.getComponentMask(pair.first);
            }
        }
        _mutex.unlock();

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
}