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

    iActionCopyEntity::iActionCopyEntity()
        : iAction("igor:copy_entity")
    {
        setIcon("igor_icon_copy");
        setDescription("Copy entities");
    }

    void iActionCopyEntity::execute(const iActionContext &context)
    {
        const iEntityActionContext *actionContext = static_cast<const iEntityActionContext *>(&context);
        std::vector<iaUUID> IDs = { 0 }; // means this is copy and not cut
        IDs.push_back(actionContext->getSceneID());
        IDs.insert(IDs.end(), actionContext->getEntities().begin(), actionContext->getEntities().end());
        iClipboard::getInstance().copyEntityIDs(IDs);
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

    iActionCutEntity::iActionCutEntity()
        : iAction("igor:cut_entity")
    {
        setIcon("igor_icon_cut");
        setDescription("Cut entities");
    }

    void iActionCutEntity::execute(const iActionContext &context)
    {
        const iEntityActionContext *actionContext = static_cast<const iEntityActionContext *>(&context);
        std::vector<iaUUID> IDs = { 1 }; // means this is cut and not copy
        IDs.push_back(actionContext->getSceneID());
        IDs.insert(IDs.end(), actionContext->getEntities().begin(), actionContext->getEntities().end());
        iClipboard::getInstance().copyEntityIDs(IDs);
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

    iActionPasteEntity::iActionPasteEntity()
        : iAction("igor:paste_entity")
    {
        setIcon("igor_icon_paste");
        setDescription("Paste entities");
    }

    void iActionPasteEntity::execute(const iActionContext &context)
    {
        const iEntityActionContext *actionContext = static_cast<const iEntityActionContext *>(&context);

        std::vector<iaUUID> IDs = iClipboard::getInstance().pasteEntityIDs();
        con_assert(IDs.size() >= 3, "invalid data");

        bool makeCopy = !IDs[0].isValid(); // else cut
        auto dstScene = iEntitySystemModule::getInstance().getScene(actionContext->getSceneID());
        auto dstEntity = dstScene->getEntity(actionContext->getEntities()[0]);
        auto srcScene = iEntitySystemModule::getInstance().getScene(IDs[1]);
        for (int i = 2; i < IDs.size(); ++i)
        {
            if(makeCopy)
            {
                auto entity = srcScene->getEntity(IDs[i]);
                iEntitySystemModule::getInstance().insert(entity, dstEntity);
            }
            else
            {
                // TODO iEntitySystemModule::getInstance().move from srcScene:entity to dstScene:entity
            }
        }
    }

    bool iActionPasteEntity::isCompatible(const iActionContext &context)
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

    iActionCreateEntity::iActionCreateEntity()
        : iAction("igor:create_new_entity")
    {
        setDescription("Create new entity");
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


} // namespace igor