#include <igor/entities/iEntityScene.h>

#include <igor/entities/iEntity.h>

#include <igor/entities/systems/iSpriteRenderSystem.h>
#include <igor/entities/systems/iTransformHierarchySystem.h>
#include <igor/entities/systems/iBehaviourSystem.h>
#include <igor/entities/systems/iVelocitySystem.h>
#include <igor/entities/systems/iQuadtreeSystem.h>

#include <utility>
#include <tuple>

namespace igor
{

    class iEntitySceneImpl
    {
    public:
        iEntitySceneImpl()
        {
            _velocitySystem = std::make_shared<iVelocitySystem>();
            _systems.push_back(_velocitySystem);
            _systems.push_back(std::make_shared<iTransformHierarchySystem>());
            _systems.push_back(std::make_shared<iQuadtreeSystem>());
            _systems.push_back(std::make_shared<iBehaviourSystem>());

            _renderingSystems.push_back(std::make_shared<iSpriteRenderSystem>());
        }

        ~iEntitySceneImpl()
        {
            if (_quadtree != nullptr)
            {
                delete _quadtree;
            }
        }

        void setBounds(const iAABoxd &box)
        {
            _velocitySystem->setBounds(box);
        }

        const iAABoxd &getBounds() const
        {
            return _velocitySystem->getBounds();
        }

        iEntity createEntity(const iaString &name, bool active, iEntityScenePtr scene)
        {
            iEntityID entityID = _registry.create();
            _registry.emplace_or_replace<iBaseEntityComponent>(entityID, name);

            iEntity entity(entityID, scene);
            entity.setActive(active);

            return entity;
        }

        void destroyEntity(iEntityID entityID)
        {
            // cleanup hierarchy
            iHierarchyComponent *hierarchy = _registry.try_get<iHierarchyComponent>(entityID);
            if (hierarchy != nullptr &&
                _registry.valid(hierarchy->_parent))
            {
                iHierarchyComponent *parentHierarchy = _registry.try_get<iHierarchyComponent>(hierarchy->_parent);
                if (parentHierarchy != nullptr)
                {
                    parentHierarchy->_childCount = std::max(0, parentHierarchy->_childCount - 1);
                }
            }

            _registry.destroy(entityID);
        }

        void clear()
        {
            _systems.clear();
            _registry.clear();
        }

        entt::registry &getRegistry()
        {
            return _registry;
        }

        void initializeQuadtree(const iaRectangled &box, const uint32 splitThreshold, const uint32 maxDepth)
        {
            con_assert(_quadtree == nullptr, "Quadtree already initialized");

            _quadtree = new iQuadtreed(box, splitThreshold, maxDepth);
        }

        iQuadtreed &getQuadtree() const
        {
            con_assert(_quadtree != nullptr, "Quadtree not initialized");
            return *_quadtree;
        }

        void onUpdate(iEntityScenePtr scene)
        {
            for (iEntitySystemPtr &system : _systems)
            {
                system->update(scene);
            }
        }

        void onRender(iEntityScenePtr scene)
        {
            for (iEntitySystemPtr &system : _renderingSystems)
            {
                system->update(scene);
            }
        }

    private:
        /*! the entt registry
         */
        entt::registry _registry;

        /*! quadtree
         */
        iQuadtreed *_quadtree = nullptr;

        std::shared_ptr<iVelocitySystem> _velocitySystem;

        /*! systems to update
         */
        std::vector<iEntitySystemPtr> _systems;

        /*! systems that render
         */
        std::vector<iEntitySystemPtr> _renderingSystems;
    };

    iEntityScene::iEntityScene()
    {
        _impl = new iEntitySceneImpl();
    }

    iEntityScene::~iEntityScene()
    {
        delete _impl;
    }

    iEntity iEntityScene::createEntity(const iaString &name, bool active)
    {
        return _impl->createEntity(name, active, shared_from_this());
    }

    void iEntityScene::destroyEntity(const iEntity &entity)
    {
        _impl->destroyEntity(entity.getID());
    }

    void iEntityScene::destroyEntity(iEntityID entityID)
    {
        iBody2DComponent *component = tryGetComponent<iBody2DComponent>(entityID);
        if (component != nullptr)
        {
            _impl->getQuadtree().remove(component->_object);
        }

        _impl->destroyEntity(entityID);
    }

    void iEntityScene::clear()
    {
        _impl->clear();
    }

    entt::registry &iEntityScene::getRegistry() const
    {
        return _impl->getRegistry();
    }

    void iEntityScene::onUpdate()
    {
        _impl->onUpdate(shared_from_this());
    }

    void iEntityScene::onRender()
    {
        _impl->onRender(shared_from_this());
    }

    template <typename... Components>
    std::type_index getTypeIndex()
    {
        return std::type_index(typeid(std::tuple<Components...>));
    }

    void iEntityScene::initializeQuadtree(const iaRectangled &box, const uint32 splitThreshold, const uint32 maxDepth)
    {
        _impl->initializeQuadtree(box, splitThreshold, maxDepth);
    }

