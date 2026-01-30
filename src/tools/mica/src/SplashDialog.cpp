// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include "SplashDialog.h"

SplashDialog::SplashDialog()
{
    onInitUI();
}

void SplashDialog::onInitUI()
{
    setTitle("Splash");
    setZValue(0);
    setDockable(false);
    setMoveable(false);
    setHeaderEnabled(false);
    setHorizontalAlignment(iHorizontalAlignment::Center);
    setVerticalAlignment(iVerticalAlignment::Center);
    setAcceptOutOfBoundsClicks();
    setIgnoreChildEventConsumption();

    getMouseOffClickEvent().add(iMouseOffClickDelegate(this, &SplashDialog::onCloseEvent));
    getClickEvent().add(iClickDelegate(this, &SplashDialog::onCloseEvent));

    iWidgetBoxLayoutPtr layout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, this);
    layout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    layout->setVerticalAlignment(iVerticalAlignment::Stretch);

    iWidgetPicturePtr logo = new iWidgetPicture(layout);
    logo->setTexture("igor_texture_logo");
    logo->setMaxSize(300,300);

    iWidgetBoxLayoutPtr layoutVert = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, layout);
    layoutVert->setMinWidth(300);
    layoutVert->setSpacing(10);
    layoutVert->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    layoutVert->setVerticalAlignment(iVerticalAlignment::Center);

    auto labelMica = new iWidgetLabel(layoutVert);
    labelMica->setText("Mica");
    labelMica->setMinWidth(100);
    labelMica->setHorizontalAlignment(iHorizontalAlignment::Left);

    iWidgetBoxLayoutPtr versionLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, layoutVert);
    auto labelVersion = new iWidgetLabel(versionLayout);
    labelVersion->setText("Igor ver.:");
    labelVersion->setMinWidth(80);
    labelVersion->setHorizontalAlignment(iHorizontalAlignment::Left);
    auto labelVersionValue = new iWidgetLabel(versionLayout);
    labelVersionValue->setText(IGOR_VERSION_FULL);
}

void SplashDialog::onCloseEvent(iWidgetPtr source)
{
    close();
}