// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlComponentScript.h"

#include <igor/entities/components/iScriptComponent.h>

UserControlComponentBehaviour::UserControlComponentBehaviour(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "Script", parent)
{
}

void UserControlComponentBehaviour::onInit()
{
    UserControlComponent::onInit();

    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetBoxLayoutPtr viewportLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    viewportLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    viewportLayout->setStretchIndex(1);
    iWidgetLabelPtr labelViewport = new iWidgetLabel(viewportLayout);
    labelViewport->setText("Viewport");
    labelViewport->setMinWidth(MICA_REGULAR_LABEL_SIZE);
    labelViewport->setHorizontalAlignment(iHorizontalAlignment::Left);


}

void UserControlComponentBehaviour::onUpdateUI()
{


    _ignoreUpdate = false;
}

void UserControlComponentBehaviour::onUpdateComponent()
{
    if (_ignoreUpdate)
    {
        return;
    }

}

void UserControlComponentBehaviour::onDestroyComponent(iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");
    
    entity->destroyComponent<iScriptComponent>();
}
