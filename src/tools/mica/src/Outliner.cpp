#include "Outliner.h"

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/dialogs/iDialog.h>
#include <igor/graphics/scene/nodes/iNode.h>
#include <igor/graphics/scene/nodes/iNodeTransform.h>
#include <igor/graphics/scene/nodes/iNodeLight.h>
#include <igor/graphics/scene/nodes/iNodeSwitch.h>
#include <igor/graphics/scene/nodes/iNodeManager.h>

#include <igor/ui/widgets/iWidgetScroll.h>
#include <igor/ui/widgets/iWidgetGrid.h>
#include <igor/ui/dialogs/iDialog.h>
#include <igor/ui/widgets/iWidgetButton.h>

#include <igor/ui/widgets/iWidgetSpacer.h>
#include <igor/ui/widgets/iWidgetCheckBox.h>
#include <igor/ui/dialogs/iDialogMessageBox.h>
#include <igor/ui/dialogs/iDialogDecisionBox.h>
using namespace Igor;

#include <iaux/system/iaConsole.h>
using namespace IgorAux;

#include "UserControlTransformation.h"
#include "UserControlLight.h"

Outliner::Outliner()
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
    gridButtons->appendCollumns(8);

    iWidgetGridPtr gridRadioButtons = new iWidgetGrid();
    gridRadioButtons->setBorder(0);
    gridRadioButtons->setCellSpacing(2);
    gridRadioButtons->setHorizontalAlignment(iHorizontalAlignment::Left);
    gridRadioButtons->setVerticalAlignment(iVerticalAlignment::Top);
    gridRadioButtons->appendCollumns(1);

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
    loadButton->setTexture("icons\\load.png");
    loadButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onLoadFile));

    iWidgetButtonPtr saveButton = new iWidgetButton();
    saveButton->setText("");
    saveButton->setWidth(30);
    saveButton->setHeight(30);
    saveButton->setTooltip("Save file");
    saveButton->setTexture("icons\\save.png");
    saveButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onSaveFile));

    iWidgetButtonPtr exitButton = new iWidgetButton();
    exitButton->setText("");
    exitButton->setWidth(30);
    exitButton->setHeight(30);
    exitButton->setTooltip("Exit Mica");
    exitButton->setTexture("icons\\exit.png");
    exitButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onExitMica));

    iWidgetSpacerPtr spacer1 = new iWidgetSpacer();
    spacer1->setWidth(2);
    spacer1->setHeight(20);

    iWidgetSpacerPtr spacer2 = new iWidgetSpacer();
    spacer2->setWidth(2);
    spacer2->setHeight(20);

    iWidgetButtonPtr cutButton = new iWidgetButton();
    cutButton->setText("");
    cutButton->setWidth(30);
    cutButton->setHeight(30);
    cutButton->setTooltip("Cut selection");
    cutButton->setTexture("icons\\cut.png");
    cutButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onCut));

    iWidgetButtonPtr copyButton = new iWidgetButton();
    copyButton->setText("");
    copyButton->setWidth(30);
    copyButton->setHeight(30);
    copyButton->setTooltip("Copy selection");
    copyButton->setTexture("icons\\copy.png");
    copyButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onCopy));

    iWidgetButtonPtr pasteButton = new iWidgetButton();
    pasteButton->setText("");
    pasteButton->setWidth(30);
    pasteButton->setHeight(30);
    pasteButton->setTooltip("paste from clipboard");
    pasteButton->setTexture("icons\\paste.png");
    pasteButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onPaste));

    iWidgetButtonPtr deleteButton = new iWidgetButton();
    deleteButton->setText("");
    deleteButton->setWidth(30);
    deleteButton->setHeight(30);
    deleteButton->setTooltip("delete selection");
    deleteButton->setTexture("icons\\delete.png");
    deleteButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onDelete));

    _grid->addWidget(gridButtons, 0, 0);
    gridButtons->addWidget(loadButton, 0, 0);
    gridButtons->addWidget(saveButton, 1, 0);
    gridButtons->addWidget(exitButton, 2, 0);
    gridButtons->addWidget(spacer1, 3, 0);
    gridButtons->addWidget(cutButton, 4, 0);
    gridButtons->addWidget(copyButton, 5, 0);
    gridButtons->addWidget(pasteButton, 6, 0);
    gridButtons->addWidget(spacer2, 7, 0);
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

void Outliner::onGraphViewSelected(const iWidgetPtr source)
{
    setViewType(ViewType::GraphView);
}

void Outliner::onMaterialViewSelected(const iWidgetPtr source)
{
    setViewType(ViewType::MaterialView);
}

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
        _userControlMaterialView->registerOnMaterialSelectionChanged(MaterialSelectionChangedDelegate(this, &Outliner::onMaterialSelectionChanged));
        _userControlMaterialView->registerOnAddMaterial(AddMaterialDelegate(this, &Outliner::onAddMaterial));
    }

    _grid->addWidget(_userControlMaterialView, 0, 2);
    refreshView();
}

