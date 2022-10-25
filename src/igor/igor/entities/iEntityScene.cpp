#include <igor/entities/iEntityScene.h>

#include <igor/entities/iEntity.h>

namespace igor
{

    iEntity iEntityScene::createEntity(const iaString &name)
    {
        iEntity entity(_registry.create(), *this);
        auto &component = entity.addComponent<iNameComponent>();
        component._name = name.isEmpty() ? L"Entity" : name.getData();
        return entity;
    }

    void iEntityScene::destroyEntity(const iEntity &entity)
    {
        _registry.destroy(entity.getID());
    }

    void iEntityScene::destroyEntity(const iEntityID entityID)
    {        
         _registry.destroy(entityID);
    }

    void iEntityScene::clear()
    {
        _registry.clear();
    }

} // igor
