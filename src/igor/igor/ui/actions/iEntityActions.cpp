#include <igor/ui/actions/iEntityActions.h>

#include <igor/entities/iEntitySystemModule.h>
#include <igor/ui/actions/context/iEntityActionContext.h>

namespace igor
{

    iActionSetEntityActive::iActionSetEntityActive()
        : iAction("igor:set_entity_active")
    {
        setDescription("Activate entity");
    }

    void iActionSetEntityActive::execute(const iActionContext &context)
    {
        const iEntityActionContext *actionContext = static_cast<const iEntityActionContext *>(&context);

        auto scene = iEntitySystemModule::getInstance().getScene(actionContext->getSceneID());
        for (auto entityID : actionContext->getEntities())
        {
            auto entity = scene->getEntity(entityID);
            entity->setActive(true);
        }
    }

    bool iActionSetEntityActive::isCompatible(const iActionContext &context)
    {
        const iEntityActionContext *actionContext = dynamic_cast<const iEntityActionContext *>(&context);
        if (actionContext == nullptr)
        {
            return false;
        }

        if (actionContext->getEntities().empty())
        {
            return false;
        }

        auto scene = iEntitySystemModule::getInstance().getScene(actionContext->getSceneID());
        if (scene == nullptr)
        {
            return false;
        }

        return true;
    }

    iActionSetEntityInactive::iActionSetEntityInactive()
        : iAction("igor:set_entity_inactive")
    {
        setDescription("Deactivate entity");
    }

    void iActionSetEntityInactive::execute(const iActionContext &context)
    {
        const iEntityActionContext *actionContext = static_cast<const iEntityActionContext *>(&context);

        auto scene = iEntitySystemModule::getInstance().getScene(actionContext->getSceneID());
        for (auto entityID : actionContext->getEntities())
        {
            auto entity = scene->getEntity(entityID);
            entity->setActive(false);
        }
    }

    bool iActionSetEntityInactive::isCompatible(const iActionContext &context)
    {
        const iEntityActionContext *actionContext = dynamic_cast<const iEntityActionContext *>(&context);
        if (actionContext == nullptr)
        {
            return false;
        }

        if (actionContext->getEntities().empty())
        {
            return false;
        }

        auto scene = iEntitySystemModule::getInstance().getScene(actionContext->getSceneID());
        if (scene == nullptr)
        {
            return false;
        }

        return true;
    }

    iActionDeleteEntity::iActionDeleteEntity()
        : iAction("igor:delete_entity")
    {
        setIcon("igor_icon_delete");
        setDescription("Delete entity");
    }

    void iActionDeleteEntity::execute(const iActionContext &context)
    {
        const iEntityActionContext *actionContext = static_cast<const iEntityActionContext *>(&context);

        auto scene = iEntitySystemModule::getInstance().getScene(actionContext->getSceneID());
        for (auto entityID : actionContext->getEntities())
        {
            scene->destroyEntity(entityID);
        }
    }

    bool iActionDeleteEntity::isCompatible(const iActionContext &context)
    {
        const iEntityActionContext *actionContext = dynamic_cast<const iEntityActionContext *>(&context);
        if (actionContext == nullptr)
        {
            return false;
        }

        if (actionContext->getEntities().empty())
        {
            return false;
        }

        auto scene = iEntitySystemModule::getInstance().getScene(actionContext->getSceneID());
        if (scene == nullptr)
        {
            return false;
        }

        return true;
    }

} // namespace igor