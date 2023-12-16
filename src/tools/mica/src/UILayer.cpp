
// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UILayer.h"
#include "actions/Actions.h"

#include "usercontrols/UserControlMaterialView.h"
#include "usercontrols/UserControlGraphView.h"

/*! default file open folder definition
 */
static const wchar_t *DEFAULT_LOAD_SAVE_DIR = L"../../data/models";

UILayer::UILayer(iWindowPtr window, int32 zIndex, WorkspacePtr workspace)
    : iLayerWidgets(iWidgetThemePtr(new iWidgetDefaultTheme(iResourceManager::getInstance().loadResource<iTexture>("igor_font_default"),
                                                            iResourceManager::getInstance().loadResource<iTexture>("igor_widget_theme_pattern"))),
                    window, "Widgets", zIndex),
      _workspace(workspace)
{
}

UILayer::~UILayer()
{
    // TODO ?
}

void UILayer::onInit()
{
    // call base class
    iLayerWidgets::onInit();

    registerMicaActions();

    _mainDialog = new MainDialog(_workspace);
    _mainDialog->setEnabled();
    _mainDialog->setVisible();

    _outliner = new Outliner(_workspace);
    _outliner->setEnabled();
    _outliner->setVisible();
    _outliner->refresh(); // TODO ?

    _propertiesDialog = new PropertiesEditor();
    _propertiesDialog->setEnabled();
    _propertiesDialog->setVisible();

    _assetBrowser = new AssetBrowser();
    _assetBrowser->setEnabled();
    _assetBrowser->setVisible();

    _viewport = new Viewport(_workspace);
    _viewport->setEnabled();
    _viewport->setVisible();

    _mainDialog->getEventCreateProject().add(CreateProjectDelegate(this, &UILayer::onCreateProject));
    _mainDialog->getEventLoadProject().add(LoadProjectDelegate(this, &UILayer::onLoadProject));
    _mainDialog->getEventSaveProject().add(SaveProjectDelegate(this, &UILayer::onSaveProject));
    _mainDialog->getEventLoadFile().add(LoadFileDelegate(this, &UILayer::onLoadFile));
    _mainDialog->getEventSaveFile().add(SaveFileDelegate(this, &UILayer::onSaveFile));

    _outliner->registerOnImportFile(ImportFileDelegate(this, &UILayer::onImportFile));
    _outliner->registerOnImportFileReference(ImportFileReferenceDelegate(this, &UILayer::onImportFileReference));

    _outliner->registerOnAddMaterial(AddMaterialDelegate(this, &UILayer::onAddMaterial));
    _outliner->registerOnLoadMaterial(LoadMaterialDelegate(this, &UILayer::onLoadMaterial));

    // _propertiesDialog->registerStructureChangedDelegate(StructureChangedDelegate(_outliner, &Outliner::refreshView));

    _outliner->registerOnGraphSelectionChanged(GraphSelectionChangedDelegate(_propertiesDialog, &PropertiesEditor::setSelection));
    _outliner->registerOnGraphSelectionChanged(GraphSelectionChangedDelegate(this, &UILayer::onGraphViewSelectionChanged));
    _outliner->registerOnResourceSelectionChanged(ResourceSelectionChangedDelegate(_propertiesDialog, &PropertiesEditor::setSelection));

    // load layout configuration here instead of this hack
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

void UILayer::onAddMaterial()
{
    iResourceManager::getInstance().createResource<iMaterial>();

    _outliner->refresh();
}

void UILayer::onLoadMaterial()
{
    _fileDialog.open(iDialogCloseDelegate(this, &UILayer::onLoadMaterialFileDialogClosed), iFileDialogPurpose::Load);
}

void UILayer::onImportFile()
{
    _fileDialog.open(iDialogCloseDelegate(this, &UILayer::onImportFileDialogClosed), iFileDialogPurpose::Load, DEFAULT_LOAD_SAVE_DIR);
}

void UILayer::onImportFileReference()
{
    _fileDialog.open(iDialogCloseDelegate(this, &UILayer::onImportFileReferenceDialogClosed), iFileDialogPurpose::Load, DEFAULT_LOAD_SAVE_DIR);
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

    _activeProject = iProject::createProject(_fileDialog.getFullPath());
    _assetBrowser->setProject(_activeProject);
}

void UILayer::onLoadProject()
{
    _fileDialog.open(iDialogCloseDelegate(this, &UILayer::onLoadProjectDialogClosed), iFileDialogPurpose::SelectFolder, iaDirectory::getCurrentDirectory());
}

void UILayer::onLoadProjectDialogClosed(iDialogPtr dialog)
{
    if (_fileDialog.getReturnState() != iDialogReturnState::Ok)
    {
        return;
    }

    _activeProject = iProject::loadProject(_fileDialog.getFullPath());
    _assetBrowser->setProject(_activeProject);
}

void UILayer::onSaveProject()
{
    if (_activeProject == nullptr)
    {
        con_err("no project to save");
        return;
    }
    iProject::saveProject(_activeProject);
}

void UILayer::onLoadFile()
{
    _fileDialog.open(iDialogCloseDelegate(this, &UILayer::onFileLoadDialogClosed), iFileDialogPurpose::Load, DEFAULT_LOAD_SAVE_DIR);
}

void UILayer::onSaveFile()
{
    _fileDialog.open(iDialogCloseDelegate(this, &UILayer::onFileSaveDialogClosed), iFileDialogPurpose::Save, DEFAULT_LOAD_SAVE_DIR);
}

void UILayer::onFileSaveDialogClosed(iDialogPtr dialog)
{
    if (_fileDialog.getReturnState() != iDialogReturnState::Ok)
    {
        return;
    }

    iaString filename = _fileDialog.getFullPath();

    auto rootNode = _workspace->getRootUser();

    std::vector<iNodePtr> children = rootNode->getChildren();
    children.insert(children.end(), rootNode->getInactiveChildren().begin(), rootNode->getInactiveChildren().end());

    if (children.empty())
    {
        con_warn("nothing to save");
    }
    else if (children.size() == 1)
    {
        iModelFactory::exportToFile(filename, children[0]);
    }
    else
    {
        iModelFactory::exportToFile(filename, rootNode);
    }
}

void UILayer::onLoadMaterialFileDialogClosed(iDialogPtr dialog)
{
    if (_fileDialog.getReturnState() != iDialogReturnState::Ok)
    {
        return;
    }

    iMaterialPtr material = iResourceManager::getInstance().loadResource<iMaterial>(_fileDialog.getFullPath());
    material->setVisibility(iMaterialVisibility::Public);
    _outliner->refresh();
}

void UILayer::onImportFileDialogClosed(iDialogPtr dialog)
{
    if (_fileDialog.getReturnState() != iDialogReturnState::Ok)
    {
        return;
    }

    _workspace->importFile(_fileDialog.getFullPath());
}

void UILayer::onImportFileReferenceDialogClosed(iDialogPtr dialog)
{
    if (_fileDialog.getReturnState() != iDialogReturnState::Ok)
    {
        return;
    }

    _workspace->importFileReference(_fileDialog.getFullPath());
}

void UILayer::onFileLoadDialogClosed(iDialogPtr dialog)
{
    if (_fileDialog.getReturnState() != iDialogReturnState::Ok)
    {
        return;
    }

    _workspace->loadFile(_fileDialog.getFullPath());
}

void UILayer::onGraphViewSelectionChanged(uint64 nodeID)
{
    if (nodeID == iNode::INVALID_NODE_ID)
    {
        _workspace->clearSelection();
    }
    else
    {
        _workspace->setSelection(std::vector<iNodeID>() = {nodeID});
    }
}

void UILayer::onUpdate()
{
    if (_refresh)
    {
        _outliner->refresh();
        _refresh = false;
    }

    iLayerWidgets::onUpdate();
}

void UILayer::onEvent(iEvent &event)
{
    iLayerWidgets::onEvent(event);

    event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(UILayer::onKeyDown));
    event.dispatch<iEventNodeAddedToScene>(IGOR_BIND_EVENT_FUNCTION(UILayer::onNodeAddedToScene));
    event.dispatch<iEventNodeRemovedFromScene>(IGOR_BIND_EVENT_FUNCTION(UILayer::onNodeRemovedFromScene));
    event.dispatch<iEventSceneSelectionChanged>(IGOR_BIND_EVENT_FUNCTION(UILayer::onSceneSelectionChanged));
}

