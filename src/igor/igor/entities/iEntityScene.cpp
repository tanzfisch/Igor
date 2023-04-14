#include <igor/entities/iEntityScene.h>

#include <igor/entities/iEntity.h>

namespace igor
{

    iEntity iEntityScene::createEntity(const iaString &name, bool active)
    {
        iEntity entity(_registry.create(), *this);
        auto &component = entity.addComponent<iBaseEntityComponent>();
        component._name = name;
        component._active = active;
        return entity;
    }

    void iEntityScene::destroyEntity(iEntity entity)
    {
        _registry.destroy(entity.getID());
    }

    void iEntityScene::destroyEntity(iEntityID entityID)
    {        
         _registry.destroy(entityID);
    }

    void iEntityScene::registerSystem(iEntitySystemPtr system)
    {
        _systems.push_back(std::move(system));
    }

    void iEntityScene::clear()
    {
        _systems.clear();
        _registry.clear();
    }

    void iEntityScene::updateSystems()
    {
        for (iEntitySystemPtr &system : _systems)
        {
            system->update(this);
        }
    }

    entt::registry& iEntityScene::getRegistry()
    {
        return _registry;
    }

} // igor
