// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlResourceIcon.h"

UserControlResourceIcon::UserControlResourceIcon(const iWidgetPtr parent)
    : iUserControl(iWidgetType::iUserControl, parent)
{
    setGrowingByContent(false);

    initGUI();
}

void UserControlResourceIcon::initGUI()
{
    iWidgetBoxLayoutPtr vBoxLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);
    _picture = new iWidgetPicture();
    _picture->setMinSize(128, 128);

    vBoxLayout->addWidget(_picture);
    _label = new iWidgetLabel();
    vBoxLayout->addWidget(_label);
}

void UserControlResourceIcon::setFilename(const iaString &filename)
{
    iResourceID id = iResourceManager::getInstance().getResourceID(filename);

    iaString toolTip;
    toolTip += filename;
    toolTip += "\n";
    toolTip += id.toString();
    setTooltip(toolTip);

    _label->setText("label text");
    _picture->setTexture(iResourceManager::getInstance().getResource<iTexture>("igor_fallback_texture"));
}