    iQuadtreed &iEntityScene::getQuadtree() const
    {
        return _impl->getQuadtree();
    }

    void iEntityScene::setBounds(const iAABoxd &box)
    {
        _impl->setBounds(box);
    }

    const iAABoxd &iEntityScene::getBounds() const
    {
        return _impl->getBounds();
    }

    template <typename T>
    T &iEntityScene::addComponent(iEntityID entityID, const T &component)
    {
        T &result = getRegistry().emplace_or_replace<T>(entityID);
        result = component;

        return result;
    }

    template <>
    iBody2DComponent &iEntityScene::addComponent<iBody2DComponent>(iEntityID entityID, const iBody2DComponent &component)
    {
        iTransformComponent *transform = tryGetComponent<iTransformComponent>(entityID);
        if (transform == nullptr)
        {
            const iaVector2d center = getQuadtree().getRootBox().getCenter();
            transform = &(getRegistry().emplace_or_replace<iTransformComponent>(entityID, iaVector3d(center._x, center._y, 0.0)));
        }

        iBody2DComponent &result = getRegistry().emplace_or_replace<iBody2DComponent>(entityID);
        // result = component;

        result._object = std::make_shared<iQuadtreed::Object>(iaCircled(transform->_position._x, transform->_position._y, 1.0), entityID);
        _impl->getQuadtree().insert(result._object);

        return result;
    }

    // the following types are left out on purpose
    // iBaseEntityComponent, iActiveComponent
    template iCircleCollision2DComponent &iEntityScene::addComponent<iCircleCollision2DComponent>(iEntityID entityID, const iCircleCollision2DComponent &component);
    template iBehaviourComponent &iEntityScene::addComponent<iBehaviourComponent>(iEntityID entityID, const iBehaviourComponent &component);
    template iSpriteRendererComponent &iEntityScene::addComponent<iSpriteRendererComponent>(iEntityID entityID, const iSpriteRendererComponent &component);
    template iTransformComponent &iEntityScene::addComponent<iTransformComponent>(iEntityID entityID, const iTransformComponent &component);
    template iVelocityComponent &iEntityScene::addComponent<iVelocityComponent>(iEntityID entityID, const iVelocityComponent &component);

    template <typename T>
    T &iEntityScene::getComponent(iEntityID entityID)
    {
        return getRegistry().get<T>(entityID);
    }

    template iCircleCollision2DComponent &iEntityScene::getComponent<iCircleCollision2DComponent>(iEntityID entityID);
    template iBaseEntityComponent &iEntityScene::getComponent<iBaseEntityComponent>(iEntityID entityID);
    template iBehaviourComponent &iEntityScene::getComponent<iBehaviourComponent>(iEntityID entityID);
    template iActiveComponent &iEntityScene::getComponent<iActiveComponent>(iEntityID entityID);
    template iSpriteRendererComponent &iEntityScene::getComponent<iSpriteRendererComponent>(iEntityID entityID);
    template iTransformComponent &iEntityScene::getComponent<iTransformComponent>(iEntityID entityID);
    template iVelocityComponent &iEntityScene::getComponent<iVelocityComponent>(iEntityID entityID);

    template <typename T>
    T *iEntityScene::tryGetComponent(iEntityID entityID)
    {
        return getRegistry().try_get<T>(entityID);
    }

    template iCircleCollision2DComponent *iEntityScene::tryGetComponent<iCircleCollision2DComponent>(iEntityID entityID);
    template iBaseEntityComponent *iEntityScene::tryGetComponent<iBaseEntityComponent>(iEntityID entityID);
    template iBehaviourComponent *iEntityScene::tryGetComponent<iBehaviourComponent>(iEntityID entityID);
    template iActiveComponent *iEntityScene::tryGetComponent<iActiveComponent>(iEntityID entityID);
    template iSpriteRendererComponent *iEntityScene::tryGetComponent<iSpriteRendererComponent>(iEntityID entityID);
    template iTransformComponent *iEntityScene::tryGetComponent<iTransformComponent>(iEntityID entityID);
    template iVelocityComponent *iEntityScene::tryGetComponent<iVelocityComponent>(iEntityID entityID);

    template <typename T>
    void iEntityScene::removeComponent(iEntityID entityID)
    {
        getRegistry().remove<T>(entityID);
    }

    template <>
    void iEntityScene::removeComponent<iBody2DComponent>(iEntityID entityID)
    {
        iBody2DComponent *component = tryGetComponent<iBody2DComponent>(entityID);
        if (component == nullptr)
        {
            return;
        }

        _impl->getQuadtree().remove(component->_object);

        getRegistry().remove<iBody2DComponent>(entityID);
    }

    // the following types are left out on purpose
    // iBaseEntityComponent
    template void iEntityScene::removeComponent<iCircleCollision2DComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iBehaviourComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iActiveComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iSpriteRendererComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iTransformComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iVelocityComponent>(iEntityID entityID);

} // igor
