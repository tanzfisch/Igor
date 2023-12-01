// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "AssetBrowser.h"

#include "usercontrols/UserControlResourceIcon.h"

AssetBrowser::AssetBrowser()
{
    initUI();
}

void AssetBrowser::initUI()
{
    setTitle("AssetBrowser");

    setDockable(true);
    setMinWidth(100);
    setMinHeight(100);

    auto splitter = new iWidgetSplitter(false, this);
    splitter->setOrientation(iSplitterOrientation::Vertical);
    splitter->setVerticalAlignment(iVerticalAlignment::Stretch);
    splitter->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    splitter->setRatio(0.1);

    _treeView = new iUserControlTreeView();
    _treeView->setMinWidth(150);
    _treeView->setVerticalAlignment(iVerticalAlignment::Stretch);
    _treeView->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _treeView->getClickEvent().add(iClickDelegate(this, &AssetBrowser::onClickTreeView));
    splitter->addWidget(_treeView);

    iWidgetScrollPtr scroll = new iWidgetScroll();
    _gridView = new iWidgetFixedGridLayout();
    _gridView->setCellSize(iaVector2f(150, 150));

    scroll->addWidget(_gridView);
    splitter->addWidget(scroll);

    _updateHandle.getEventTimerTick().add(iTimerTickDelegate(this, &AssetBrowser::update));
    _updateHandle.setInterval(iaTime::fromMilliseconds(10000));
    _updateHandle.start();
}

void AssetBrowser::onClickTreeView(const iWidgetPtr source)
{
    iItemPtr item = std::any_cast<iItemPtr>(source->getUserData());

    _gridView->clearChildren();

    for (const auto child : item->getItems())
    {
        if (!child->hasValue("isDirectory"))
        {
            continue;
        }

        bool isDirectory = child->getValue<bool>("isDirectory");
        if (isDirectory)
        {
            continue;
        }

        if (!child->hasValue("relativePath"))
        {
            continue;
        }

        const iaString relativePath = child->getValue<iaString>("relativePath");

        UserControlResourceIcon *icon = new UserControlResourceIcon(_gridView);
        icon->setFilename(relativePath);
    }
}

void AssetBrowser::update(const iaDirectory &dir, iItemPtr item)
{
    std::vector<iaDirectory> dirs = dir.getDirectories();

    for (const auto &subDir : dirs)
    {
        iItemPtr child = item->addItem();
        child->setValue<iaString>("displayName", subDir.getDirectoryName());
        child->setValue<iaString>("icon", "igor_icon_folder");
        child->setValue<bool>("isDirectory", true);
        iaString relativePath = iaDirectory::getRelativePath(_projectDir.getFullDirectoryName(), subDir.getFullDirectoryName());
        child->setValue<iaString>("relativePath", relativePath);

        update(subDir, child);
    }

    auto files = dir.getFiles();

    for (const auto &file : files)
    {
        iItemPtr child = item->addItem();
        child->setValue<iaString>("displayName", file.getFileName());
        child->setValue<bool>("isDirectory", false);

        iaString relativePath = iaDirectory::getRelativePath(_projectDir.getFullDirectoryName(), file.getFullFileName());
        child->setValue<iaString>("relativePath", relativePath);

        // expecting everything to be already inside the dictionary
        iaUUID uuid = iResourceManager::getInstance().getResourceID(relativePath);
        child->setValue<uint64>("uuid", (uint64)uuid);
    }
}

void AssetBrowser::update(const iaTime &time)
{
    _root.clear();

    if (_projectDir.exists())
    {
        iItemPtr root = _root.addItem();
        root->setValue<iaString>("displayName", _projectDir.getDirectoryName());
        root->setValue<iaString>("icon", "igor_icon_folder");
        root->setValue<bool>("isDirectory", true);
        root->setValue<iaString>("relativePath", "");

        update(_projectDir, root);
    }

    _treeView->setItems(&_root);
}

void AssetBrowser::setProjectFolder(const iaString &projectDir)
{
    _projectDir = projectDir;

    _updateHandle.triggerNow();
}

const iaString AssetBrowser::getProjectFolder() const
{
    return _projectDir.getFullDirectoryName();
}