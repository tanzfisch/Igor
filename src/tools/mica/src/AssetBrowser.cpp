// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "AssetBrowser.h"

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

    _gridView = new iUserControlGridView();
    splitter->addWidget(_gridView);
}

void AssetBrowser::onClickTreeView(const iWidgetPtr source)
{
    iItemPtr item = std::any_cast<iItemPtr>(source->getUserData());
    _gridView->setItems(item);
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

void AssetBrowser::update()
{
    update(_projectDir, &_root);
    _treeView->setItems(&_root);
}

void AssetBrowser::setProjectDir(const iaString &projectDir)
{
    _projectDir = projectDir;

    update();
}

const iaString AssetBrowser::getProjectDir() const
{
    return _projectDir.getFullDirectoryName();
}