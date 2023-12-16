// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "Outliner.h"

Outliner::Outliner(WorkspacePtr workspace)
    : _workspace(workspace)
{
    initGUI();
}

Outliner::~Outliner()
{
    deinitGUI();
}

void Outliner::initGUI()
{
    _messageBox = new iDialogMessageBox();

    setDockable(true);
    setTitle("Outliner");
    setMinWidth(350);
    setHorizontalAlignment(iHorizontalAlignment::Left);
    setVerticalAlignment(iVerticalAlignment::Stretch);

    _grid = new iWidgetGridLayout(this);
    _grid->setBorder(2);
    _grid->setCellSpacing(8);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    _grid->setVerticalAlignment(iVerticalAlignment::Stretch);
    _grid->appendRows(1);
    _grid->setStretchRow(1);
    _grid->setStretchColumn(0);

    iWidgetGridLayoutPtr gridRadioButtons = new iWidgetGridLayout();
    gridRadioButtons->setBorder(0);
    gridRadioButtons->setCellSpacing(2);
    gridRadioButtons->setHorizontalAlignment(iHorizontalAlignment::Left);
    gridRadioButtons->setVerticalAlignment(iVerticalAlignment::Top);
    gridRadioButtons->appendColumns(1);

    iWidgetCheckBox::beginRadioButtonGroup();
    iWidgetCheckBoxPtr checkBoxGraph = new iWidgetCheckBox();
    checkBoxGraph->setText("Graph");
    checkBoxGraph->registerOnClickEvent(iClickDelegate(this, &Outliner::onGraphViewSelected));
    iWidgetCheckBoxPtr checkBoxMaterial = new iWidgetCheckBox();
    checkBoxMaterial->setText("Material");
    checkBoxMaterial->registerOnClickEvent(iClickDelegate(this, &Outliner::onMaterialViewSelected));
    iWidgetCheckBox::endRadioButtonGroup();
    checkBoxGraph->setChecked();

    _grid->addWidget(gridRadioButtons, 0, 0);
    gridRadioButtons->addWidget(checkBoxGraph, 0, 0);
    gridRadioButtons->addWidget(checkBoxMaterial, 1, 0);

    setViewType(ViewType::GraphView);
}

