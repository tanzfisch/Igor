// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlResource.h"

#include "../../MicaDefines.h"

UserControlResource::UserControlResource(iResourceID resourceID, const iWidgetPtr parent)
    : iUserControl(iWidgetType::iUserControl, parent), _resourceID(resourceID)
{
    con_assert(_resourceID.isValid(), "invalid resource id");
}

void UserControlResource::init()
{
    setHorizontalAlignment(iHorizontalAlignment::Stretch);

    iWidgetGroupBox *mainGroupBox = new iWidgetGroupBox(this);
    mainGroupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    mainGroupBox->setText("Resource");
    mainGroupBox->setHeaderOnly();

    _layout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, mainGroupBox);
    _layout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _layout->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetBoxLayoutPtr typeLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    typeLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    typeLayout->setStretchIndex(1);
    iWidgetLabelPtr labelType = new iWidgetLabel(typeLayout);
    labelType->setText("Type");
    labelType->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelType->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textType = new iWidgetLineTextEdit(typeLayout);
    _textType->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textType->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textType->setEnabled(false);

    iWidgetBoxLayoutPtr sourceLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    sourceLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    sourceLayout->setStretchIndex(1);
    iWidgetLabelPtr labelSource = new iWidgetLabel(sourceLayout);
    labelSource->setText("Source");
    labelSource->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelSource->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textSource = new iWidgetLineTextEdit(sourceLayout);
    _textSource->setMaxTextLength(256);
    _textSource->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textSource->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textSource->setEnabled(false);

    iWidgetBoxLayoutPtr idLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    idLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    idLayout->setStretchIndex(1);
    iWidgetLabelPtr labelID = new iWidgetLabel(idLayout);
    labelID->setText("ID");
    labelID->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelID->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textID = new iWidgetLineTextEdit(idLayout);
    _textID->setMaxTextLength(256);
    _textID->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textID->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textID->setEnabled(false);

    iWidgetBoxLayoutPtr aliasLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    aliasLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    aliasLayout->setStretchIndex(1);
    iWidgetLabelPtr labelAlias = new iWidgetLabel(aliasLayout);
    labelAlias->setText("Alias");
    labelAlias->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelAlias->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textAlias = new iWidgetLineTextEdit(aliasLayout);
    _textAlias->setMaxTextLength(256);
    _textAlias->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textAlias->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textAlias->registerOnChangeEvent(iChangeDelegate(this, &UserControlResource::onAliasChanged));    
}

iWidgetBoxLayoutPtr UserControlResource::getLayout()
{
    return _layout;
}

void UserControlResource::update()
{
    _textType->setText(iResourceManager::getInstance().getType(getResourceID()));
    _textAlias->setText(iResourceManager::getInstance().getAlias(getResourceID()));
    _textID->setText(getResourceID().toString());
    _textSource->setText(iResourceManager::getInstance().getFilename(getResourceID()));
}

void UserControlResource::updateResource()
{
}

void UserControlResource::onAliasChanged(const iWidgetPtr source)
{
    iResourceManager::getInstance().setAlias(getResourceID(), _textAlias->getText());
    // TODO #400 iResourceManager::getInstance().saveResourceDictionary();
}

iResourceID UserControlResource::getResourceID() const
{
    return _resourceID;
}