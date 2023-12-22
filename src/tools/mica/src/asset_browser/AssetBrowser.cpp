// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "AssetBrowser.h"

#include "UserControlResourceIcon.h"

AssetBrowser::AssetBrowser()
{
    _itemData = std::make_unique<iItemData>();

    initUI();
}

void AssetBrowser::initUI()
{
    setTitle("AssetBrowser");

    setDockable(true);
    setMinWidth(100);
    setMinHeight(100);

    auto vboxLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);
    vboxLayout->setVerticalAlignment(iVerticalAlignment::Stretch);
    vboxLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    vboxLayout->setStretchIndex(1);

    auto buttonLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal);
    vboxLayout->addWidget(buttonLayout);

    _showFilesButton = new iWidgetButton();
    _showFilesButton->setCheckable(true);
    _showFilesButton->setMinSize(30, 30);
    _showFilesButton->setTexture("igor_icon_files");
    _showFilesButton->registerOnClickEvent(iClickDelegate(this, &AssetBrowser::onClickShowAssetsButton));
    updateContentModeButton();
    buttonLayout->addWidget(_showFilesButton);

    auto splitter = new iWidgetSplitter(false);
    splitter->setOrientation(iSplitterOrientation::Vertical);
    splitter->setVerticalAlignment(iVerticalAlignment::Stretch);
    splitter->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    splitter->setRatio(0.1);
    vboxLayout->addWidget(splitter);

    _treeView = new iUserControlTreeView();
    _treeView->setMinWidth(150);
    _treeView->setVerticalAlignment(iVerticalAlignment::Stretch);
    _treeView->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _treeView->getClickEvent().add(iClickDelegate(this, &AssetBrowser::onClickTreeView));
    splitter->addWidget(_treeView);

    iWidgetScrollPtr scroll = new iWidgetScroll();
    _gridView = new iWidgetFixedGridLayout();
    _gridView->setVerticalAlignment(iVerticalAlignment::Top);
    _gridView->setHorizontalAlignment(iHorizontalAlignment::Left);
    _gridView->setCellSize(iaVector2f(150, 150));
    _gridView->registerOnSelectionChangedEvent(iSelectionChangedDelegate(this, &AssetBrowser::onSelectionChanged));

    scroll->addWidget(_gridView);
    splitter->addWidget(scroll);

    _updateHandle.getEventTimerTick().add(iTimerTickDelegate(this, &AssetBrowser::update));
    _updateHandle.setInterval(iaTime::fromMilliseconds(5000));
    _updateHandle.start();
}

void AssetBrowser::onSelectionChanged(const iWidgetPtr source)
{
    auto selection = source->getSelection();
    if(selection.empty())
    {
        return;
    }

    // only care about first for now
    UserControlResourceIcon* icon = static_cast<UserControlResourceIcon*>(selection[0]);
    iResourceID id = icon->getResourceID();
    
    _resourceSelectionChanged(id);
}

void AssetBrowser::onClickShowAssetsButton(iWidgetPtr source)
{
    if (_contentMode == ContentMode::Assets)
    {
        _contentMode = ContentMode::Files;
    }
    else
    {
        _contentMode = ContentMode::Assets;
    }

    updateContentModeButton();
    _updateHandle.triggerNow();
}

void AssetBrowser::updateContentModeButton()
{
    _showFilesButton->setChecked(_contentMode == ContentMode::Files);
}

void AssetBrowser::updateGridView(iItemPtr item)
{
    _gridView->clear();

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

void AssetBrowser::refreshGridView()
{
    for (auto child : _gridView->getChildren())
    {
        UserControlResourceIcon *icon = static_cast<UserControlResourceIcon *>(child);
        icon->refresh();
    }
}

void AssetBrowser::onClickTreeView(const iWidgetPtr source)
{
    iItemPtr item = std::any_cast<iItemPtr>(source->getUserData());
    updateGridView(item);
}

void AssetBrowser::update(const iaDirectory &dir, iItemPtr item)
{
    const iaDirectory projectDir(_project->getProjectFolder());
    std::vector<iaDirectory> dirs = dir.getDirectories();

    for (const auto &subDir : dirs)
    {
        iItemPtr child = item->addItem(subDir.getDirectoryName());
        child->setValue<iaString>("displayName", subDir.getDirectoryName());
        child->setValue<iaString>("icon", "igor_icon_folder");
        child->setValue<bool>("isDirectory", true);
        iaString relativePath = iaDirectory::getRelativePath(projectDir.getFullDirectoryName(), subDir.getFullDirectoryName());
        child->setValue<iaString>("relativePath", relativePath);

        update(subDir, child);
    }

    auto files = dir.getFiles();

    for (const auto &file : files)
    {
        const iaString relativePath = iaDirectory::getRelativePath(projectDir.getFullDirectoryName(), file.getFullFileName());
        if (_contentMode == ContentMode::Assets)
        {
            const iResourceID id = iResourceManager::getInstance().getResourceID(relativePath);
            if (id == iResourceID(IGOR_INVALID_ID))
            {
                continue;
            }
        }

        const iaString filename = file.getFileName();

        iItemPtr child = item->addItem(filename);
        child->setValue<iaString>("displayName", filename);
        child->setValue<bool>("isDirectory", false);
        child->setValue<iaString>("relativePath", relativePath);
        child->setValue<uint64>("timeStamp", iaFile::getLastModifiedTime(file.getFullFileName()).getMicroseconds());

        // expecting everything to be already inside the dictionary
        iaUUID uuid = iResourceManager::getInstance().getResourceID(relativePath);
        child->setValue<uint64>("uuid", (uint64)uuid);
    }
}

void AssetBrowser::update(const iaTime &time)
{
    if (_project == nullptr)
    {
        return;
    }

    iItemData *itemData = new iItemData();

    const iaDirectory projectDir(_project->getProjectFolder());
    iItemPtr projectRoot = itemData->addItem(projectDir.getDirectoryName());
    projectRoot->setValue<iaString>("displayName", projectDir.getDirectoryName());
    projectRoot->setValue<iaString>("icon", "igor_icon_folder");
    projectRoot->setValue<bool>("isDirectory", true);
    projectRoot->setValue<iaString>("relativePath", "");
    update(projectDir, projectRoot);

    if (*itemData != *_itemData)
    {
        _itemData = std::unique_ptr<iItemData>(itemData);

        _treeView->setItems(_itemData->getRoot());
        updateGridView(_itemData->getItem(_treeView->getSelectedItemPath()));
    }
    else
    {
        refreshGridView();
    }
}

void AssetBrowser::setProject(iProjectPtr project)
{
    _project = project;

    _updateHandle.triggerNow();
}

iProjectPtr AssetBrowser::getProject() const
{
    return _project;
}

ResourceSelectionChangedEvent& AssetBrowser::getResourceSelectionChangedEvent()
{
    return _resourceSelectionChanged;
}