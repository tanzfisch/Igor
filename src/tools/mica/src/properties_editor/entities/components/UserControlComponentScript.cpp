// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlComponentScript.h"

#include <igor/entities/components/iScriptComponent.h>

UserControlComponentScript::UserControlComponentScript(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "Script", parent)
{
}

void UserControlComponentScript::onInit()
{
    UserControlComponent::onInit();

    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetBoxLayoutPtr scriptLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    scriptLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    scriptLayout->setStretchIndex(1);

    iWidgetButtonPtr editButton = new iWidgetButton(scriptLayout);
    editButton->setText("edit");
    editButton->setMinWidth(MICA_REGULAR_LABEL_SIZE);
    editButton->setHorizontalAlignment(iHorizontalAlignment::Left);

    new iUserControlScript(scriptLayout);
}

void UserControlComponentScript::onUpdateUI()
{


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
