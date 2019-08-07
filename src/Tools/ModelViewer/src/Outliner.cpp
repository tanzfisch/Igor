#include "Outliner.h"

#include <iWidgetManager.h>
#include <iDialog.h>
#include <iNode.h>
#include <iNodeTransform.h>
#include <iNodeLight.h>
#include <iNodeSwitch.h>
#include <iNodeFactory.h>

#include <iWidgetScroll.h>
#include <iWidgetGrid.h>
#include <iDialog.h>
#include <iWidgetButton.h>
#include <iWidgetSpacer.h>
#include <iWidgetCheckBox.h>
#include <iDialogMessageBox.h>
#include <iDialogDecisionBox.h>
using namespace Igor;

#include <iaConsole.h>
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
    _messageBox = iWidgetManager::getInstance().createWidget<iDialogMessageBox>();

    setWidth(350);
    setHorizontalAlignment(iHorizontalAlignment::Left);
    setVerticalAlignment(iVerticalAlignment::Strech);

    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _grid->setBorder(2);
    _grid->setCellSpacing(8);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _grid->setVerticalAlignment(iVerticalAlignment::Strech);
    _grid->appendRows(2);
    _grid->setStrechRow(2);
    _grid->setStrechColumn(0);

    _gridButtons = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _gridButtons->setBorder(0);
    _gridButtons->setCellSpacing(2);
    _gridButtons->setHorizontalAlignment(iHorizontalAlignment::Left);
    _gridButtons->setVerticalAlignment(iVerticalAlignment::Top);
    _gridButtons->appendCollumns(8);

    _gridRadioButtons = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _gridRadioButtons->setBorder(0);
    _gridRadioButtons->setCellSpacing(2);
    _gridRadioButtons->setHorizontalAlignment(iHorizontalAlignment::Left);
    _gridRadioButtons->setVerticalAlignment(iVerticalAlignment::Top);
    _gridRadioButtons->appendCollumns(1);

    iWidgetCheckBox::beginRadioButtonGroup();
    _checkBoxGraph = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox"));
    _checkBoxGraph->setText("Graph");
    _checkBoxGraph->registerOnClickEvent(iClickDelegate(this, &Outliner::onGraphViewSelected));
    _checkBoxMaterial = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox"));
    _checkBoxMaterial->setText("Material");
    _checkBoxMaterial->registerOnClickEvent(iClickDelegate(this, &Outliner::onMaterialViewSelected));
    iWidgetCheckBox::endRadioButtonGroup();
    _checkBoxGraph->setChecked();

    _loadButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _loadButton->setText("");
    _loadButton->setWidth(30);
    _loadButton->setHeight(30);
	_loadButton->setTooltip("Load file");
    _loadButton->setTexture("icons\\load.png");
    _loadButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onLoadFile));

    _saveButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _saveButton->setText("");
    _saveButton->setWidth(30);
    _saveButton->setHeight(30);
	_saveButton->setTooltip("Save file");
    _saveButton->setTexture("icons\\save.png");
    _saveButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onSaveFile));

    _exitButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _exitButton->setText("");
    _exitButton->setWidth(30);
    _exitButton->setHeight(30);
	_exitButton->setTooltip("Exit ModelViewer");
    _exitButton->setTexture("icons\\exit.png");
    _exitButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onExitModelViewer));

    _spacer1 = static_cast<iWidgetSpacer*>(iWidgetManager::getInstance().createWidget("Spacer"));
    _spacer1->setWidth(2);
    _spacer1->setHeight(20);

    _spacer2 = static_cast<iWidgetSpacer*>(iWidgetManager::getInstance().createWidget("Spacer"));
    _spacer2->setWidth(2);
    _spacer2->setHeight(20);

    _cutButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _cutButton->setText("");
    _cutButton->setWidth(30);
    _cutButton->setHeight(30);
	_cutButton->setTooltip("Cut selection");
    _cutButton->setTexture("icons\\cut.png");
    _cutButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onCut));

    _copyButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _copyButton->setText("");
    _copyButton->setWidth(30);
    _copyButton->setHeight(30);
	_copyButton->setTooltip("Copy selection");
    _copyButton->setTexture("icons\\copy.png");
    _copyButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onCopy));

    _pasteButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _pasteButton->setText("");
    _pasteButton->setWidth(30);
    _pasteButton->setHeight(30);
	_pasteButton->setTooltip("paste from clipboard");
    _pasteButton->setTexture("icons\\paste.png");
    _pasteButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onPaste));

    _deleteButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    _deleteButton->setText("");
    _deleteButton->setWidth(30);
    _deleteButton->setHeight(30);
	_deleteButton->setTooltip("delete selection");
    _deleteButton->setTexture("icons\\delete.png");
    _deleteButton->registerOnClickEvent(iClickDelegate(this, &Outliner::onDelete));

    addWidget(_grid);
    _grid->addWidget(_gridButtons, 0, 0);
    _gridButtons->addWidget(_loadButton, 0, 0);
    _gridButtons->addWidget(_saveButton, 1, 0);
    _gridButtons->addWidget(_exitButton, 2, 0);
    _gridButtons->addWidget(_spacer1, 3, 0);
    _gridButtons->addWidget(_cutButton, 4, 0);
    _gridButtons->addWidget(_copyButton, 5, 0);
    _gridButtons->addWidget(_pasteButton, 6, 0);
    _gridButtons->addWidget(_spacer2, 7, 0);
    _gridButtons->addWidget(_deleteButton, 8, 0);

    _grid->addWidget(_gridRadioButtons, 0, 1);
    _gridRadioButtons->addWidget(_checkBoxGraph, 0, 0);
    _gridRadioButtons->addWidget(_checkBoxMaterial, 1, 0);

    setViewType(ViewType::GraphView);
}

