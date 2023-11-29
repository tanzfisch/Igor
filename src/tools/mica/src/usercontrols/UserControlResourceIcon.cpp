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
    vBoxLayout->setHorizontalAlignment(iHorizontalAlignment::Center);
    vBoxLayout->setVerticalAlignment(iVerticalAlignment::Top);

    _picture = new iWidgetPicture();
    _picture->setHorizontalAlignment(iHorizontalAlignment::Center);
    _picture->setMinSize(90, 90);
    vBoxLayout->addWidget(_picture);

    _label = new iWidgetLabel();
    _label->setHorizontalAlignment(iHorizontalAlignment::Center);
    vBoxLayout->addWidget(_label);
}

void UserControlResourceIcon::setFilename(const iaString &filename)
{
    const iResourceID id = iResourceManager::getInstance().getResourceID(filename);
    const bool notInDictionary = id == iResourceID(IGOR_INVALID_ID);
    const iaString type = iResourceManager::getInstance().getType(filename);
    iaFile file(filename);

    iaString toolTip;
    toolTip += filename;
    toolTip += "\n";
    toolTip += id.toString();
    setTooltip(toolTip);

    _label->setText(file.getFileName());
    _label->setMaxTextWidth(128);

    iTexturePtr texture;

    con_endl(type);

    if (type == "texture")
    {
        texture = iResourceManager::getInstance().requestResource<iTexture>("igor_icon_file_texture");
        // TODO thumbnail
    }
    else if (type == "material")
    {
        texture = iResourceManager::getInstance().requestResource<iTexture>("igor_icon_file_material");
        // TODO thumbnail
    }
    else if (type == "animation")
    {
        texture = iResourceManager::getInstance().requestResource<iTexture>("igor_icon_file_animation");
    }
    else if (type == "sound")
    {
        texture = iResourceManager::getInstance().requestResource<iTexture>("igor_icon_file_sound");
    }
    else if (type == "sprite")
    {
        texture = iResourceManager::getInstance().requestResource<iTexture>("igor_icon_file_sprite");
        // TODO thumbnail
    }
    else if (type == "model")
    {
        // TODO thumbnail
        texture = iResourceManager::getInstance().requestResource<iTexture>("igor_icon_file_model");
    }
    else
    {
        // TODO thumbnail
        texture = iResourceManager::getInstance().requestResource<iTexture>("igor_icon_file");
    }

    _picture->setTexture(texture);
}
