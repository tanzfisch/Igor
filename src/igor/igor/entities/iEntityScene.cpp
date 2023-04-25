#include <igor/entities/iEntityScene.h>

#include <igor/entities/iEntity.h>

#include <igor/entities/systems/iSpriteRenderSystem.h>
#include <igor/entities/systems/iTransformHierarchySystem.h>
#include <igor/entities/systems/iBehaviourSystem.h>
#include <igor/entities/systems/iVelocitySystem.h>

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
        _impl->destroyEntity(entityID);
    }

    void iEntityScene::clear()
    {
        _impl->clear();
    }

    entt::registry &iEntityScene::getRegistry()
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


} // igor
