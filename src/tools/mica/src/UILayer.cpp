
// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UILayer.h"
#include "actions/Actions.h"

#include "usercontrols/UserControlMesh.h"
#include "usercontrols/UserControlModel.h"
#include "usercontrols/UserControlNode.h"
#include "usercontrols/UserControlParticleSystem.h"
#include "usercontrols/UserControlTransformation.h"
#include "usercontrols/UserControlLight.h"
#include "usercontrols/UserControlEmitter.h"
#include "usercontrols/UserControlMaterial.h"
#include "usercontrols/UserControlProperties.h"
#include "usercontrols/UserControlMaterialView.h"
#include "usercontrols/UserControlGraphView.h"

/*! default file open folder definition
 */
static const wchar_t *DEFAULT_LOAD_SAVE_DIR = L"../../data/models";

UILayer::UILayer(iWindowPtr window, int32 zIndex, WorkspacePtr workspace)
    : iLayerWidgets(iWidgetThemePtr(new iWidgetDefaultTheme("igor/textures/StandardFont.png", "WidgetThemePattern.png")), window, "Widgets", zIndex), _workspace(workspace)
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

    _propertiesDialog = new PropertiesDialog();
    _outliner = new Outliner(_workspace);

    _outliner->registerOnExitMica(ExitMicaDelegate(this, &UILayer::onExitMica));
    _outliner->registerOnLoadFile(LoadFileDelegate(this, &UILayer::onLoadFile));
    _outliner->registerOnImportFile(ImportFileDelegate(this, &UILayer::onImportFile));
    _outliner->registerOnImportFileReference(ImportFileReferenceDelegate(this, &UILayer::onImportFileReference));
    _outliner->registerOnSaveFile(SaveFileDelegate(this, &UILayer::onSaveFile));
    _outliner->registerOnAddMaterial(AddMaterialDelegate(this, &UILayer::onAddMaterial));
    _outliner->registerOnLoadMaterial(LoadMaterialDelegate(this, &UILayer::onLoadMaterial));

    // _propertiesDialog->registerStructureChangedDelegate(StructureChangedDelegate(_outliner, &Outliner::refreshView));

    _outliner->registerOnGraphSelectionChanged(GraphSelectionChangedDelegate(_propertiesDialog, &PropertiesDialog::onGraphViewSelectionChanged));
    _outliner->registerOnGraphSelectionChanged(GraphSelectionChangedDelegate(this, &UILayer::onGraphViewSelectionChanged));
    _outliner->registerOnMaterialSelectionChanged(MaterialSelectionChangedDelegate(_propertiesDialog, &PropertiesDialog::onMaterialSelectionChanged));

    _outliner->setActive();
    _outliner->setVisible();
    _outliner->refresh();

    _propertiesDialog->setActive();
    _propertiesDialog->setVisible();
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

    if (_fileDialog != nullptr)
    {
        delete _fileDialog;
        _fileDialog = nullptr;
    }

    // call base class
    iLayerWidgets::onDeinit();
}

void UILayer::onAddMaterial()
{
    iMaterialResourceFactory::getInstance().createMaterial("new Material");
    _outliner->refresh();
}

void UILayer::onLoadMaterial()
{
    if (_fileDialog == nullptr)
    {
        _fileDialog = new iDialogFileSelect();
        _fileDialog->open(iDialogCloseDelegate(this, &UILayer::onLoadMaterialFileDialogClosed), iFileDialogPurpose::Load);
    }
}

void UILayer::onImportFile()
{
    if (_fileDialog == nullptr)
    {
        _fileDialog = new iDialogFileSelect();
        _fileDialog->open(iDialogCloseDelegate(this, &UILayer::onImportFileDialogClosed), iFileDialogPurpose::Load, DEFAULT_LOAD_SAVE_DIR);
    }
}

