// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlResourceSprite.h"

UserControlResourceSprite::UserControlResourceSprite(iResourceID resourceID, const iWidgetPtr parent)
    : UserControlResource(resourceID, parent)
{
}

UserControlResourceSprite::~UserControlResourceSprite()
{
}

void UserControlResourceSprite::onUpdateResource()
{
    iSpritePtr sprite = iResourceManager::getInstance().loadResource<iSprite>(getResourceID());

    auto id = _texture->getID();
    if (id.isValid())
    {
        sprite->setTexture(iResourceManager::getInstance().loadResource<iTexture>(id));
    }

    iResourceManager::getInstance().saveResource(getResourceID());
}

void UserControlResourceSprite::onUpdateTexture(const iWidgetPtr source)
{
    onUpdateResource();
}

void UserControlResourceSprite::onUpdateUI()
{
    UserControlResource::onUpdateUI();

    iSpritePtr sprite = iResourceManager::getInstance().loadResource<iSprite>(getResourceID());
    if (sprite->getTexture() != nullptr)
    {
        _texture->setID(sprite->getTexture()->getID());
    }
    else
    {
        _texture->setID(iResourceID::getInvalid());
    }
}

void UserControlResourceSprite::onInit()
{
    UserControlResource::onInit();

    iWidgetGroupBoxPtr group = new iWidgetGroupBox(getLayout());
    group->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    group->setVerticalAlignment(iVerticalAlignment::Top);
    group->setHeaderOnly();
    group->setText("Sprite");

    iWidgetBoxLayoutPtr layout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, group);
    layout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    layout->setVerticalAlignment(iVerticalAlignment::Top);

    _texture = new iUserControlTexture(layout);
    _texture->setHorizontalAlignment(iHorizontalAlignment::Center);
    _texture->setMinSize(256, 256);
    _texture->getChangeEvent().add(iChangeDelegate(this, &UserControlResourceSprite::onUpdateTexture));
}
