#include <igor/ui/actions/iEntityActions.h>

#include <igor/entities/iEntitySystemModule.h>
#include <igor/entities/components/iPrefabComponent.h>
#include <igor/resources/project/iProject.h>

#include <igor/ui/actions/context/iEntityActionContext.h>
#include <igor/system/iClipboard.h>

namespace igor
{

    iActionSetEntityActive::iActionSetEntityActive()
        : iAction("igor:set_entity_active")
    {
        setDescription("Activate", "Activates selected entities");
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

    iActionSetEntityInactive::iActionSetEntityInactive()
        : iAction("igor:set_entity_inactive")
    {
        setDescription("Deactivate", "Deactivates selected entities");
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

    iActionDeleteEntity::iActionDeleteEntity()
        : iAction("igor:delete_entity")
    {
        setIcon("igor_icon_delete");
        setDescription("Delete", "Deletes selected entities");
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

    void iActionDeleteEntity::execute(const iActionContext &context)
    {
        const iEntityActionContext *actionContext = static_cast<const iEntityActionContext *>(&context);

        auto scene = iEntitySystemModule::getInstance().getScene(actionContext->getSceneID());
        for (auto entityID : actionContext->getEntities())
        {
            scene->destroyEntity(entityID);
        }
    }

    iActionCopyEntity::iActionCopyEntity()
        : iAction("igor:copy_entity")
    {
        setIcon("igor_icon_copy");
        setDescription("Copy", "Copies selected entities");
    }

    bool iActionCopyEntity::isCompatible(const iActionContext &context)
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

    void iActionCopyEntity::execute(const iActionContext &context)
    {
        const iEntityActionContext *actionContext = static_cast<const iEntityActionContext *>(&context);
        iEntityScene::copy(actionContext->getSceneID(), actionContext->getEntities());
    }

    iActionCutEntity::iActionCutEntity()
        : iAction("igor:cut_entity")
    {
        setIcon("igor_icon_cut");
        setDescription("Cut", "Cut selected entities");
    }

    bool iActionCutEntity::isCompatible(const iActionContext &context)
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

    void iActionCutEntity::execute(const iActionContext &context)
    {
        const iEntityActionContext *actionContext = static_cast<const iEntityActionContext *>(&context);
        iEntityScene::cut(actionContext->getSceneID(), actionContext->getEntities());
    }

    iActionPasteEntity::iActionPasteEntity()
        : iAction("igor:paste_entity")
    {
        setIcon("igor_icon_paste");
        setDescription("Paste", "Paste selected entities");
    }

    bool iActionPasteEntity::isCompatible(const iActionContext &context)
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

    void iActionPasteEntity::execute(const iActionContext &context)
    {
        const iEntityActionContext *actionContext = static_cast<const iEntityActionContext *>(&context);
        iEntityScene::paste(actionContext->getSceneID(), actionContext->getEntities()[0]);
    }

    iActionCreateEntity::iActionCreateEntity()
        : iAction("igor:create_new_entity")
    {
        setDescription("Create entity", "Creates a new entity");
    }

    bool iActionCreateEntity::isCompatible(const iActionContext &context)
    {
        const iEntityActionContext *actionContext = dynamic_cast<const iEntityActionContext *>(&context);
        if (actionContext == nullptr)
        {
            return false;
        }

        if (actionContext->getEntities().size() != 1)
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

    void iActionCreateEntity::execute(const iActionContext &context)
    {
        const iEntityActionContext *actionContext = static_cast<const iEntityActionContext *>(&context);

        auto projectScene = iEntitySystemModule::getInstance().getScene(actionContext->getSceneID());
        auto dstEntity = projectScene->getEntity(actionContext->getEntities()[0]);

        iEntityPtr entity = projectScene->createEntity("entity");
        entity->setActive(true);
        entity->setParent(dstEntity);
    }

    iActionDuplicateEntity::iActionDuplicateEntity()
        : iAction("igor:duplicate_entity")
    {
        setDescription("Duplicate entity", "Creates a duplicate of given entity");
    }

    bool iActionDuplicateEntity::isCompatible(const iActionContext &context)
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

    void iActionDuplicateEntity::execute(const iActionContext &context)
    {
        const iEntityActionContext *actionContext = static_cast<const iEntityActionContext *>(&context);

        iEntityScene::duplicate(actionContext->getSceneID(), actionContext->getEntities());
    }

} // namespace igor