// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "AssetBrowser.h"

AssetBrowser::AssetBrowser()
{
    initGUI();
}

void AssetBrowser::initGUI()
{
    setTitle("AssetBrowser");

    setDockable(true);
    setMinWidth(100);
    setMinHeight(100);

    auto boxLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, this);
    boxLayout->addWidget(new iUserControlTreeView());
}
