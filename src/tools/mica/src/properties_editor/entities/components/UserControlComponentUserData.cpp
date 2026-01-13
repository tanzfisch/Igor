// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlComponentUserData.h"

#include <igor/entities/components/iUserDataComponent.h>
#include <igor/utils/iAnyUtil.h>

UserControlComponentUserData::UserControlComponentUserData(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "User Data", parent)
{
}

void UserControlComponentUserData::onInit()
{
    UserControlComponent::onInit();

    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    auto buttonLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);

    iWidgetButtonPtr addButton = new iWidgetButton(buttonLayout);
    addButton->setText("add data field");
    addButton->getClickEvent().add(iClickDelegate(this, &UserControlComponentUserData::onClickAdd));

    _dataLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, _layout);
    _dataLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
}

void UserControlComponentUserData::onClickAdd(iWidgetPtr source)
{
    // TODO
}

void UserControlComponentUserData::emptyUI()
{
    _dataLayout->clear();
    new iWidgetSpacer(100, 40, true, _dataLayout);
}

void UserControlComponentUserData::onUpdateUI()
{
    iEntityScenePtr scene = iEntitySystemModule::getInstance().getScene(_sceneID);
    if (scene == nullptr)
    {
        emptyUI();
        return;
    }

    iEntityPtr entity = scene->getEntity(_entityID);
    if (entity == nullptr)
    {
        emptyUI();
        return;
    }

    iUserDataComponentPtr component = entity->getComponent<iUserDataComponent>();
    if (component == nullptr)
    {
        emptyUI();
        return;
    }

    _ignoreUpdate = true;

    const auto &parameters = component->getData();
    for (const auto &pair : parameters.getData())
    {
        auto parameterLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _dataLayout);

        auto nameTextEdit = new iWidgetTextEdit(parameterLayout);
        nameTextEdit->setText(pair.first);
        nameTextEdit->setMinWidth(MICA_REGULAR_LABEL_SIZE);
        auto typeLabel = new iWidgetLabel(parameterLayout);
        typeLabel->setText(iAnyUtil::toString(pair.second.type()));
        typeLabel->setMinWidth(MICA_REGULAR_LABEL_SIZE);

        const auto type = iAnyUtil::toEnum(pair.second.type());

        switch (type)
        {
        case iAnyUtilType::Bool:
        {
            auto widget = new iWidgetCheckBox(parameterLayout);
            widget->setChecked();
        }
        break;
        case iAnyUtilType::uint8:
            break;
        case iAnyUtilType::int8:
            break;
        case iAnyUtilType::uint16:
            break;
        case iAnyUtilType::int16:
            break;
        case iAnyUtilType::uint32:
            break;
        case iAnyUtilType::int32:
            break;
        case iAnyUtilType::uint64:
            break;
        case iAnyUtilType::int64:
            break;
        case iAnyUtilType::float32:
            break;
        case iAnyUtilType::float64:
            break;
        case iAnyUtilType::std_string:
            break;
        case iAnyUtilType::iaString:
            break;
        case iAnyUtilType::iaVector2f:
            break;
        case iAnyUtilType::iaVector2d:
            break;
        case iAnyUtilType::iaVector2i:
            break;
        case iAnyUtilType::iaVector2I:
            break;
        case iAnyUtilType::iaVector3f:
            break;
        case iAnyUtilType::iaVector3d:
            break;
        case iAnyUtilType::iaVector3i:
            break;
        case iAnyUtilType::iaVector3I:
            break;
        case iAnyUtilType::iaVector4f:
            break;
        case iAnyUtilType::iaVector4d:
            break;
        case iAnyUtilType::iaVector4i:
            break;
        case iAnyUtilType::iaVector4I:
            break;
        };

        auto valueTextEdit = new iWidgetTextEdit(parameterLayout);
    }

    _ignoreUpdate = false;
}

void UserControlComponentUserData::onUpdateComponent()
{
    if (_ignoreUpdate)
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

    iUserDataComponentPtr component = entity->getComponent<iUserDataComponent>();
    if (component == nullptr)
    {
        return;
    }

    // TODO
}

void UserControlComponentUserData::onDestroyComponent(iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");

    entity->destroyComponent<iUserDataComponent>();
}