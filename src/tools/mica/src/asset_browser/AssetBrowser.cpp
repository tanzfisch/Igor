// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "AssetBrowser.h"

#include "UserControlResourceIcon.h"

AssetBrowser::AssetBrowser()
{
    _itemData = std::make_unique<iItemData>();

    initUI();
}

AssetBrowser::~AssetBrowser()
{
    iFilesystem::getInstance().stopListenToChanges(_currentPath);
}

void AssetBrowser::initUI()
{
    setTitle("Asset Browser");
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
    _showFilesButton->setBackgroundTexture("igor_icon_files");
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

    _gridView = new iWidgetFixedGridLayout();
    _gridView->setVerticalAlignment(iVerticalAlignment::Top);
    _gridView->setHorizontalAlignment(iHorizontalAlignment::Left);
    _gridView->setCellSize(iaVector2f(150, 150));
    _gridView->registerOnSelectionChangedEvent(iSelectionChangedDelegate(this, &AssetBrowser::onSelectionChanged));
    

    iWidgetScrollPtr scroll = new iWidgetScroll();
    scroll->registerOnContextMenuEvent(iContextMenuDelegate(this, &AssetBrowser::OnContextMenu));
    scroll->addWidget(_gridView);
    splitter->addWidget(scroll);
}

void AssetBrowser::OnContextMenu(iWidgetPtr source)
{
    _contextMenu.clear();
    _contextMenu.setPos(iMouse::getInstance().getPos());

    if (_currentPath.isEmpty())
    {
        return;
    }

    iActionContextPtr actionContext = std::make_shared<iFilesystemActionContext>(_currentPath);

    iWidgetMenuPtr createMenu = new iWidgetMenu("Create");
    _contextMenu.addMenu(createMenu);

    createMenu->addAction("igor:create_scene", actionContext);
    createMenu->addAction("igor:create_material", actionContext);
    createMenu->addAction("igor:create_shader", actionContext);

    _contextMenu.open();
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
    onUpdateFilesystem();
}

void AssetBrowser::updateContentModeButton()
{
    _showFilesButton->setChecked(_contentMode == ContentMode::Files);
}

static void findMeshPaths(iNodePtr node, const iaString &meshPath, std::vector<iaString> &paths)
{
    const iaString path = meshPath + iaString("/") + node->getName();

    if (node->getType() == iNodeType::iNodeMesh)
    {
        paths.push_back(path);
    }

    const auto &children = node->getChildren();
    for (const auto child : children)
    {
        findMeshPaths(child, path, paths);
    }
}

void AssetBrowser::onUpdateGridView()
{
    _gridView->clear();

    const iItemPtr item = _itemData->getItem(_treeView->getSelectedItemPath());
    if (!item->hasValue("relativePath"))
    {
        return;
    }
    const iaString path = item->getValue<iaString>("relativePath");

    const iaDirectory projectDir(_projectFolder);
    const auto newPath = iaDirectory::fixPath(projectDir.getAbsoluteDirectoryName() + IGOR_PATHSEPARATOR + path);
    if (_currentPath != newPath)
    {
        iFilesystem::getInstance().stopListenToChanges(_currentPath);
        iFilesystem::getInstance().listenToChanges(newPath);

        _currentPath = newPath;
    }

    if (iaDirectory::isDirectory(_currentPath))
    {
        iResourceManager::getInstance().getResourceProcessedEvent().remove(iResourceProcessedDelegate(this, &AssetBrowser::onResourceLoaded));

        const iaDirectory dir(_currentPath);
        auto files = dir.getFiles();
        for (const auto &file : files)
        {
            const iaString relativePath = iaDirectory::getRelativePath(projectDir.getAbsoluteDirectoryName(), file.getFullFileName());
            if (_contentMode == ContentMode::Assets &&
                iResourceManager::getInstance().getResourceID(relativePath) == iResourceID(IGOR_INVALID_ID))
            {
                continue;
            }

            UserControlResourceIcon *icon = new UserControlResourceIcon(relativePath, "", _gridView);
        }
    }
    else
    {
        const iaFile file(_currentPath);
        if (!IGOR_SUPPORTED_MODEL_EXTENSION(file.getExtension()))
        {
            return;
        }

        iResourceManager::getInstance().getResourceProcessedEvent().add(iResourceProcessedDelegate(this, &AssetBrowser::onResourceLoaded), false, true);

        const iaString relativePath = iaDirectory::getRelativePath(projectDir.getAbsoluteDirectoryName(), file.getFullFileName());
        _currentFocussedResource = iResourceManager::getInstance().getResourceID(relativePath);
        iModelPtr model = iResourceManager::getInstance().requestResource<iModel>(_currentFocussedResource);
        if (model->isValid())
        {
            std::vector<iaString> meshPaths;
            findMeshPaths(model->getNode(), "", meshPaths);

            for (const auto &path : meshPaths)
            {
                UserControlResourceIcon *icon = new UserControlResourceIcon(relativePath, path, _gridView);
            }
        }
    }
}