void Outliner::onGraphViewSelected(iWidget* source)
{
    setViewType(ViewType::GraphView);
}

void Outliner::onMaterialViewSelected(iWidget* source)
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

		iWidgetManager::getInstance().destroyWidget(_userControlMaterialView);
        _userControlMaterialView = nullptr;
    }
}

void Outliner::initMaterialView()
{
    if (_userControlMaterialView == nullptr)
    {
		_userControlMaterialView = new UserControlMaterialView();
        _userControlMaterialView->registerOnMaterialSelectionChanged(MaterialSelectionChangedDelegate(this, &Outliner::onMaterialSelectionChanged));
        _userControlMaterialView->registerOnAddMaterial(AddMaterialDelegate(this, &Outliner::onAddMaterial));

        _grid->addWidget(_userControlMaterialView, 0, 2);
    }
    else
    {
        con_err("internal error");
    }
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
		iWidgetManager::getInstance().destroyWidget(_userControlGraphView);
        _userControlGraphView = nullptr;
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

        _grid->addWidget(_userControlGraphView, 0, 2);

        refreshView();
    }
    else
    {
        con_err("internal error");
    }
}

void Outliner::duplicateSelected()
{
	if (_userControlGraphView == nullptr)
	{
		return;
	}

	iNodePtr node = iNodeFactory::getInstance().getNode(_userControlGraphView->getSelectedNode());
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
		iNodePtr node = iNodeFactory::getInstance().getNode(_userControlGraphView->getSelectedNode());
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
			iNodePtr copiedNode = iNodeFactory::getInstance().getNode(_copiedNodeID);
			if (copiedNode != nullptr)
			{
				pasteNode = iNodeFactory::getInstance().createCopy(copiedNode);
			}

			if (pasteNode != nullptr)
			{
				iNodePtr destination = iNodeFactory::getInstance().getNode(_userControlGraphView->getSelectedNode());

				if (destination == nullptr)
				{
					destination = iNodeFactory::getInstance().getNode(_rootNodeID);
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
			iNodePtr cutNode = iNodeFactory::getInstance().getNode(_cutNodeID);
			if (cutNode != nullptr)
			{
				iNodePtr destination = iNodeFactory::getInstance().getNode(_userControlGraphView->getSelectedNode());

				if (destination == nullptr)
				{
					destination = iNodeFactory::getInstance().getNode(_rootNodeID);
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
			_messageBox->show("clipboard is empty");
		}
	}
}

void Outliner::copySelected()
{
	if (_userControlGraphView != nullptr)
	{
		_cutNodeID = 0;

		iNodePtr node = iNodeFactory::getInstance().getNode(_userControlGraphView->getSelectedNode());
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

		iNodePtr node = iNodeFactory::getInstance().getNode(_userControlGraphView->getSelectedNode());
		if (node != nullptr)
		{
			iNodePtr parent = node->getParent();
			if (parent != nullptr)
			{
				parent->removeNode(node);
				iNodeFactory::getInstance().destroyNodeAsync(node);
				refreshView();
			}
			else
			{
				_messageBox->show("can't delete root node");
			}
		}
	}
}

void Outliner::onDelete(iWidget* source)
{
	deleteSelected();
}

void Outliner::onCopy(iWidget* source)
{
	copySelected();
}

void Outliner::onPaste(iWidget* source)
{
	pasteSelected();
}

void Outliner::onCut(iWidget* source)
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

void Outliner::deinitGUI()
{
    _checkBoxGraph->unregisterOnClickEvent(iClickDelegate(this, &Outliner::onGraphViewSelected));
    _checkBoxMaterial->unregisterOnClickEvent(iClickDelegate(this, &Outliner::onMaterialViewSelected));
    _loadButton->unregisterOnClickEvent(iClickDelegate(this, &Outliner::onLoadFile));
    _saveButton->unregisterOnClickEvent(iClickDelegate(this, &Outliner::onSaveFile));
    _exitButton->unregisterOnClickEvent(iClickDelegate(this, &Outliner::onExitModelViewer));

    _cutButton->unregisterOnClickEvent(iClickDelegate(this, &Outliner::onCut));
    _copyButton->unregisterOnClickEvent(iClickDelegate(this, &Outliner::onCopy));
    _pasteButton->unregisterOnClickEvent(iClickDelegate(this, &Outliner::onPaste));
    _deleteButton->unregisterOnClickEvent(iClickDelegate(this, &Outliner::onDelete));

    deinitGraphView();
    deinitMaterialView();

    removeWidget(_grid);
	iWidgetManager::getInstance().destroyWidget(_grid);
	_grid = nullptr;

    if (_messageBox != nullptr)
    {
        iWidgetManager::getInstance().destroyDialog(_messageBox);
        _messageBox = nullptr;
    }

    if (_decisionBoxModelRef != nullptr)
    {
        iWidgetManager::getInstance().destroyDialog(_decisionBoxModelRef);
        _decisionBoxModelRef = nullptr;
    }
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

void Outliner::onLoadFile(iWidget* source)
{
    _loadFile();
}

void Outliner::onSaveFile(iWidget* source)
{
    _saveFile();
}

void Outliner::onExitModelViewer(iWidget* source)
{
    _exitModelViewer();
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

void Outliner::registerOnExitModelViewer(ExitModelViewerDelegate exitModelViewerDelegate)
{
    _exitModelViewer.append(exitModelViewerDelegate);
}

void Outliner::unregisterOnExitModelViewer(ExitModelViewerDelegate exitModelViewerDelegate)
{
    _exitModelViewer.remove(exitModelViewerDelegate);
}

void Outliner::onAddModel(uint64 addAt)
{
    if (_decisionBoxModelRef == nullptr)
    {
        _decisionBoxModelRef = iWidgetManager::getInstance().createWidget<iDialogDecisionBox>();
    }

    _decisionBoxModelRef->show("Import model ...", iDecisionBoxCloseDelegate(this, &Outliner::onAddModelDecision), { "embedded", "as reference" }, 0);
}

void Outliner::onAddModelDecision(bool ok, int32 selection)
{
    if (_userControlGraphView != nullptr)
    {
        if (ok)
        {
            switch (selection)
            {
            case 0:
                _importFile(_userControlGraphView->getSelectedNode());
                break;

            case 1:
                _importFileReference(_userControlGraphView->getSelectedNode());
                break;

            default:
                con_assert(false, "invalid selection");
            }
        }
    }
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