
// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UILayer.h"

/*! default file open folder definition
 */
static const wchar_t *DEFAULT_LOAD_SAVE_DIR = L"../../data/models";

UILayer::UILayer(iWindowPtr window, int32 zIndex)
    : iLayerWidgets(iWidgetThemePtr(new iWidgetDefaultTheme(iResourceManager::getInstance().loadResource<iTexture>("igor_font_default"),
                                                            iResourceManager::getInstance().loadResource<iTexture>("igor_widget_theme_pattern"))),
                    window, "Widgets", zIndex)
{
}

UILayer::~UILayer()
{
    // TODO ?
}

void UILayer::onInit()
{
    iLayerWidgets::onInit();

    _mainDialog = new MainDialog();
    _mainDialog->setEnabled();
    _mainDialog->setVisible();

    _propertiesDialog = new PropertiesEditor();
    _propertiesDialog->setEnabled();
    _propertiesDialog->setVisible();

    _outliner = new Outliner();
    _outliner->setEnabled();
    _outliner->setVisible();
    _outliner->getEntitySelectionChangedEvent().add(EntitySelectionChangedDelegate(this, &UILayer::onOutlinerSelectionChanged));

    _assetBrowser = new AssetBrowser();
    _assetBrowser->setEnabled();
    _assetBrowser->setVisible();
    _assetBrowser->getResourceSelectionChangedEvent().add(ResourceSelectionChangedDelegate(_propertiesDialog, &PropertiesEditor::setSelectionResource));

    _viewport = new Viewport();
    _viewport->setEnabled();
    _viewport->setVisible();

    _mainDialog->getEventCreateProject().add(CreateProjectDelegate(this, &UILayer::onCreateProject));
    _mainDialog->getEventLoadProject().add(LoadProjectDelegate(this, &UILayer::onLoadProject));
    _mainDialog->getEventSaveProject().add(SaveProjectDelegate(this, &UILayer::onSaveProject));

    // TODO load layout configuration here instead of this hack
    iWidgetSplitterPtr rootSplitter = static_cast<iWidgetSplitterPtr>(_mainDialog->getChildren()[0]->getChildren()[1]->getChildren()[0]);
    iWidgetSplitterPtr splitter0 = new iWidgetSplitter(true);
    iWidgetSplitterPtr splitter1 = new iWidgetSplitter(true);

    rootSplitter->setOrientation(iSplitterOrientation::Vertical);
    rootSplitter->setRatio(0.1f);
    rootSplitter->addWidget(_outliner);
    rootSplitter->addWidget(splitter0);

    splitter0->setOrientation(iSplitterOrientation::Vertical);
    splitter0->setRatio(0.8f);
    splitter0->addWidget(splitter1);
    splitter0->addWidget(_propertiesDialog);

    splitter1->setOrientation(iSplitterOrientation::Horizontal);
    splitter1->setRatio(0.7f);
    splitter1->addWidget(_viewport);
    splitter1->addWidget(_assetBrowser);
}

void UILayer::onOutlinerSelectionChanged(const iEntitySceneID &sceneID, const iEntityID &entityID)
{
    _propertiesDialog->setSelectionEntity(sceneID, entityID);
}

void UILayer::onDeinit()
{
    if (_propertiesDialog != nullptr)
    {
        delete _propertiesDialog;
        _propertiesDialog = nullptr;
    }

    if (_outliner != nullptr)
    {
        delete _outliner;
        _outliner = nullptr;
    }

    // call base class
    iLayerWidgets::onDeinit();
}

void UILayer::onCreateProject()
{
    _fileDialog.open(iDialogCloseDelegate(this, &UILayer::onCreateProjectDialogClosed), iFileDialogPurpose::SelectFolder, iaDirectory::getCurrentDirectory());
}

void UILayer::onCreateProjectDialogClosed(iDialogPtr dialog)
{
    if (_fileDialog.getReturnState() != iDialogReturnState::Ok)
    {
        return;
    }

    iProject::getInstance().create(_fileDialog.getFullPath());
    _assetBrowser->setProjectFolder(iProject::getInstance().getProjectPath());
}

void UILayer::onLoadProject()
{
    _fileDialog.open(iDialogCloseDelegate(this, &UILayer::onLoadProjectDialogClosed), iFileDialogPurpose::Load, iaDirectory::getCurrentDirectory(), {"project"});
}

void UILayer::onLoadProjectDialogClosed(iDialogPtr dialog)
{
    if (_fileDialog.getReturnState() != iDialogReturnState::Ok)
    {
        return;
    }

    iProject::getInstance().load(_fileDialog.getFullPath());
    _assetBrowser->setProjectFolder(iProject::getInstance().getProjectPath());
}

void UILayer::onSaveProject()
{
    iProject::getInstance().save();
}

void UILayer::onUpdate()
{
    if (_refresh)
    {
        _refresh = false;
    }

    iLayerWidgets::onUpdate();
}

void UILayer::onEvent(iEvent &event)
{
    iLayerWidgets::onEvent(event);

    event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(UILayer::onKeyDown));

    event.dispatch<iEventFileCreated>(IGOR_BIND_EVENT_FUNCTION(UILayer::onFileCreated));
    event.dispatch<iEventFileDeleted>(IGOR_BIND_EVENT_FUNCTION(UILayer::onFileDeleted));
    event.dispatch<iEventFileMovedFrom>(IGOR_BIND_EVENT_FUNCTION(UILayer::onFileMovedFrom));
    event.dispatch<iEventFileMovedTo>(IGOR_BIND_EVENT_FUNCTION(UILayer::onFileMovedTo));
    event.dispatch<iEventFileChanged>(IGOR_BIND_EVENT_FUNCTION(UILayer::onFileChanged));
}

bool UILayer::onFileCreated(iEventFileCreated &event)
{
    _refresh = true;
    return false;
}

bool UILayer::onFileDeleted(iEventFileDeleted &event)
{
    _refresh = true;
    return false;
}

bool UILayer::onFileMovedFrom(iEventFileMovedFrom &event)
{
    _refresh = true;
    return false;
}

bool UILayer::onFileMovedTo(iEventFileMovedTo &event)
{
    _refresh = true;
    return false;
}

bool UILayer::onFileChanged(iEventFileChanged &event)
{
    _refresh = true;
    return false;
}

bool UILayer::onKeyDown(iEventKeyDown &event)
{
    switch (event.getKey())
    {
    case iKeyCode::N:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
            // clearWorkspace();
        }
        return true;

    case iKeyCode::D:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
            // _workspace->duplicateSelected();
        }
        return true;

    case iKeyCode::X:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
            // _workspace->cutSelected();
        }
        return true;

    case iKeyCode::C:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
            // _workspace->copySelected();
        }
        return true;

    case iKeyCode::V:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
            // _workspace->pasteSelected();
        }
        return true;

    case iKeyCode::O:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
            // TODO open project
        }
        return true;

    case iKeyCode::S:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
            // TODO save project
        }
        return true;

    case iKeyCode::Delete:
        // TODO _workspace->deleteSelected();
        return true;

        /* TODO hide _outliner, _propertiesDialog, _assetBrowser
            and make full screen _viewport
            and vice versa
    case iKeyCode::Space:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
        }
        return true; */
    }

    return false;
}
