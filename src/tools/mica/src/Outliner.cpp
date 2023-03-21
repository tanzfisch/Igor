#include "Outliner.h"

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/dialogs/iDialog.h>
#include <igor/scene/nodes/iNode.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodeLight.h>
#include <igor/scene/nodes/iNodeSwitch.h>
#include <igor/scene/nodes/iNodeManager.h>

#include <igor/ui/widgets/iWidgetScroll.h>
#include <igor/ui/widgets/iWidgetGrid.h>
#include <igor/ui/dialogs/iDialog.h>
#include <igor/ui/widgets/iWidgetButton.h>

#include <igor/ui/widgets/iWidgetSpacer.h>
#include <igor/ui/widgets/iWidgetCheckBox.h>
#include <igor/ui/dialogs/iDialogMessageBox.h>
#include <igor/ui/dialogs/iDialogDecisionBox.h>
using namespace igor;

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include "usercontrols/UserControlTransformation.h"
#include "usercontrols/UserControlLight.h"

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

    setWidth(350);
    setHorizontalAlignment(iHorizontalAlignment::Left);
    setVerticalAlignment(iVerticalAlignment::Strech);

    _grid = new iWidgetGrid(this);
    _grid->setBorder(2);
    _grid->setCellSpacing(8);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _grid->setVerticalAlignment(iVerticalAlignment::Strech);
    _grid->appendRows(2);
    _grid->setStrechRow(2);
    _grid->setStrechColumn(0);

    iWidgetGridPtr gridButtons = new iWidgetGrid();
    gridButtons->setBorder(0);
    gridButtons->setCellSpacing(2);
    gridButtons->setHorizontalAlignment(iHorizontalAlignment::Left);
    gridButtons->setVerticalAlignment(iVerticalAlignment::Top);
    gridButtons->appendColumns(8);

    iWidgetGridPtr gridRadioButtons = new iWidgetGrid();
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

    iWidgetButtonPtr loadButton = new iWidgetButton();
    loadButton->setText("");
    loadButton->setWidth(30);
    loadButton->setHeight(30);
    loadButton->setTooltip("Load file");
    loadButton->setTexture("igor/icons/load.png");
    loadButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onLoadFile));

    iWidgetButtonPtr saveButton = new iWidgetButton();
    saveButton->setText("");
    saveButton->setWidth(30);
    saveButton->setHeight(30);
    saveButton->setTooltip("Save file");
    saveButton->setTexture("igor/icons/save.png");
    saveButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onSaveFile));

    iWidgetButtonPtr exitButton = new iWidgetButton();
    exitButton->setText("");
    exitButton->setWidth(30);
    exitButton->setHeight(30);
    exitButton->setTooltip("Exit Mica");
    exitButton->setTexture("igor/icons/exit.png");
    exitButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onExitMica));

    iWidgetButtonPtr cutButton = new iWidgetButton();
    cutButton->setText("");
    cutButton->setWidth(30);
    cutButton->setHeight(30);
    cutButton->setTooltip("Cut selection");
    cutButton->setTexture("igor/icons/cut.png");
    cutButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onCut));

    iWidgetButtonPtr copyButton = new iWidgetButton();
    copyButton->setText("");
    copyButton->setWidth(30);
    copyButton->setHeight(30);
    copyButton->setTooltip("Copy selection");
    copyButton->setTexture("igor/icons/copy.png");
    copyButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onCopy));

    iWidgetButtonPtr pasteButton = new iWidgetButton();
    pasteButton->setText("");
    pasteButton->setWidth(30);
    pasteButton->setHeight(30);
    pasteButton->setTooltip("paste from clipboard");
    pasteButton->setTexture("igor/icons/paste.png");
    pasteButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onPaste));

    iWidgetButtonPtr deleteButton = new iWidgetButton();
    deleteButton->setText("");
    deleteButton->setWidth(30);
    deleteButton->setHeight(30);
    deleteButton->setTooltip("delete selection");
    deleteButton->setTexture("igor/icons/delete.png");
    deleteButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onDelete));

    _grid->addWidget(gridButtons, 0, 0);
    gridButtons->addWidget(loadButton, 0, 0);
    gridButtons->addWidget(saveButton, 1, 0);
    gridButtons->addWidget(exitButton, 2, 0);
    gridButtons->addWidget(new iWidgetSpacer(2, 20), 3, 0);
    gridButtons->addWidget(cutButton, 4, 0);
    gridButtons->addWidget(copyButton, 5, 0);
    gridButtons->addWidget(pasteButton, 6, 0);
    gridButtons->addWidget(new iWidgetSpacer(2, 20), 7, 0);
    gridButtons->addWidget(deleteButton, 8, 0);

    _grid->addWidget(gridRadioButtons, 0, 1);
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

    _grid->addWidget(_userControlMaterialView, 0, 2);
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

    _grid->addWidget(_userControlGraphView, 0, 2);
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