void Outliner::deinitGraphView()
{
    if (_userControlGraphView != nullptr)
    {
        _userControlGraphView->unregisterOnSelectionChange(GraphSelectionChangedDelegate(this, &Outliner::onGraphSelectionChanged));
        _userControlGraphView->unregisterOnAddEmitter(AddEmitterDelegate(this, &Outliner::onAddEmitter));
        _userControlGraphView->unregisterOnAddGroup(AddGroupDelegate(this, &Outliner::onAddGroup));
        _userControlGraphView->unregisterOnAddModel(AddModelDelegate(this, &Outliner::onAddModel));
        _userControlGraphView->unregisterOnAddParticleSystem(AddParticleSystemDelegate(this, &Outliner::onAddParticleSystem));
        _userControlGraphView->unregisterOnAddSwitch(AddSwitchDelegate(this, &Outliner::onAddSwitch));
        _userControlGraphView->unregisterOnAddTransformation(AddTransformationDelegate(this, &Outliner::onAddTransformation));

        _grid->removeWidget(_userControlGraphView);
    }
}

void Outliner::initGraphView()
{
    if (_userControlGraphView == nullptr)
    {
        _userControlGraphView = new UserControlGraphView();
        _userControlGraphView->registerOnSelectionChange(GraphSelectionChangedDelegate(this, &Outliner::onGraphSelectionChanged));
        _userControlGraphView->registerOnAddEmitter(AddEmitterDelegate(this, &Outliner::onAddEmitter));
        _userControlGraphView->registerOnAddGroup(AddGroupDelegate(this, &Outliner::onAddGroup));
        _userControlGraphView->registerOnAddModel(AddModelDelegate(this, &Outliner::onAddModel));
        _userControlGraphView->registerOnAddParticleSystem(AddParticleSystemDelegate(this, &Outliner::onAddParticleSystem));
        _userControlGraphView->registerOnAddSwitch(AddSwitchDelegate(this, &Outliner::onAddSwitch));
        _userControlGraphView->registerOnAddTransformation(AddTransformationDelegate(this, &Outliner::onAddTransformation));
    }

    _grid->addWidget(_userControlGraphView, 0, 2);
    refreshView();
}

void Outliner::duplicateSelected()
{
    if (_userControlGraphView == nullptr)
    {
        return;
    }

    iNodePtr node = iNodeManager::getInstance().getNode(_userControlGraphView->getSelectedNode());
    if (node == nullptr)
    {
        return;
    }

    auto parent = node->getParent();
    if (parent == nullptr)
    {
        return;
    }

    _cutNodeID = 0;
    _copiedNodeID = node->getID();

    setSelectedNode(parent);

    pasteSelected();
}

void Outliner::cutSelected()
{
    _copiedNodeID = 0;

    if (_userControlGraphView != nullptr)
    {
        iNodePtr node = iNodeManager::getInstance().getNode(_userControlGraphView->getSelectedNode());
        if (node != nullptr)
        {
            _cutNodeID = node->getID();
        }
        else
        {
            _cutNodeID = iNode::INVALID_NODE_ID;
        }
    }
}

void Outliner::pasteSelected()
{
    if (_userControlGraphView != nullptr)
    {
        if (_copiedNodeID != 0)
        {
            iNodePtr pasteNode = nullptr;
            iNodePtr copiedNode = iNodeManager::getInstance().getNode(_copiedNodeID);
            if (copiedNode != nullptr)
            {
                pasteNode = iNodeManager::getInstance().createCopy(copiedNode);
            }

            if (pasteNode != nullptr)
            {
                iNodePtr destination = iNodeManager::getInstance().getNode(_userControlGraphView->getSelectedNode());

                if (destination == nullptr)
                {
                    destination = iNodeManager::getInstance().getNode(_rootNodeID);
                }

                if (destination != nullptr)
                {
                    destination->insertNode(pasteNode);
                    refreshView();
                    setSelectedNode(pasteNode);
                }
            }
        }
        else if (_cutNodeID != 0)
        {
            iNodePtr cutNode = iNodeManager::getInstance().getNode(_cutNodeID);
            if (cutNode != nullptr)
            {
                iNodePtr destination = iNodeManager::getInstance().getNode(_userControlGraphView->getSelectedNode());

                if (destination == nullptr)
                {
                    destination = iNodeManager::getInstance().getNode(_rootNodeID);
                }

                if (destination != nullptr)
                {
                    iNodePtr parent = cutNode->getParent();
                    if (parent != nullptr)
                    {
                        parent->removeNode(cutNode);

                        destination->insertNode(cutNode);
                        refreshView();
                        setSelectedNode(cutNode);
                    }
                }
            }
        }
        else
        {
            _messageBox->open("clipboard is empty");
        }
    }
}

void Outliner::copySelected()
{
    if (_userControlGraphView != nullptr)
    {
        _cutNodeID = 0;

        iNodePtr node = iNodeManager::getInstance().getNode(_userControlGraphView->getSelectedNode());
        if (node != nullptr)
        {
            _copiedNodeID = node->getID();
        }
        else
        {
            _copiedNodeID = 0;
        }
    }
}