void Outliner::deinitGUI()
{
    if (_userControlGraphView != nullptr &&
        !_userControlGraphView->hasParent())
    {
        delete _userControlGraphView;
        _userControlGraphView = nullptr;
    }

    if (_userControlMaterialView != nullptr &&
        !_userControlMaterialView->hasParent())
    {
        delete _userControlMaterialView;
        _userControlMaterialView = nullptr;
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

// TODO replace with tabs one day
void Outliner::onGraphViewSelected(const iWidgetPtr source)
{
    setViewType(ViewType::GraphView);
}

// TODO replace with tabs one day
void Outliner::onMaterialViewSelected(const iWidgetPtr source)
{
    setViewType(ViewType::MaterialView);
}

// TODO replace with tabs one day
void Outliner::setViewType(ViewType viewType)
{
    switch (_currentView)
    {
    case ViewType::GraphView:
        deinitGraphView();
        break;

    case ViewType::MaterialView:
        deinitMaterialView();
        break;

    default:
        con_err("internal error");
    }

    _currentView = viewType;

    switch (_currentView)
    {
    case ViewType::GraphView:
        initGraphView();
        break;

    case ViewType::MaterialView:
        initMaterialView();
        break;

    default:
        con_err("internal error");
    }
}

void Outliner::deinitMaterialView()
{
    if (_userControlMaterialView != nullptr)
    {
        _userControlMaterialView->unregisterOnMaterialSelectionChanged(MaterialSelectionChangedDelegate(this, &Outliner::onMaterialSelectionChanged));
        _userControlMaterialView->unregisterOnAddMaterial(AddMaterialDelegate(this, &Outliner::onAddMaterial));

        _grid->removeWidget(_userControlMaterialView);
    }
}

void Outliner::initMaterialView()
{
    if (_userControlMaterialView == nullptr)
    {
        _userControlMaterialView = new UserControlMaterialView();
    }

    _userControlMaterialView->registerOnMaterialSelectionChanged(MaterialSelectionChangedDelegate(this, &Outliner::onMaterialSelectionChanged));
    _userControlMaterialView->registerOnAddMaterial(AddMaterialDelegate(this, &Outliner::onAddMaterial));
    _userControlMaterialView->registerOnLoadMaterial(LoadMaterialDelegate(this, &Outliner::onLoadMaterial));

    _grid->addWidget(_userControlMaterialView, 0, 1);
    refresh();
}

void Outliner::deinitGraphView()
{
    if (_userControlGraphView != nullptr)
    {
        _userControlGraphView->unregisterOnSelectionChange(GraphSelectionChangedDelegate(this, &Outliner::onGraphSelectionChanged));
        _grid->removeWidget(_userControlGraphView);
    }
}

void Outliner::initGraphView()
{
    if (_userControlGraphView == nullptr)
    {
        _userControlGraphView = new UserControlGraphView(_workspace, this);
    }

    _userControlGraphView->registerOnSelectionChange(GraphSelectionChangedDelegate(this, &Outliner::onGraphSelectionChanged));

    _grid->addWidget(_userControlGraphView, 0, 1);
    refresh();
}

void Outliner::onGraphSelectionChanged(uint64 nodeID)
{
    _graphSelectionChanged(nodeID);
}

void Outliner::onMaterialSelectionChanged(const iMaterialID &materialID)
{
    _materialSelectionChanged(materialID);
}

void Outliner::onDelete(const iWidgetPtr source)
{
    _workspace->deleteSelected();
}

void Outliner::onCopy(const iWidgetPtr source)
{
    _workspace->copySelected();
}

void Outliner::onPaste(const iWidgetPtr source)
{
    _workspace->pasteSelected();
}

void Outliner::onCut(const iWidgetPtr source)
{
    _workspace->cutSelected();
}

void Outliner::onAddMaterial()
{
    _addMaterial();
}

void Outliner::onLoadMaterial()
{
    _loadMaterial();
}

void Outliner::refresh()
{
    if (_userControlGraphView != nullptr)
    {
        _userControlGraphView->setRootNode(_workspace->getRootUser()->getID());

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

    if (_userControlMaterialView != nullptr)
    {
        _userControlMaterialView->refresh();
    }
}

void Outliner::registerOnGraphSelectionChanged(GraphSelectionChangedDelegate graphSelectionChangedDelegate)
{
    _graphSelectionChanged.add(graphSelectionChangedDelegate);
}

void Outliner::unregisterOnGraphSelectionChanged(GraphSelectionChangedDelegate graphSelectionChangedDelegate)
{
    _graphSelectionChanged.remove(graphSelectionChangedDelegate);
}

void Outliner::registerOnImportFile(ImportFileDelegate importFileDelegate)
{
    _importFile.add(importFileDelegate);
}

void Outliner::unregisterOnImportFile(ImportFileDelegate importFileDelegate)
{
    _importFile.remove(importFileDelegate);
}

void Outliner::registerOnImportFileReference(ImportFileReferenceDelegate importFileReferenceDelegate)
{
    _importFileReference.add(importFileReferenceDelegate);
}

void Outliner::unregisterOnImportFileReference(ImportFileReferenceDelegate importFileReferenceDelegate)
{
    _importFileReference.remove(importFileReferenceDelegate);
}

void Outliner::addModel()
{
    if (_decisionBoxModelRef == nullptr)
    {
        _decisionBoxModelRef = new iDialogDecisionBox();
        _decisionBoxModelRef->open(iDialogCloseDelegate(this, &Outliner::onAddModelDecision), "Import model", "Select type:", {"embedded", "as reference"}, 0);
    }
}

void Outliner::onAddModelDecision(iDialogPtr dialog)
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

void Outliner::registerOnMaterialSelectionChanged(MaterialSelectionChangedDelegate materialSelectionChangedDelegate)
{
    _materialSelectionChanged.add(materialSelectionChangedDelegate);
}

void Outliner::unregisterOnMaterialSelectionChanged(MaterialSelectionChangedDelegate materialSelectionChangedDelegate)
{
    _materialSelectionChanged.remove(materialSelectionChangedDelegate);
}

void Outliner::registerOnAddMaterial(AddMaterialDelegate addMaterialDelegate)
{
    _addMaterial.add(addMaterialDelegate);
}

void Outliner::unregisterOnAddMaterial(AddMaterialDelegate addMaterialDelegate)
{
    _addMaterial.remove(addMaterialDelegate);
}

void Outliner::registerOnLoadMaterial(LoadMaterialDelegate addMaterialDelegate)
{
    _loadMaterial.add(addMaterialDelegate);
}
void Outliner::unregisterOnLoadMaterial(LoadMaterialDelegate addMaterialDelegate)
{
    _loadMaterial.remove(addMaterialDelegate);
}
