// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
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

    const std::vector<std::type_index> iEntity::getComponentTypes()
    {
        std::vector<std::type_index> result;
        _mutex.lock();
        for(const auto &pair : _components)
        {
            result.push_back(pair.first);
        }
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
        if (_name == name)
        {
            return;
        }

        _name = name;

        iEntitySystemModule::getInstance().getEntityNameChangedEvent()(this);
    }

    void iEntity::addComponent(const std::type_index &typeID, iEntityComponentPtr component)
    {
        con_assert(component != nullptr, "zero pointer");

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

        _unloadedComponents.emplace_back(typeID, component);
        _mutex.unlock();

        componentToProcess(typeID);
    }

    void iEntity::componentToProcess(const std::type_index &typeID)
    {
        _scene->onComponentToProcess(this, typeID);
    }

    bool iEntity::processComponents()
    {
        if (getName() == "shop")
        {
            int x = 0;
        }

        _mutex.lock();
        auto componentsToProcess = std::move(_unloadedComponents);
        _mutex.unlock();

        if (componentsToProcess.empty())
        {
            return true;
        }

        std::vector<std::pair<std::type_index, iEntityComponentPtr>> remainUnloaded;

        bool changed = false;

        for (const auto &pair : componentsToProcess)
        {
            con_assert(pair.second->_state == iEntityComponentState::Unloaded || pair.second->_state == iEntityComponentState::UnloadedInactive, "invalid state");

            bool asyncLoad = false;
            bool active = pair.second->_state == iEntityComponentState::Unloaded;

            bool success = pair.second->onLoad(this, asyncLoad);
            if (success)
            {
                if (active)
                {
                    pair.second->onActivate(this);
                    pair.second->_state = iEntityComponentState::Active;
                }
                else
                {
                    pair.second->onActivate(this);
                    pair.second->onDeactivate(this);
                    pair.second->_state = iEntityComponentState::Inactive;
                }

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
                    remainUnloaded.push_back(pair);
                }
            }
        }

        const bool processAgain = !remainUnloaded.empty();

        if (processAgain)
        {
            _mutex.lock();
            _unloadedComponents.insert(_unloadedComponents.end(), remainUnloaded.begin(), remainUnloaded.end());
            _mutex.unlock();
        }

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
            component->_state == iEntityComponentState::Inactive;
        }

        if (component->_state == iEntityComponentState::Inactive)
        {
            component->onUnLoad(this);
            component->_state = iEntityComponentState::Unloaded;
        }

        delete component;

        _componentMask = calcComponentMask();

        _scene->onComponentRemoved(this, typeID);
    }

    void iEntity::reloadComponent(const std::type_index &typeID)
    {
        _mutex.lock();
        auto iter = _components.find(typeID);
        if (iter == _components.end())
        {
            _mutex.unlock();
            con_err("trying to reload component that does not exist");
            return;
        }

        auto component = iter->second;
        _mutex.unlock();

        con_assert(component->_state == iEntityComponentState::Active || component->_state == iEntityComponentState::Inactive, "invalid state");

        const bool active = component->_state == iEntityComponentState::Active;

        if (active)
        {
            component->onDeactivate(this);
        }

        component->onUnLoad(this);
        component->_state = active ? iEntityComponentState::Unloaded : iEntityComponentState::UnloadedInactive;

        _mutex.lock();
        _unloadedComponents.emplace_back(typeID, component);
        _mutex.unlock();

        componentToProcess(typeID);
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
        if (parent == nullptr)
        {
            con_err("invalid pointer");
            return;
        }

        if (parent->getScene() != getScene())
        {
            con_err("incompatible scene");
            return;
        }

        removeParent();

        _parent = parent;
        _parent->_children.push_back(this);

        iEntitySystemModule::getInstance().getHierarchyChangedEvent()(_scene);
    }

    void iEntity::setParent(const iEntityID &parentID)
    {
        iEntityPtr parent = _scene->getEntity(parentID);
        setParent(parent);
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
        iEntitySystemModule::getInstance().getHierarchyChangedEvent()(_scene);
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
                if (pair.second->_state == iEntityComponentState::Inactive)
                {
                    pair.second->onActivate(this);
                    pair.second->_state = iEntityComponentState::Active;
                }
                else if (pair.second->_state == iEntityComponentState::UnloadedInactive)
                {
                    pair.second->_state = iEntityComponentState::Unloaded;
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
                else if (pair.second->_state == iEntityComponentState::Unloaded)
                {
                    pair.second->_state = iEntityComponentState::UnloadedInactive;
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