void Outliner::deleteSelected()
{
    if (_userControlGraphView != nullptr)
    {
        _graphSelectionChanged(iNode::INVALID_NODE_ID);

        iNodePtr node = iNodeManager::getInstance().getNode(_userControlGraphView->getSelectedNode());
        if (node != nullptr)
        {
            iNodePtr parent = node->getParent();
            if (parent != nullptr)
            {
                parent->removeNode(node);
                iNodeManager::getInstance().destroyNodeAsync(node);
                refreshView();
            }
            else
            {
                _messageBox->open("can't delete root node");
            }
        }
    }
}

void Outliner::onDelete(const iWidgetPtr source)
{
    deleteSelected();
}

void Outliner::onCopy(const iWidgetPtr source)
{
    copySelected();
}

void Outliner::onPaste(const iWidgetPtr source)
{
    pasteSelected();
}

void Outliner::onCut(const iWidgetPtr source)
{
    cutSelected();
}

void Outliner::onGraphSelectionChanged(uint64 nodeID)
{
    _graphSelectionChanged(nodeID);
}

void Outliner::onMaterialSelectionChanged(uint64 materialID)
{
    _materialSelectionChanged(materialID);
}

void Outliner::onAddMaterial()
{
    _addMaterial();
}

void Outliner::setSelectedNode(iNodePtr node)
{
    if (_userControlGraphView != nullptr)
    {
        if (node != nullptr)
        {
            _userControlGraphView->setSelectedNode(node->getID());
        }
        else
        {
            _userControlGraphView->setSelectedNode(iNode::INVALID_NODE_ID);
        }
    }
}

void Outliner::setRootNode(iNodePtr root)
{
    con_assert(root != nullptr, "zero pointer");

    if (root != nullptr)
    {
        _rootNodeID = root->getID();
        if (_userControlGraphView != nullptr)
        {
            _userControlGraphView->setRootNode(_rootNodeID);
        }
    }
}

void Outliner::refreshView()
{
    if (_userControlGraphView != nullptr)
    {
        _userControlGraphView->setRootNode(_rootNodeID);
    }

    if (_userControlMaterialView != nullptr)
    {
        _userControlMaterialView->refresh();
    }
}

void Outliner::fileOpen()
{
    _loadFile();
}

void Outliner::fileSave()
{
    _saveFile();
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

void Outliner::registerOnAddTransformation(AddTransformationDelegate addTransformationDelegate)
{
    _addTransformation.append(addTransformationDelegate);
}

void Outliner::unregisterOnAddTransformation(AddTransformationDelegate addTransformationDelegate)
{
    _addTransformation.remove(addTransformationDelegate);
}

void Outliner::registerOnAddGroup(AddGroupDelegate addGroupDelegate)
{
    _addGroup.append(addGroupDelegate);
}

void Outliner::unregisterOnAddGroup(AddGroupDelegate addGroupDelegate)
{
    _addGroup.remove(addGroupDelegate);
}

void Outliner::registerOnAddEmitter(AddEmitterDelegate addEmitterDelegate)
{
    _addEmitter.append(addEmitterDelegate);
}

void Outliner::unregisterOnAddEmitter(AddEmitterDelegate addEmitterDelegate)
{
    _addEmitter.remove(addEmitterDelegate);
}

void Outliner::registerOnAddParticleSystem(AddParticleSystemDelegate addParticleSystemDelegate)
{
    _addParticleSystem.append(addParticleSystemDelegate);
}

void Outliner::unregisterOnAddParticleSystem(AddParticleSystemDelegate addParticleSystemDelegate)
{
    _addParticleSystem.remove(addParticleSystemDelegate);
}

void Outliner::registerOnAddSwitch(AddSwitchDelegate addSwitchDelegate)
{
    _addSwitch.append(addSwitchDelegate);
}

void Outliner::unregisterOnAddSwitch(AddSwitchDelegate addSwitchDelegate)
{
    _addSwitch.remove(addSwitchDelegate);
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

void Outliner::onAddModel(uint64 addAt)
{
    if (_decisionBoxModelRef == nullptr)
    {
        _decisionBoxModelRef = new iDialogDecisionBox();
        _decisionBoxModelRef->open(iDialogCloseDelegate(this, &Outliner::onAddModelDecision), "Import model ...", { "embedded", "as reference" }, 0);
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

void Outliner::onAddTransformation(uint64 addAt)
{
    _addTransformation(addAt);
}

void Outliner::onAddGroup(uint64 addAt)
{
    _addGroup(addAt);
}

void Outliner::onAddEmitter(uint64 addAt)
{
    _addEmitter(addAt);
}

void Outliner::onAddParticleSystem(uint64 addAt)
{
    _addParticleSystem(addAt);
}

void Outliner::onAddSwitch(uint64 addAt)
{
    _addSwitch(addAt);
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