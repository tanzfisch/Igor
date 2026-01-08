// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
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

    _scriptLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, _layout);
    _scriptLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
}

void UserControlComponentScript::onUpdateUI()
{
    _scriptLayout->clear();

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
        new iWidgetSpacer(100, 40, true, _scriptLayout);
    }
    else
    {
        for (const auto &script : component->getScripts())
        {
            iWidgetBoxLayoutPtr scriptLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _scriptLayout);
            iWidgetButtonPtr editButton = new iWidgetButton(scriptLayout);
            editButton->setText("edit");
            editButton->setEnabled(script._script->hasSource());
            editButton->setUserData(script._script->getID());
            editButton->setMinWidth(MICA_REGULAR_LABEL_SIZE);
            editButton->setVerticalAlignment(iVerticalAlignment::Top);
            editButton->setHorizontalAlignment(iHorizontalAlignment::Left);
            editButton->getClickEvent().add(iClickDelegate(this, &UserControlComponentScript::onClickEditEvent));

            auto userControlScript = new iUserControlScript(scriptLayout);
            userControlScript->setID(script._script->getID());
        }
    }

    _ignoreUpdate = false;
}

void UserControlComponentScript::onClickEditEvent(iWidgetPtr source)
{
    const auto &resourceID = std::any_cast<iResourceID>(source->getUserData());

    const auto &script = iResourceManager::getInstance().getResource<iScript>(resourceID);
    if(script == nullptr)
    {
        return;
    }

    const auto resolvedPath = iResourceManager::getInstance().resolvePath(script->getSource());

    std::string editor;
    if (iConfig::getInstance().hasValue("mica.scriptEditor"))
    {
        editor = iaString::toStdString(iConfig::getInstance().getValue("mica.scriptEditor"));
    }
    
    std::string command;
    if (!editor.empty()) {
        command = std::string(editor) + " \"" + iaString::toStdString(resolvedPath) + "\"";
    } else {
        command = "xdg-open \"" + iaString::toStdString(resolvedPath) + "\"";
    }

    std::system(command.c_str());
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