bool UILayer::onNodeAddedToScene(iEventNodeAddedToScene &event)
{
    _refresh = true;
    return false;
}

bool UILayer::onNodeRemovedFromScene(iEventNodeRemovedFromScene &event)
{
    _refresh = true;
    return false;
}

bool UILayer::onSceneSelectionChanged(iEventSceneSelectionChanged &event)
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
            clearWorkspace();
        }
        return true;

    case iKeyCode::D:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
            _workspace->duplicateSelected();
        }
        return true;

    case iKeyCode::X:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
            _workspace->cutSelected();
        }
        return true;

    case iKeyCode::C:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
            _workspace->copySelected();
        }
        return true;

    case iKeyCode::V:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
            _workspace->pasteSelected();
        }
        return true;

    case iKeyCode::O:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
            onLoadFile();
        }
        return true;

    case iKeyCode::S:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
            onSaveFile();
        }
        return true;

    case iKeyCode::Delete:
        _workspace->deleteSelected();
        return true;

    case iKeyCode::Space:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
            if (_outliner->isEnabled() && _outliner->isVisible())
            {
                _outliner->setEnabled(false);
                _outliner->setVisible(false);
            }
            else
            {
                _outliner->setEnabled();
                _outliner->setVisible();
                _outliner->refresh();
            }

            if (_propertiesDialog->isEnabled() && _propertiesDialog->isVisible())
            {
                _propertiesDialog->setEnabled(false);
                _propertiesDialog->setVisible(false);
            }
            else
            {
                _propertiesDialog->setEnabled();
                _propertiesDialog->setVisible();
            }
        }
        return true;
    }

    return false;
}

void UILayer::clearWorkspace()
{
    _workspace->clear();
}
