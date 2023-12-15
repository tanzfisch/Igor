// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
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

    _layout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);
    _layout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _layout->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetBoxLayoutPtr nameLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, _layout);
    nameLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    nameLayout->setStretchIndex(1);
    iWidgetLabelPtr labelName = new iWidgetLabel(nameLayout);
    labelName->setText("Name");
    labelName->setMinWidth(MICA_REGULARBUTTON_SIZE);
    labelName->setHorizontalAlignment(iHorizontalAlignment::Left);

    _textName = new iWidgetLineTextEdit(nameLayout);
    _textName->setMaxTextLength(256);
    _textName->setHorizontalTextAlignment(iHorizontalAlignment::Left);
    _textName->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _textName->setText("1");
    _textName->registerOnChangeEvent(iChangeDelegate(this, &UserControlResource::onNameChanged));

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
    _textSource->setText("2");
    _textSource->registerOnChangeEvent(iChangeDelegate(this, &UserControlResource::onNameChanged));

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
    _textID->setText("3");
    _textID->registerOnChangeEvent(iChangeDelegate(this, &UserControlResource::onNameChanged));
}

iWidgetBoxLayoutPtr UserControlResource::getLayout()
{
    return _layout;
}

void UserControlResource::update()
{
    iMaterialPtr material = iResourceManager::getInstance().getResource<iMaterial>(getResourceID());
    _textName->setText(material->getName());
    _textID->setText(material->getID().toString());
    _textSource->setText(material->getSource());
}

void UserControlResource::updateResource()
{
    iMaterialPtr material = iResourceManager::getInstance().getResource<iMaterial>(getResourceID());
    material->setName(_textName->getText());
}

void UserControlResource::onNameChanged(const iWidgetPtr source)
{
}

iResourceID UserControlResource::getResourceID() const
{
    return _resourceID;
}