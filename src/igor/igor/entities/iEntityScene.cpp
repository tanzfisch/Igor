#include <igor/entities/iEntityScene.h>

#include <igor/entities/iEntity.h>

#include <igor/entities/systems/iSpriteRenderSystem.h>
#include <igor/entities/systems/iTransformHierarchySystem.h>
#include <igor/entities/systems/iBehaviourSystem.h>
#include <igor/entities/systems/iVelocitySystem.h>

#include <utility>
#include <tuple>

namespace igor
{

    class iEntitySceneImpl
    {
    public:
        iEntitySceneImpl()
        {
            _systems.push_back(std::make_unique<iVelocitySystem>());
            _systems.push_back(std::make_unique<iTransformHierarchySystem>());
            _systems.push_back(std::make_unique<iBehaviourSystem>());

            _renderingSystems.push_back(std::make_unique<iSpriteRenderSystem>());
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

        /*! systems to update
         */
        std::vector<iEntitySystemPtr> _systems;

        /*! systems that render
         */
        std::vector<iEntitySystemPtr> _renderingSystems;
    };

    template const std::vector<iEntityID> &iEntityScene::getEntitiesV2<iVelocityComponent, iTransformComponent, iActiveComponent>();
    template const std::vector<iEntityID> &iEntityScene::getEntitiesV2<iTransformComponent>();
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