void UILayer::onImportFileReference()
{
    if (_fileDialog == nullptr)
    {
        _fileDialog = new iDialogFileSelect();
        _fileDialog->open(iDialogCloseDelegate(this, &UILayer::onImportFileReferenceDialogClosed), iFileDialogPurpose::Load, DEFAULT_LOAD_SAVE_DIR);
    }
}

void UILayer::onLoadFile()
{
    if (_fileDialog == nullptr)
    {
        _fileDialog = new iDialogFileSelect();
        _fileDialog->open(iDialogCloseDelegate(this, &UILayer::onFileLoadDialogClosed), iFileDialogPurpose::Load, DEFAULT_LOAD_SAVE_DIR);
    }
}

void UILayer::onSaveFile()
{
    if (_fileDialog == nullptr)
    {
        _fileDialog = new iDialogFileSelect();
        _fileDialog->open(iDialogCloseDelegate(this, &UILayer::onFileSaveDialogClosed), iFileDialogPurpose::Save, DEFAULT_LOAD_SAVE_DIR);
    }
}

void UILayer::onFileSaveDialogClosed(iDialogPtr dialog)
{
    if (_fileDialog->getReturnState() == iDialogReturnState::Ok)
    {
        iaString filename = _fileDialog->getFullPath();

        auto rootNode = _workspace->getRootUser();

        std::vector<iNodePtr> children = rootNode->getChildren();
        children.insert(children.end(), rootNode->getInactiveChildren().begin(), rootNode->getInactiveChildren().end());

        if (children.empty())
        {
            con_warn("nothing to save");
        }
        else if (children.size() == 1)
        {
            iModelResourceFactory::getInstance().exportModelData(filename, children[0]);
        }
        else
        {
            iModelResourceFactory::getInstance().exportModelData(filename, rootNode);
        }
    }

    delete _fileDialog;
    _fileDialog = nullptr;
}

void UILayer::onLoadMaterialFileDialogClosed(iDialogPtr dialog)
{
    if (_fileDialog->getReturnState() == iDialogReturnState::Ok)
    {
        iMaterialPtr material = iMaterialResourceFactory::getInstance().loadMaterial(_fileDialog->getFullPath());
        material->setVisibility(iMaterialVisibility::Public);
        _outliner->refresh();
    }

    delete _fileDialog;
    _fileDialog = nullptr;
}

void UILayer::onImportFileDialogClosed(iDialogPtr dialog)
{
    if (_fileDialog->getReturnState() == iDialogReturnState::Ok)
    {
        _workspace->importFile(_fileDialog->getFullPath());
    }

    delete _fileDialog;
    _fileDialog = nullptr;
}

void UILayer::onImportFileReferenceDialogClosed(iDialogPtr dialog)
{
    if (_fileDialog->getReturnState() == iDialogReturnState::Ok)
    {
        _workspace->importFileReference(_fileDialog->getFullPath());
    }

    delete _fileDialog;
    _fileDialog = nullptr;
}

void UILayer::onFileLoadDialogClosed(iDialogPtr dialog)
{
    if (_fileDialog->getReturnState() == iDialogReturnState::Ok)
    {
        _workspace->loadFile(_fileDialog->getFullPath());
    }

    delete _fileDialog;
    _fileDialog = nullptr;
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

void UILayer::onExitMica()
{
    iApplication::getInstance().stop();
}

void UILayer::onPreDraw()
{
    if (_refresh)
    {
        _outliner->refresh();
        _refresh = false;
    }

    iLayerWidgets::onPreDraw();
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
            if (_outliner->isActive() && _outliner->isVisible())
            {
                _outliner->setActive(false);
                _outliner->setVisible(false);
            }
            else
            {
                _outliner->setActive();
                _outliner->setVisible();
                _outliner->refresh();
            }

            if (_propertiesDialog->isActive() && _propertiesDialog->isVisible())
            {
                _propertiesDialog->setActive(false);
                _propertiesDialog->setVisible(false);
            }
            else
            {
                _propertiesDialog->setActive();
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
