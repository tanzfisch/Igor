// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include "SceneOutliner.h"

SceneOutliner::SceneOutliner(WorkspacePtr workspace)
    : _workspace(workspace)
{
    initGUI();
}

SceneOutliner::~SceneOutliner()
{
    deinitGUI();
}

void SceneOutliner::initGUI()
{
    _messageBox = new iDialogMessageBox();

    setDockable(true);
    setTitle("Scene Outliner");
    setMinWidth(350);

    _grid = new iWidgetGridLayout(this);
    _grid->setBorder(2);
    _grid->setCellSpacing(8);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _grid->setVerticalAlignment(iVerticalAlignment::Stretch);
    _grid->appendRows(1);
    _grid->setStretchRow(1);
    _grid->setStretchColumn(0);

    _userControlGraphView = new UserControlGraphView(_workspace, this);
    _userControlGraphView->registerOnSelectionChange(GraphSelectionChangedDelegate(this, &SceneOutliner::onGraphSelectionChanged));

    _grid->addWidget(_userControlGraphView, 0, 1);
    refresh();
}

void SceneOutliner::deinitGUI()
{
    if (_userControlGraphView != nullptr &&
        !_userControlGraphView->hasParent())
    {
        delete _userControlGraphView;
        _userControlGraphView = nullptr;
    }

    if (_messageBox != nullptr)
    {
        delete _messageBox;
        _messageBox = nullptr;
    }

    if (_decisionBoxModelRef != nullptr)
    {
        delete _decisionBoxModelRef;
        _decisionBoxModelRef = nullptr;
    }
}

void SceneOutliner::onGraphSelectionChanged(uint64 nodeID)
{
    _graphSelectionChanged(nodeID);
}

void SceneOutliner::onDelete(const iWidgetPtr source)
{
    _workspace->deleteSelected();
}

void SceneOutliner::onCopy(const iWidgetPtr source)
{
    _workspace->copySelected();
}

void SceneOutliner::onPaste(const iWidgetPtr source)
{
    _workspace->pasteSelected();
}

void SceneOutliner::onCut(const iWidgetPtr source)
{
    _workspace->cutSelected();
}

void SceneOutliner::refresh()
{
    if (_userControlGraphView != nullptr)
    {
        _userControlGraphView->setRootNode(_workspace->getUserScene()->getID());

        const auto &selection = _workspace->getSelection();
        if (!selection.empty())
        {
            _userControlGraphView->setSelectedNode(selection[0]);
        }
        else
        {
            _userControlGraphView->setSelectedNode(iNode::INVALID_NODE_ID);
        }
    }
}

void SceneOutliner::registerOnGraphSelectionChanged(GraphSelectionChangedDelegate graphSelectionChangedDelegate)
{
    _graphSelectionChanged.add(graphSelectionChangedDelegate);
}

void SceneOutliner::unregisterOnGraphSelectionChanged(GraphSelectionChangedDelegate graphSelectionChangedDelegate)
{
    _graphSelectionChanged.remove(graphSelectionChangedDelegate);
}

void SceneOutliner::registerOnImportFile(ImportFileDelegate importFileDelegate)
{
    _importFile.add(importFileDelegate);
}

void SceneOutliner::unregisterOnImportFile(ImportFileDelegate importFileDelegate)
{
    _importFile.remove(importFileDelegate);
}

void SceneOutliner::registerOnImportFileReference(ImportFileReferenceDelegate importFileReferenceDelegate)
{
    _importFileReference.add(importFileReferenceDelegate);
}

void SceneOutliner::unregisterOnImportFileReference(ImportFileReferenceDelegate importFileReferenceDelegate)
{
    _importFileReference.remove(importFileReferenceDelegate);
}

void SceneOutliner::addModel()
{
    if (_decisionBoxModelRef == nullptr)
    {
        _decisionBoxModelRef = new iDialogDecisionBox();
        _decisionBoxModelRef->open(iDialogCloseDelegate(this, &SceneOutliner::onAddModelDecision), "Import model", "Select type:", {"embedded", "as reference"}, 0);
    }
}

void SceneOutliner::onAddModelDecision(iDialogPtr dialog)
{
    if (_decisionBoxModelRef != dialog)
    {
        return;
    }

    if (_decisionBoxModelRef->getReturnState() == iDialogReturnState::Ok)
    {
        switch (_decisionBoxModelRef->getSelection())
        {
        case 0:
            _importFile();
            break;

        case 1:
            _importFileReference();
            break;

        default:
            con_assert(false, "invalid selection");
        }
    }

    delete _decisionBoxModelRef;
    _decisionBoxModelRef = nullptr;
}
