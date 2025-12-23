// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlComponentScript.h"

#include <igor/entities/components/iScriptComponent.h>

UserControlComponentScript::UserControlComponentScript(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "Script", parent)
{
    setAcceptDrop(true);
}

void UserControlComponentScript::onInit()
{
    UserControlComponent::onInit();

    setHorizontalAlignment(iHorizontalAlignment::Stretch);
}

void UserControlComponentScript::onUpdateUI()
{
    _layout->clear();

    iEntityScenePtr scene = iEntitySystemModule::getInstance().getScene(_sceneID);
    if (scene == nullptr)
    {
        return;
    }

    iEntityPtr entity = scene->getEntity(_entityID);
    if (entity == nullptr)
    {
        return;
    }

    iScriptComponentPtr component = entity->getComponent<iScriptComponent>();
    if (component == nullptr)
    {
        return;
    }

    if (component->getScripts().empty())
    {
        new iWidgetSpacer(100, 40, true, _layout);
    }
    else
    {
        for (const auto &script : component->getScripts())
        {
            iWidgetBoxLayoutPtr scriptLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
            iWidgetButtonPtr editButton = new iWidgetButton(scriptLayout);
            editButton->setText("edit");
            editButton->setMinWidth(MICA_REGULAR_LABEL_SIZE);
            editButton->setVerticalAlignment(iVerticalAlignment::Top);
            editButton->setHorizontalAlignment(iHorizontalAlignment::Left);

            auto userControlScript = new iUserControlScript(scriptLayout);
            userControlScript->setID(script._script->getID());
        }
    }

    _ignoreUpdate = false;
}

void UserControlComponentScript::onUpdateComponent()
{
    if (_ignoreUpdate)
    {
        return;
    }
}

void UserControlComponentScript::onDestroyComponent(iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");

    entity->destroyComponent<iScriptComponent>();
}

void UserControlComponentScript::onDragMove(iDrag &drag, const iaVector2f &mousePos)
{
    const iMimeData &mimeData = drag.getMimeData();
    if (!mimeData.hasResourceID())
    {
        drag.reject();
        return;
    }

    iResourceID id = mimeData.getResourceID();

    const iaString resourceType = iResourceManager::getInstance().getType(id);
    if (resourceType == IGOR_RESOURCE_SCRIPT)
    {
        drag.accept();
        return;
    }

    drag.reject();
}

void UserControlComponentScript::onDrop(const iDrag &drag, const iaVector2f &mousePos)
{
    const iMimeData &mimeData = drag.getMimeData();
    if (!mimeData.hasResourceID())
    {
        return;
    }

    iResourceID resourceID = mimeData.getResourceID();
    iScriptPtr script = iResourceManager::getInstance().loadResource<iScript>(resourceID);
    if (script == nullptr)
    {
        return;
    }

    iEntityScenePtr scene = iEntitySystemModule::getInstance().getScene(_sceneID);
    if (scene == nullptr)
    {
        return;
    }

    iEntityPtr entity = scene->getEntity(_entityID);
    if (entity == nullptr)
    {
        return;
    }

    iScriptComponentPtr component = entity->getComponent<iScriptComponent>();
    if (component == nullptr)
    {
        return;
    }

    component->addScript(script);
    onUpdateUI();
    refresh();
}