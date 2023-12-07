// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlResourceIcon.h"

#include "../actions/MicaActionContext.h"
#include "../ThumbnailCache.h"

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

    _spacer = new iWidgetSpacer(128,128, true);
    _spacer->setBackground(iaColor4f(0.0,0.0,0.0,0.2));
    _picture->addWidget(_spacer);

    iWidgetPicturePtr dictPicture = new iWidgetPicture();
    dictPicture->setVerticalAlignment(iVerticalAlignment::Center);
    dictPicture->setHorizontalAlignment(iHorizontalAlignment::Center);
    dictPicture->setMinSize(90, 90);
    dictPicture->setMaxSize(90, 90);
    dictPicture->setTexture("igor_icon_no_dictionary");
    _spacer->addWidget(dictPicture);

    _label = new iWidgetLabel();
    _label->setHorizontalAlignment(iHorizontalAlignment::Center);
    vBoxLayout->addWidget(_label);
}

void UserControlResourceIcon::onAddDictionary(iWidgetPtr source)
{
    iResourceManager::getInstance().addResource(_filename);
    _spacer->setVisible(false);
}

void UserControlResourceIcon::onRemoveDictionary(iWidgetPtr source)
{
    const iResourceID id = iResourceManager::getInstance().getResourceID(_filename);
    iResourceManager::getInstance().removeResource(id);
    _spacer->setVisible(true);
}

void UserControlResourceIcon::OnContextMenu(iWidgetPtr source)
{
    _contextMenu.clear();

    iaVector2i pos = iMouse::getInstance().getPos();
    _contextMenu.setPos(iaVector2f(pos._x, pos._y));

    const iResourceID id = iResourceManager::getInstance().getResourceID(_filename);
    if (id != iResourceID(IGOR_INVALID_ID))
    {
        _contextMenu.addCallback(iClickDelegate(this, &UserControlResourceIcon::onRemoveDictionary), "Unregister Asset", "Remove asset from resource dictionary", "igor_icon_no_dictionary");
    }
    else
    {
        _contextMenu.addCallback(iClickDelegate(this, &UserControlResourceIcon::onAddDictionary), "Register Asset", "Add asset to resource dictionary", "igor_icon_dictionary");
    }

    _contextMenu.open();
}

void UserControlResourceIcon::refresh()
{
    iaFile file(iResourceManager::getInstance().resolvePath(_filename));
    iTexturePtr texture = ThumbnailCache::getInstance().getThumbnail(file.getFullFileName());

    if (texture == nullptr)
    {
        return;
    }

    _picture->setTexture(texture);
}

void UserControlResourceIcon::setFilename(const iaString &filename)
{
    _filename = filename;
    const iResourceID id = iResourceManager::getInstance().getResourceID(_filename);
    const iaString type = iResourceManager::getInstance().getType(_filename);

    _spacer->setVisible(id == iResourceID(IGOR_INVALID_ID));

    iaFile file(iResourceManager::getInstance().resolvePath(_filename));
    setTooltip(file.getFullFileName());
    _label->setText(file.getFileName());
    _label->setMaxTextWidth(128);

    iTexturePtr texture = ThumbnailCache::getInstance().getThumbnail(file.getFullFileName());

    if (texture == nullptr)
    {
        if (type == "texture")
        {
            texture = iResourceManager::getInstance().requestResource<iTexture>("igor_icon_file_texture");
        }
        else if (type == "material")
        {
            texture = iResourceManager::getInstance().requestResource<iTexture>("igor_icon_file_material");
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
        }
        else if (type == "model")
        {
            texture = iResourceManager::getInstance().requestResource<iTexture>("igor_icon_file_model");
        }
        else
        {
            texture = iResourceManager::getInstance().requestResource<iTexture>("igor_icon_file");
        }
    }

    _picture->setTexture(texture);
}
