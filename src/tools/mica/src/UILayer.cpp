
// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UILayer.h"
#include "widget3d/Widget3DLocator.h"
#include "widget3d/Widget3DEmitter.h"
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
static const wchar_t *DEFAULT_LOAD_SAVE_DIR = L"..\\data\\models";

UILayer::UILayer(iWindow *window, int32 zIndex, WorkspacePtr workspace)
    : iLayerWidgets(new iWidgetDefaultTheme("StandardFont.png", "WidgetThemePattern.png"), window, "Widgets", zIndex)
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
    _outliner = new Outliner();

    _outliner->registerOnExitMica(ExitMicaDelegate(this, &UILayer::onExitMica));
    _outliner->registerOnLoadFile(LoadFileDelegate(this, &UILayer::onLoadFile));
    _outliner->registerOnImportFile(ImportFileDelegate(this, &UILayer::onImportFile));
    _outliner->registerOnImportFileReference(ImportFileReferenceDelegate(this, &UILayer::onImportFileReference));
    _outliner->registerOnSaveFile(SaveFileDelegate(this, &UILayer::onSaveFile));
    _outliner->registerOnAddMaterial(AddMaterialDelegate(this, &UILayer::onAddMaterial));

    _propertiesDialog->registerStructureChangedDelegate(StructureChangedDelegate(_outliner, &Outliner::refreshView));

    _outliner->registerOnGraphSelectionChanged(GraphSelectionChangedDelegate(_propertiesDialog, &PropertiesDialog::onGraphViewSelectionChanged));
    _outliner->registerOnGraphSelectionChanged(GraphSelectionChangedDelegate(this, &UILayer::onGraphViewSelectionChanged));
    _outliner->registerOnMaterialSelectionChanged(MaterialSelectionChangedDelegate(_propertiesDialog, &PropertiesDialog::onMaterialSelectionChanged));

    _outliner->setActive();
    _outliner->setVisible();
    _outliner->refreshView();

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
    _outliner->refreshView();
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
    if (_fileDialog != dialog)
    {
        return;
    }

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

void UILayer::onImportFileDialogClosed(iDialogPtr dialog)
{
    /*    if (_fileDialog != dialog)
    {
        return;
    }

    iNodePtr selectNode = nullptr;

    if (_fileDialog->getReturnState() == iDialogReturnState::Ok)
    {
        iaString filename = _fileDialog->getFullPath();

        iNodeModel *model = iNodeManager::getInstance().createNode<iNodeModel>();
        iModelDataInputParameter *parameter = createDataInputParameter();
        model->setModel(filename, iResourceCacheMode::Free, parameter);

        //         forceLoadingNow(model);

        if (model->isValid())
        {
            iNodePtr groupNode = nullptr;

            auto children = model->getChildren();
            if (children.size() > 1)
            {
                groupNode = iNodeManager::getInstance().createNode<iNode>();
                iaString groupName = "group:";
                groupName += filename;
                groupNode->setName(groupName);

                iNodePtr cursorNode = nullptr;
                if (!_workspace->getSelection().empty())
                {
                    iNodePtr cursorNode = iNodeManager::getInstance().getNode(_workspace->getSelection()[0]);
                }

                if (cursorNode != nullptr)
                {
                    cursorNode->insertNode(groupNode);
                }
                else
                {
                    _workspace->getRootUser()->insertNode(groupNode);
                }

                selectNode = groupNode;
            }
            else
            {
                iNodePtr cursorNode = nullptr;
                if (!_workspace->getSelection().empty())
                {
                    iNodePtr cursorNode = iNodeManager::getInstance().getNode(_workspace->getSelection()[0]);
                }

                if (cursorNode != nullptr)
                {
                    groupNode = cursorNode;
                }
                else
                {
                    groupNode = _workspace->getRootUser();
                }

                if (!children.empty())
                {
                    selectNode = children.front();
                }
            }

            auto child = children.begin();
            while (child != children.end())
            {
                model->removeNode((*child));
                groupNode->insertNode((*child));
                child++;
            }
        }

        iNodeManager::getInstance().destroyNodeAsync(model);
    }

    _outliner->setActive();
    _outliner->setVisible();
    _outliner->refreshView();

    _propertiesDialog->setActive();
    _propertiesDialog->setVisible();

    _outliner->setSelectedNode(selectNode);
    // TODO frameOnSelectedNode();

    delete _fileDialog;
    _fileDialog = nullptr;*/
}

