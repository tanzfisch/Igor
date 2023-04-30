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
            iEntity entity(_registry.create(), scene);
            auto &component = entity.addComponent<iBaseEntityComponent>();
            component._name = name;
            if (active)
            {
                entity.addComponent<iActiveComponent>();
            }
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

    template const std::vector<iEntityID> &iEntityScene::getEntitiesV2<iVelocityComponent, iTransformComponent, iActiveComponent>();
    template const std::vector<iEntityID> &iEntityScene::getEntitiesV2<iTransformComponent>();
    template const std::vector<iEntityID> &iEntityScene::getEntitiesV2<iTransformComponent, iQuadtreeComponent>();
    template const std::vector<iEntityID> &iEntityScene::getEntitiesV2<iSpriteRendererComponent, iTransformComponent, iActiveComponent>();
    template const std::vector<iEntityID> &iEntityScene::getEntitiesV2<iBehaviourComponent, iActiveComponent>();

    iEntityScene::iEntityScene()
    {
        _impl = new iEntitySceneImpl();

        /*generate_all_combinations<std::tuple<iBaseEntityComponent,
                                             iActiveComponent,
                                             iSpriteRendererComponent,
                                             iTransformComponent,
                                             iVelocityComponent>>();*/

        /*generate_combination<iVelocityComponent, iTransformComponent, iActiveComponent>();
        generate_combination<iTransformComponent>();
        generate_combination<iSpriteRendererComponent, iTransformComponent, iActiveComponent>();
        generate_combination<iBehaviourComponent, iActiveComponent>();                                             */
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
        removeFromQuadtree(entityID);
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

    template <typename... Components>
    const std::vector<iEntityID> &iEntityScene::getEntitiesV2()
    {
        std::type_index key = getTypeIndex<Components...>();

        // TODO need to cache it and need to figure out when the update the cache. for now always overwrite
        // if (_entityIDCache.find(key) == _entityIDCache.end())
        {
            auto view = getRegistry().view<Components...>();
            std::vector<iEntityID> entities;
            for (auto entityID : view)
            {
                entities.emplace_back(entityID);
            }
            _entityIDCache[key] = entities;
        }

        return _entityIDCache[key];
    }

    iTransformComponent &iEntityScene::addTransformComponent(iEntityID entityID, const iaVector3d &position, const iaVector3d &orientation, const iaVector3d &scale)
    {
        return getRegistry().emplace_or_replace<iTransformComponent>(entityID, position, orientation, scale);
    }

    iSpriteRendererComponent &iEntityScene::addSpriteRendererComponent(iEntityID entityID, iTexturePtr texture, const iaColor4f &color, int32 zIndex)
    {
        return getRegistry().emplace_or_replace<iSpriteRendererComponent>(entityID, texture, color, zIndex);
    }

    iVelocityComponent &iEntityScene::addVelocityComponent(iEntityID entityID, const iaVector3d &velocity, const iaVector3d &angularVelocity)
    {
        return getRegistry().emplace_or_replace<iVelocityComponent>(entityID, velocity, angularVelocity);
    }

    void iEntityScene::addToQuadtree(iEntityID entityID, float64 radius)
    {
        iTransformComponent *transform = tryGetComponent<iTransformComponent>(entityID);
        if (transform == nullptr)
        {
            const iaVector2d center = getQuadtree().getRootBox().getCenter();
            transform = &(getRegistry().emplace_or_replace<iTransformComponent>(entityID, iaVector3d(center._x, center._y, 0.0)));
        }

        iQuadtreeComponent &component = getRegistry().emplace_or_replace<iQuadtreeComponent>(entityID, nullptr);
        component._object = std::make_shared<iQuadtreed::Object>(iaCircled(transform->_position._x, transform->_position._y, radius), entityID);
        _impl->getQuadtree().insert(component._object);
    }

    void iEntityScene::removeFromQuadtree(iEntityID entityID)
    {
        iQuadtreeComponent *component = tryGetComponent<iQuadtreeComponent>(entityID);
        if (component == nullptr)
        {
            return;
        }

        _impl->getQuadtree().remove(component->_object);
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
    T &iEntityScene::getComponent(iEntityID entityID)
    {
        return getRegistry().get<T>(entityID);
    }

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

    template void iEntityScene::removeComponent<iBaseEntityComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iBehaviourComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iActiveComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iSpriteRendererComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iTransformComponent>(iEntityID entityID);
    template void iEntityScene::removeComponent<iVelocityComponent>(iEntityID entityID);

} // igor