void AssetBrowser::onResourceLoaded(iResourceID resourceID)
{
    if (_currentFocussedResource != resourceID)
    {
        return;
    }

    onUpdateGridView();
}

void AssetBrowser::onRefreshGridView()
{
    for (auto child : _gridView->getChildren())
    {
        UserControlResourceIcon *icon = static_cast<UserControlResourceIcon *>(child);
        icon->refresh();
    }
}

void AssetBrowser::onClickTreeView(const iWidgetPtr source)
{
    onUpdateGridView();
}

void AssetBrowser::update(const iaDirectory &dir, iItemPtr item)
{
    const iaDirectory projectDir(_projectFolder);
    auto dirs = dir.getDirectories();
    auto files = dir.getFiles();

    for (const auto &subDir : dirs)
    {
        iItemPtr child = item->addItem(subDir.getDirectoryName());
        child->setValue<iaString>(IGOR_ITEM_DATA_ICON, "igor_icon_folder");
        iaString relativePath = iaDirectory::getRelativePath(projectDir.getAbsoluteDirectoryName(), subDir.getAbsoluteDirectoryName());
        child->setValue<iaString>("relativePath", relativePath);

        update(subDir, child);
    }

    for (const auto &file : files)
    {
        const iaString relativePath = iaDirectory::getRelativePath(projectDir.getAbsoluteDirectoryName(), file.getFullFileName());
        if (_contentMode == ContentMode::Assets &&
            iResourceManager::getInstance().getResourceID(relativePath) == iResourceID(IGOR_INVALID_ID))
        {
            continue;
        }

        iItemPtr child = item->addItem(file.getStem());
        child->setValue<iaString>(IGOR_ITEM_DATA_ICON, "igor_icon_file_model");
        child->setValue<iaString>("relativePath", relativePath);
    }
}

bool AssetBrowser::onEvent(iEvent &event)
{
    if (iWidget::onEvent(event))
    {
        return true;
    }

    if (!event.isOfKind(iEventKind::Filesystem))
    {
        return false;
    }

    onUpdateFilesystem();

    return true;
}

void AssetBrowser::onUpdateFilesystem()
{
    if (_projectFolder.isEmpty())
    {
        return;
    }

    _itemData = std::unique_ptr<iItemData>(new iItemData());

    const iaDirectory projectDir(_projectFolder);
    iItemPtr projectRoot = _itemData->addItem(projectDir.getDirectoryName());
    projectRoot->setValue<iaString>(IGOR_ITEM_DATA_ICON, "igor_icon_folder");
    projectRoot->setValue<iaString>("relativePath", "");
    update(projectDir, projectRoot);
    _treeView->setItems(_itemData.get());
    onUpdateGridView();
}

void AssetBrowser::setProjectFolder(const iaString &projectFolder)
{
    _projectFolder = projectFolder;
    onUpdateFilesystem();
}

const iaString &AssetBrowser::getProjectPath() const
{
    return _projectFolder;
}

ResourceSelectionChangedEvent &AssetBrowser::getResourceSelectionChangedEvent()
{
    return _resourceSelectionChanged;
}