void UILayer::onImportFileReferenceDialogClosed(iDialogPtr dialog)
{
    /*    if (_fileDialog != dialog)
    {
        return;
    }

    iNodePtr selectNode = nullptr;

    if (_fileDialog->getReturnState() == iDialogReturnState::Ok)
    {
        iaString filename = _fileDialog->getFullPath();

        iNodeModel *model = iNodeManager::getInstance().createNode<iNodeModel>();
        iModelDataInputParameter *parameter = createDataInputParameter();

        model->setModel(filename, iResourceCacheMode::Free, parameter);
        // TODO forceLoadingNow(model);

        if (model->isValid())
        {
            iNodePtr cursorNode = nullptr;
            if (!_workspace->getSelection().empty())
            {
                iNodePtr cursorNode = iNodeManager::getInstance().getNode(_workspace->getSelection()[0]);
            }

            if (cursorNode != nullptr)
            {
                cursorNode->insertNode(model);
            }
            else
            {
                _workspace->getRootUser()->insertNode(model);
            }

            selectNode = model;
        }
    }

    _outliner->setActive();
    _outliner->setVisible();
    _outliner->refreshView();

    _propertiesDialog->setActive();
    _propertiesDialog->setVisible();

    _outliner->setSelectedNode(selectNode);
    // TODO frameOnSelectedNode();

    delete _fileDialog;
    _fileDialog = nullptr;*/
}

void UILayer::onFileLoadDialogClosed(iDialogPtr dialog)
{
    /* TODO
    if (_fileDialog != dialog)
    {
        return;
    }

    iNodePtr selectNode = nullptr;

    if (_fileDialog->getReturnState() == iDialogReturnState::Ok)
    {
        iaString filename = _fileDialog->getFullPath();

        if (_workspace->getChildren().size() > 0)
        {
            auto children = _workspace->getChildren();
            auto childIter = children.begin();
            while (childIter != children.end())
            {
                _workspace->removeNode((*childIter));
                iNodeManager::getInstance().destroyNodeAsync((*childIter));
                childIter++;
            }
        }

        iNodeModel *model = iNodeManager::getInstance().createNode<iNodeModel>();
        iModelDataInputParameter *parameter = createDataInputParameter();

        model->setModel(filename, iResourceCacheMode::Free, parameter);
        forceLoadingNow(model);

        if (model->isValid())
        {
            iNodePtr insertAt = nullptr;

            auto children = model->getChildren();
            if (children.size() > 1)
            {
                insertAt = iNodeManager::getInstance().createNode<iNode>();
                iaString groupName = "group:";
                groupName += filename;
                insertAt->setName(groupName);

                _workspace->insertNode(insertAt);
                selectNode = insertAt;
            }
            else
            {
                insertAt = _workspace;
                selectNode = children.front();
            }

            auto child = children.begin();
            while (child != children.end())
            {
                model->removeNode((*child));
                insertAt->insertNode((*child));
                child++;
            }
        }

        iNodeManager::getInstance().destroyNodeAsync(model);
    }

    _outliner->setActive();
    _outliner->setVisible();
    _outliner->refreshView();

    _propertiesDialog->setActive();
    _propertiesDialog->setVisible();

    _outliner->setSelectedNode(selectNode);
    frameOnSelectedNode();

    delete _fileDialog;
    _fileDialog = nullptr; */
}

void UILayer::onGraphViewSelectionChanged(uint64 nodeID)
{
    /*    _selectedNodeID = nodeID;
    _manipulator->setNodeID(_selectedNodeID);
    resetManipulatorMode();

    // todo caching?
    if (_widget3D != nullptr)
    {
        delete _widget3D;
        _widget3D = nullptr;
    }

    iNode *node = iNodeManager::getInstance().getNode(_selectedNodeID);
    if (node)
    {
        switch (node->getType())
        {
        case iNodeType::iNodeEmitter:
            _widget3D = new Widget3DEmitter(&_window, &_viewWidget3D, _sceneWidget3D);
            break;
        }
    }

    if (_widget3D != nullptr)
    {
        _widget3D->setNodeID(_selectedNodeID);
    }*/
}

void UILayer::onExitMica()
{
    iApplication::getInstance().stop();
}

void UILayer::onEvent(iEvent &event)
{
    // call base class
    iLayerWidgets::onEvent(event);

    event.dispatch<iKeyDownEvent_TMP>(IGOR_BIND_EVENT_FUNCTION(UILayer::onKeyDown));
}

bool UILayer::onKeyDown(iKeyDownEvent_TMP &event)
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
            _outliner->duplicateSelected();
        }
        return true;

    case iKeyCode::X:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
            _outliner->cutSelected();
        }
        return true;

    case iKeyCode::C:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
            _outliner->copySelected();
        }
        return true;

    case iKeyCode::V:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
            _outliner->pasteSelected();
        }
        return true;

    case iKeyCode::O:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
            _outliner->fileOpen();
        }
        return true;

    case iKeyCode::S:
        if (iKeyboard::getInstance().getKey(iKeyCode::LControl))
        {
            _outliner->fileSave();
        }
        return true;

    case iKeyCode::Delete:
        _outliner->deleteSelected();
        return true;
    }

    return false;
}

void UILayer::clearWorkspace()
{
    _workspace->clear();
    _outliner->refreshView();
}