void Outliner::onLoadFile(const iWidgetPtr source)
{
    _loadFile();
}

void Outliner::onSaveFile(const iWidgetPtr source)
{
    _saveFile();
}

void Outliner::onExitMica(const iWidgetPtr source)
{
    _exitMica();
}

void Outliner::registerOnCopyNode(CopyNodeDelegate copyNodeDelegate)
{
    _copyNode.append(copyNodeDelegate);
}

void Outliner::unregisterOnCopyNode(CopyNodeDelegate copyNodeDelegate)
{
    _copyNode.remove(copyNodeDelegate);
}

void Outliner::registerOnPasteNode(PasteNodeDelegate pasteNodeDelegate)
{
    _pasteNode.append(pasteNodeDelegate);
}

void Outliner::unregisterOnPasteNode(PasteNodeDelegate pasteNodeDelegate)
{
    _pasteNode.remove(pasteNodeDelegate);
}

void Outliner::registerOnGraphSelectionChanged(GraphSelectionChangedDelegate graphSelectionChangedDelegate)
{
    _graphSelectionChanged.append(graphSelectionChangedDelegate);
}

void Outliner::unregisterOnGraphSelectionChanged(GraphSelectionChangedDelegate graphSelectionChangedDelegate)
{
    _graphSelectionChanged.remove(graphSelectionChangedDelegate);
}

void Outliner::registerOnCutNode(CutNodeDelegate cutNodeDelegate)
{
    _cutNode.append(cutNodeDelegate);
}

void Outliner::unregisterOnCutNode(CutNodeDelegate cutNodeDelegate)
{
    _cutNode.remove(cutNodeDelegate);
}

void Outliner::registerOnImportFile(ImportFileDelegate importFileDelegate)
{
    _importFile.append(importFileDelegate);
}

void Outliner::unregisterOnImportFile(ImportFileDelegate importFileDelegate)
{
    _importFile.remove(importFileDelegate);
}

void Outliner::registerOnImportFileReference(ImportFileReferenceDelegate importFileReferenceDelegate)
{
    _importFileReference.append(importFileReferenceDelegate);
}

void Outliner::unregisterOnImportFileReference(ImportFileReferenceDelegate importFileReferenceDelegate)
{
    _importFileReference.remove(importFileReferenceDelegate);
}

void Outliner::registerOnLoadFile(LoadFileDelegate loadFileDelegate)
{
    _loadFile.append(loadFileDelegate);
}

void Outliner::unregisterOnLoadFile(LoadFileDelegate loadFileDelegate)
{
    _loadFile.remove(loadFileDelegate);
}

void Outliner::registerOnSaveFile(SaveFileDelegate saveFileDelegate)
{
    _saveFile.append(saveFileDelegate);
}

void Outliner::unregisterOnSaveFile(SaveFileDelegate saveFileDelegate)
{
    _saveFile.remove(saveFileDelegate);
}

void Outliner::registerOnExitMica(ExitMicaDelegate exitMicaDelegate)
{
    _exitMica.append(exitMicaDelegate);
}

void Outliner::unregisterOnExitMica(ExitMicaDelegate exitMicaDelegate)
{
    _exitMica.remove(exitMicaDelegate);
}

void Outliner::addModel()
{
    if (_decisionBoxModelRef == nullptr)
    {
        _decisionBoxModelRef = new iDialogDecisionBox();
        _decisionBoxModelRef->open(iDialogCloseDelegate(this, &Outliner::onAddModelDecision), "Import model ...", {"embedded", "as reference"}, 0);
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
    _materialSelectionChanged.append(materialSelectionChangedDelegate);
}

void Outliner::unregisterOnMaterialSelectionChanged(MaterialSelectionChangedDelegate materialSelectionChangedDelegate)
{
    _materialSelectionChanged.remove(materialSelectionChangedDelegate);
}

void Outliner::registerOnAddMaterial(AddMaterialDelegate addMaterialDelegate)
{
    _addMaterial.append(addMaterialDelegate);
}

void Outliner::unregisterOnAddMaterial(AddMaterialDelegate addMaterialDelegate)
{
    _addMaterial.remove(addMaterialDelegate);
}

void Outliner::registerOnLoadMaterial(LoadMaterialDelegate addMaterialDelegate)
{
    _loadMaterial.append(addMaterialDelegate);
}
void Outliner::unregisterOnLoadMaterial(LoadMaterialDelegate addMaterialDelegate)
{
    _loadMaterial.remove(addMaterialDelegate);
}
