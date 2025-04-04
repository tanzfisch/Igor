// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlTexture.h"

UserControlTexture::UserControlTexture(iResourceID resourceID, const iWidgetPtr parent)
    : UserControlResource(resourceID, parent)
{
}

UserControlTexture::~UserControlTexture()
{
}

void UserControlTexture::updateResource()
{
    // TODO?
}

void UserControlTexture::update()
{
    UserControlResource::update();

    iTexturePtr texture = iResourceManager::getInstance().loadResource<iTexture>(getResourceID());

    std::wstringstream stream;
    stream << "width: " << texture->getWidth() << "px\n";
    stream << "height: " << texture->getHeight() << "px\n";
    stream << "byte/pixel: " << texture->getBpp() << "\n";
    stream << "color format: " << texture->getColorFormat() << "\n";
    stream << "build mode: " << texture->getBuildMode() << "\n";
    stream << "mip levels: " << texture->getMipMapLevels() << "\n";
    stream << "wrap mode: " << texture->getWrapMode();

    _infoLabel->setText(iaString(stream.str().c_str()));
    _picture->setTexture(texture);
}

void UserControlTexture::init()
{
    UserControlResource::init();

    iWidgetGroupBoxPtr group = new iWidgetGroupBox(getLayout());
    group->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    group->setVerticalAlignment(iVerticalAlignment::Top);
    group->setHeaderOnly();
    group->setText("Texture");

    iWidgetBoxLayoutPtr layout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, group);
    layout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    layout->setVerticalAlignment(iVerticalAlignment::Top);
    layout->setStretchIndex(1);

    _infoLabel = new iWidgetLabel(layout);
    _infoLabel->setHorizontalAlignment(iHorizontalAlignment::Left);

    _picture = new iWidgetPicture(layout);
    _picture->setCheckerBoard(true);
    _picture->setHorizontalAlignment(iHorizontalAlignment::Center);
    _picture->setMaxSize(256, 256);
}
