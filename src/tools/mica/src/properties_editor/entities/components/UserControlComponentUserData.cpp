// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlComponentUserData.h"

#include <igor/entities/components/iUserDataComponent.h>
#include <igor/data/iAny.h>

static const std::vector<iaString> s_selectionTexts = {
    toString(iAnyType::Bool),
    toString(iAnyType::int64),
    toString(iAnyType::float64),
    toString(iAnyType::iaString),
    toString(iAnyType::iaVector2d),
    toString(iAnyType::iaVector3d),
    toString(iAnyType::iaVector4d)};

static const std::vector<iAnyType> s_selectionTypes = {
    iAnyType::Bool,
    iAnyType::int64,
    iAnyType::float64,
    iAnyType::iaString,
    iAnyType::iaVector2d,
    iAnyType::iaVector3d,
    iAnyType::iaVector4d};

UserControlComponentUserData::UserControlComponentUserData(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "User Data", parent)
{
    _typeSelectionDialog = std::make_unique<iDialogDecisionBox>();
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
    _typeSelectionDialog->open(iDialogCloseDelegate(this, &UserControlComponentUserData::onDialogClosed),
                               "Component Type Selection", "Select component type to add to entity", s_selectionTexts);
}

void UserControlComponentUserData::onDialogClosed(iDialogPtr source)
{
    if (_typeSelectionDialog->getReturnState() != iDialogReturnState::Ok)
    {
        return;
    }

    int selectedIndex = _typeSelectionDialog->getSelection();

    const auto type = s_selectionTypes[selectedIndex];

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

    auto parameters = component->getData();
    // TODO add field with generated name component->add
    
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
        typeLabel->setText(toString(pair.second.getType()));
        typeLabel->setMinWidth(MICA_REGULAR_LABEL_SIZE);

        iWidgetPtr dataWidget = nullptr;

        switch (pair.second.getType())
        {
        case iAnyType::Bool:
        {
            auto widget = new iWidgetCheckBox(parameterLayout);
            widget->setChecked(pair.second.getValue<bool>());
            dataWidget = widget;
            break;
        }

        case iAnyType::uint8:
        {
            auto widget = new iWidgetLineTextEdit(parameterLayout);
            widget->setText(iaString::toString(pair.second.getValue<uint8>()));
            dataWidget = widget;
            break;
        }

        case iAnyType::int8:
        {
            auto widget = new iWidgetLineTextEdit(parameterLayout);
            widget->setText(iaString::toString(pair.second.getValue<int8>()));
            break;
        }

        case iAnyType::uint16:
        {
            auto widget = new iWidgetLineTextEdit(parameterLayout);
            widget->setText(iaString::toString(pair.second.getValue<int8>()));
            dataWidget = widget;
            break;
        }

        case iAnyType::int16:
        {
            auto widget = new iWidgetLineTextEdit(parameterLayout);
            widget->setText(iaString::toString(pair.second.getValue<int8>()));
            dataWidget = widget;
            break;
        }

        case iAnyType::uint32:
        {
            auto widget = new iWidgetLineTextEdit(parameterLayout);
            widget->setText(iaString::toString(pair.second.getValue<int8>()));
            dataWidget = widget;
            break;
        }

        case iAnyType::int32:
        {
            auto widget = new iWidgetLineTextEdit(parameterLayout);
            widget->setText(iaString::toString(pair.second.getValue<int8>()));
            dataWidget = widget;
            break;
        }

        case iAnyType::uint64:
        {
            auto widget = new iWidgetLineTextEdit(parameterLayout);
            widget->setText(iaString::toString(pair.second.getValue<int8>()));
            dataWidget = widget;
            break;
        }

        case iAnyType::int64:
        {
            auto widget = new iWidgetLineTextEdit(parameterLayout);
            widget->setText(iaString::toString(pair.second.getValue<int8>()));
            dataWidget = widget;
            break;
        }

        case iAnyType::float32:
            break;
        case iAnyType::float64:
            break;
        case iAnyType::std_string:
            break;
        case iAnyType::iaString:
            break;
        case iAnyType::iaVector2f:
            break;
        case iAnyType::iaVector2d:
            break;
        case iAnyType::iaVector2i:
            break;
        case iAnyType::iaVector2I:
            break;
        case iAnyType::iaVector3f:
            break;
        case iAnyType::iaVector3d:
            break;
        case iAnyType::iaVector3i:
            break;
        case iAnyType::iaVector3I:
            break;
        case iAnyType::iaVector4f:
            break;
        case iAnyType::iaVector4d:
            break;
        case iAnyType::iaVector4i:
            break;
        case iAnyType::iaVector4I:
            break;
        };

        if (dataWidget != nullptr)
        {
            dataWidget->setUserData(pair.second.getType());
        }

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