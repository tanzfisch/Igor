// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlResourceIcon.h"

#include "../actions/MicaActionContext.h"

UserControlResourceIcon::UserControlResourceIcon(const iWidgetPtr parent)
    : iUserControl(iWidgetType::iUserControl, parent)
{
    setGrowingByContent(false);
    setIgnoreChildEventConsumption(true);

    initGUI();
}

void UserControlResourceIcon::initGUI()
{
    registerOnContextMenuEvent(iContextMenuDelegate(this, &UserControlResourceIcon::OnContextMenu));

    iWidgetBoxLayoutPtr vBoxLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);
    vBoxLayout->setHorizontalAlignment(iHorizontalAlignment::Center);
    vBoxLayout->setVerticalAlignment(iVerticalAlignment::Top);

    _picture = new iWidgetPicture();
    _picture->setHorizontalAlignment(iHorizontalAlignment::Center);
    _picture->setMinSize(128, 128);
    _picture->setMaxSize(128, 128);
    vBoxLayout->addWidget(_picture);

    _label = new iWidgetLabel();
    _label->setHorizontalAlignment(iHorizontalAlignment::Center);
    vBoxLayout->addWidget(_label);

    _inDictionary = new iWidgetPicture();
    _inDictionary->setVerticalAlignment(iVerticalAlignment::Top);
    _inDictionary->setHorizontalAlignment(iHorizontalAlignment::Left);
    _inDictionary->setBackground(iaColor4f::gray);
    _inDictionary->setMinSize(32, 32);
    _inDictionary->setMaxSize(32, 32);
    _picture->addWidget(_inDictionary);
}

void UserControlResourceIcon::onAddDictionary(iWidgetPtr source)
{
    iResourceManager::getInstance().addResource(_filename);
    _inDictionary->setTexture("igor_icon_dictionary");
}

void UserControlResourceIcon::onRemoveDictionary(iWidgetPtr source)
{
    const iResourceID id = iResourceManager::getInstance().getResourceID(_filename);
    iResourceManager::getInstance().removeResource(id);
    _inDictionary->setTexture("igor_icon_no_dictionary");
}

void UserControlResourceIcon::OnContextMenu(iWidgetPtr source)
{
    _contextMenu.clear();

    iaVector2i pos = iMouse::getInstance().getPos();
    _contextMenu.setPos(iaVector2f(pos._x, pos._y));

    const iResourceID id = iResourceManager::getInstance().getResourceID(_filename);
    if(id != iResourceID(IGOR_INVALID_ID))
    {
        _contextMenu.addCallback(iClickDelegate(this, &UserControlResourceIcon::onRemoveDictionary), "Unregister Asset", "Remove asset from resource dictionary");
    }
    else
    {
        _contextMenu.addCallback(iClickDelegate(this, &UserControlResourceIcon::onAddDictionary), "Register Asset", "Add asset to resource dictionary");
    }

    _contextMenu.open();
}

void UserControlResourceIcon::setFilename(const iaString &filename)
{
    _filename = filename;
    const iResourceID id = iResourceManager::getInstance().getResourceID(filename);
    const bool inDictionary = id != iResourceID(IGOR_INVALID_ID);
    const iaString type = iResourceManager::getInstance().getType(filename);

    if (inDictionary)
    {
        _inDictionary->setTexture("igor_icon_dictionary");
    }
    else
    {
        _inDictionary->setTexture("igor_icon_no_dictionary");
    }

    iaFile file(filename);
    setTooltip(file.getFullFileName());
    _label->setText(file.getFileName());
    _label->setMaxTextWidth(128);

    iTexturePtr texture;

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
        texture = iResourceManager::getInstance().requestResource<iTexture>("igor_icon_file");
    }

    _picture->setTexture(texture);
}
