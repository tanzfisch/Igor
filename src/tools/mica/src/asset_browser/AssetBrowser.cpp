// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
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
    _showFilesButton->setTooltip("Show/Hide non registered project files");
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
    _gridView->registerOnContextMenuEvent(iContextMenuDelegate(this, &AssetBrowser::OnContextMenu));
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

void AssetBrowser::OnContextMenu(iWidgetPtr source)
{
    _contextMenu.clear();
    _contextMenu.setPos(iMouse::getInstance().getPos());

    iWidgetMenuPtr createMenu = new iWidgetMenu("Create");
    _contextMenu.addMenu(createMenu);

    createMenu->addCallback(iClickDelegate(this, &AssetBrowser::onCreateMaterial), "Material", "Creates a default material", "");
    createMenu->addCallback(iClickDelegate(this, &AssetBrowser::onCreateShader), "Shader", "Creates a default shader", "");

    _contextMenu.open();
}

void AssetBrowser::onCreateMaterial(iWidgetPtr source)
{
    iMaterialPtr resource = iResourceManager::getInstance().createResource<iMaterial>();
    iResourceManager::getInstance().saveResource(resource, _currentPath + IGOR_PATHSEPARATOR + "new_material.mat");
}

void AssetBrowser::onCreateShader(iWidgetPtr source)
{
    iShaderPtr resource = iResourceManager::getInstance().createResource<iShader>();
    iResourceManager::getInstance().saveResource(resource, _currentPath + IGOR_PATHSEPARATOR + "new_shader.shader");
}

void AssetBrowser::onSelectionChanged(const iWidgetPtr source)
{
    auto selection = source->getSelection();
    if (selection.empty())
    {
        return;
    }

    // only care about first for now
    UserControlResourceIcon *icon = static_cast<UserControlResourceIcon *>(selection[0]);
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

void AssetBrowser::updateGridView(const iaString &relativePath)
{
    _gridView->clear();

    const iaDirectory projectDir(_projectFolder);
    _currentPath = projectDir.getFullParentDirectoryName() + IGOR_PATHSEPARATOR + relativePath;

    const iaDirectory dir(_currentPath);
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
    updateGridView(std::any_cast<iaString>(source->getUserData()));
}

void AssetBrowser::update(const iaDirectory &dir, iItemPtr item)
{
    const iaDirectory projectDir(_projectFolder);
    std::vector<iaDirectory> dirs = dir.getDirectories();

    for (const auto &subDir : dirs)
    {
        iItemPtr child = item->addItem(subDir.getDirectoryName());
        child->setValue<iaString>(IGOR_ITEM_DATA_ICON, "igor_icon_folder");
        iaString relativePath = iaDirectory::getRelativePath(projectDir.getFullDirectoryName(), subDir.getFullDirectoryName());
        child->setValue<iaString>("relativePath", relativePath);

        update(subDir, child);
    }
}

void AssetBrowser::update(const iaTime &time)
{
    if (_projectFolder.isEmpty())
    {
        return;
    }

    iItemData *itemData = new iItemData();

    const iaDirectory projectDir(_projectFolder);
    iItemPtr projectRoot = itemData->addItem(projectDir.getDirectoryName());
    projectRoot->setValue<iaString>(IGOR_ITEM_DATA_ICON, "igor_icon_folder");
    projectRoot->setValue<iaString>("relativePath", "");
    update(projectDir, projectRoot);

    if (*itemData != *_itemData)
    {
        _itemData = std::unique_ptr<iItemData>(itemData);

        _treeView->setItems(_itemData.get());
        updateGridView(_treeView->getSelectedItemPath());
    }
    else
    {
        delete itemData; // weird how I did this TODO this needs some explanation
        refreshGridView();
    }
}

void AssetBrowser::setProjectFolder(const iaString &projectFolder)
{
    _projectFolder = projectFolder;
    _updateHandle.triggerNow();
}

const iaString &AssetBrowser::getProjectFolder() const
{
    return _projectFolder;
}

ResourceSelectionChangedEvent &AssetBrowser::getResourceSelectionChangedEvent()
{
    return _resourceSelectionChanged;
}