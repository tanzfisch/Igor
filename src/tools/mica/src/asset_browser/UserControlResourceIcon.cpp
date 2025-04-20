// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlResourceIcon.h"

#include <igor/resources/texture/iThumbnailCache.h>
#include <iaux/system/iaFile.h>

UserControlResourceIcon::UserControlResourceIcon(const iaString &filename, const iaString &referencePath, const iWidgetPtr parent)
    : iUserControl(iWidgetType::iUserControl, parent), _filename(filename), _referencePath(referencePath)
{
    setGrowingByContent(false);
    setIgnoreChildEventConsumption(true);
    setSelectable(true);
    setAcceptDrag(true);

    initGUI();
    updateUI();
}

void UserControlResourceIcon::initGUI()
{
    registerOnContextMenuEvent(iContextMenuDelegate(this, &UserControlResourceIcon::OnContextMenu));

    iWidgetBoxLayoutPtr vBoxLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);
    vBoxLayout->setHorizontalAlignment(iHorizontalAlignment::Center);
    vBoxLayout->setVerticalAlignment(iVerticalAlignment::Top);

    _picture = new iWidgetPicture(vBoxLayout);
    _picture->setHorizontalAlignment(iHorizontalAlignment::Center);
    _picture->setMaxSize(128, 128);

    _dictPicture = new iWidgetPicture(_picture);
    _dictPicture->setVerticalAlignment(iVerticalAlignment::Center);
    _dictPicture->setHorizontalAlignment(iHorizontalAlignment::Center);
    _dictPicture->setMinSize(90, 90);
    _dictPicture->setMaxSize(90, 90);
    _dictPicture->setTexture("igor_icon_no_dictionary");

    _labelFilename = new iWidgetLabel(vBoxLayout);
    _labelFilename->setHorizontalAlignment(iHorizontalAlignment::Center);
    _labelFilename->setMaxTextWidth(128);

    _labelReferencePath = new iWidgetLabel(vBoxLayout);
    _labelReferencePath->setHorizontalAlignment(iHorizontalAlignment::Center);
}

iResourceID UserControlResourceIcon::getResourceID() const
{
    return _resourceID;
}

void UserControlResourceIcon::onAddDictionary(iWidgetPtr source)
{
    iResourceManager::getInstance().addToDictionary(_filename);
    updateUI();
}

void UserControlResourceIcon::onRemoveDictionary(iWidgetPtr source)
{
    const iResourceID id = iResourceManager::getInstance().getResourceID(_filename);
    iResourceManager::getInstance().removeFromDictionary(id);
    updateUI();
}

void UserControlResourceIcon::OnContextMenu(iWidgetPtr source)
{
    _contextMenu.clear();

    _contextMenu.setPos(iMouse::getInstance().getPos());

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

void UserControlResourceIcon::onRefresh()
{
    iaFile file(iResourceManager::getInstance().resolvePath(_filename));
    iTexturePtr texture = iThumbnailCache::getInstance().getThumbnail(file.getFullFileName());

    if (texture == nullptr)
    {
        return;
    }

    _picture->setTexture(texture);
}

void UserControlResourceIcon::updateDictionaryState()
{
    if (_resourceID.isValid())
    {
        _picture->setForeground(iaColor4f::white);
        _dictPicture->setVisible(false);
    }
    else
    {
        _dictPicture->setVisible(true);
        _picture->setForeground(iaColor4f(1.0, 1.0, 1.0, 0.5));
    }
}

void UserControlResourceIcon::updateUI()
{
    _resourceID = iResourceManager::getInstance().getResourceID(_filename);
    const iaString type = iResourceManager::getInstance().getType(_filename);

    updateDictionaryState();

    iaFile file(iResourceManager::getInstance().resolvePath(_filename));
    setTooltip(file.getFullFileName());
    _labelFilename->setText(file.getFileName());    
    _labelReferencePath->setText(_referencePath);

    iTexturePtr texture = iThumbnailCache::getInstance().getThumbnail(file.getFullFileName());

    if (texture == nullptr)
    {
        if (type == "texture")
        {
            texture = iResourceManager::getInstance().requestResource<iTexture>("igor_icon_file_texture");
        }
        else if (type == IGOR_RESOURCE_SHADER)
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
        else if (type == "prefab")
        {
            texture = iResourceManager::getInstance().requestResource<iTexture>("igor_icon_prefab");
        }
        else
        {
            texture = iResourceManager::getInstance().requestResource<iTexture>("igor_icon_file");
        }
    }
    else
    {
        _picture->setCheckerBoard(true);
    }

    _picture->setTexture(texture);
}

void UserControlResourceIcon::draw()
{
    if (!isVisible())
    {
        return;
    }

    if (isSelected())
    {
        iWidgetManager::getInstance().getTheme()->drawSelection(getActualRect());
    }

    for (const auto child : _children)
    {
        child->draw();
    }
}

void UserControlResourceIcon::onDrag()
{
    if (!getResourceID().isValid())
    {
        return;
    }

    iDrag drag(this);
    iMimeData mimeData;
    mimeData.setResourceID(getResourceID());
    iaFile file(_filename);
    mimeData.setText(file.getStem());
    drag.setMimeData(mimeData);
    drag.setTexture(_picture->getTexture());
    drag.execute();
}
