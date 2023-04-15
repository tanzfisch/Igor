#include <igor/entities/iEntityScene.h>

#include <igor/entities/iEntity.h>

#include <igor/entities/systems/iSpriteRenderSystem.h>

namespace igor
{

    iEntityScene::iEntityScene()
    {
        _renderingSystems.push_back(std::make_unique<iSpriteRenderSystem>());
    }

    iEntity iEntityScene::createEntity(const iaString &name, bool active)
    {
        iEntity entity(_registry.create(), shared_from_this());
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

    void iEntityScene::clear()
    {
        _systems.clear();
        _registry.clear();
    }

    entt::registry& iEntityScene::getRegistry()
    {
        return _registry;
    }

} // igor
