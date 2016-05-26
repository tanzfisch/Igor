#include "MenuDialog.h"

#include <iWidgetManager.h>
#include <iWidgetDialog.h>
#include <iNode.h>
#include <iNodeTransform.h>
#include <iNodeLight.h>
#include <iNodeSwitch.h>
#include <iNodeFactory.h>

#include <iWidgetScroll.h>
#include <iWidgetGrid.h>
#include <iWidgetDialog.h>
#include <iWidgetButton.h>
#include <iWidgetGroupBox.h>
#include <iWidgetSpacer.h>
#include <iWidgetCheckBox.h>
#include <iMessageBox.h>
#include <iDecisionBox.h>
using namespace Igor;

#include <iaConsole.h>
using namespace IgorAux;

#include "UserControlTransformation.h"
#include "UserControlLight.h"

MenuDialog::MenuDialog()
{
    initGUI();
}

MenuDialog::~MenuDialog()
{
    deinitGUI();
}

void MenuDialog::initGUI()
{
    _messageBox = new iMessageBox();

	getDialog()->setWidth(300);
	getDialog()->setHorrizontalAlignment(iHorrizontalAlignment::Left);
	getDialog()->setVerticalAlignment(iVerticalAlignment::Strech);

    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allwidgets.push_back(_grid);
    _grid->setBorder(2);
    _grid->setCellSpacing(8);
	_grid->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    _grid->setVerticalAlignment(iVerticalAlignment::Strech);
    _grid->appendRows(2);
	_grid->setStrechRow(2);
    
    _gridButtons = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allwidgets.push_back(_gridButtons);
    _gridButtons->setBorder(0);
    _gridButtons->setCellSpacing(2);
    _gridButtons->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _gridButtons->setVerticalAlignment(iVerticalAlignment::Top);
	_gridButtons->appendCollumns(8);

	_gridRadioButtons = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
	_allwidgets.push_back(_gridRadioButtons);
	_gridRadioButtons->setBorder(0);
	_gridRadioButtons->setCellSpacing(2);
	_gridRadioButtons->setHorrizontalAlignment(iHorrizontalAlignment::Left);
	_gridRadioButtons->setVerticalAlignment(iVerticalAlignment::Top);
	_gridRadioButtons->appendCollumns(1);

	iWidgetCheckBox::beginRadioButtonGroup();
	_checkBoxGraph = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::CheckBox));
	_checkBoxGraph->setText("Graph");
    _checkBoxGraph->registerOnClickEvent(iClickDelegate(this, &MenuDialog::onGraphViewSelected));
	_checkBoxMaterial = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::CheckBox));
	_checkBoxMaterial->setText("Material");
    _checkBoxMaterial->registerOnClickEvent(iClickDelegate(this, &MenuDialog::onMaterialViewSelected));
	iWidgetCheckBox::endRadioButtonGroup();
	_checkBoxGraph->setChecked();

    _loadButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allwidgets.push_back(_loadButton);
    _loadButton->setText("");
    _loadButton->setWidth(30);
    _loadButton->setHeight(30);
    _loadButton->setTexture("icons\\load.png");
    _loadButton->registerOnClickEvent(iClickDelegate(this, &MenuDialog::onLoadFile));
    
    _saveButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allwidgets.push_back(_saveButton);
    _saveButton->setText("");
    _saveButton->setWidth(30);
    _saveButton->setHeight(30);
    _saveButton->setTexture("icons\\save.png");
    _saveButton->registerOnClickEvent(iClickDelegate(this, &MenuDialog::onSaveFile));

    _exitButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allwidgets.push_back(_exitButton);
    _exitButton->setText("");
    _exitButton->setWidth(30);
    _exitButton->setHeight(30);
    _exitButton->setTexture("icons\\exit.png");
    _exitButton->registerOnClickEvent(iClickDelegate(this, &MenuDialog::onExitModelViewer));

    _spacer1 = static_cast<iWidgetSpacer*>(iWidgetManager::getInstance().createWidget(iWidgetType::Spacer));
    _allwidgets.push_back(_spacer1);
    _spacer1->setWidth(2);
    _spacer1->setHeight(20);

    _spacer2 = static_cast<iWidgetSpacer*>(iWidgetManager::getInstance().createWidget(iWidgetType::Spacer));
    _allwidgets.push_back(_spacer2);
    _spacer2->setWidth(2);
    _spacer2->setHeight(20);

    _cutButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allwidgets.push_back(_cutButton);
    _cutButton->setText("");
    _cutButton->setWidth(30);
    _cutButton->setHeight(30);
    _cutButton->setTexture("icons\\cut.png");
    _cutButton->registerOnClickEvent(iClickDelegate(this, &MenuDialog::onCut));

    _copyButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allwidgets.push_back(_copyButton);
    _copyButton->setText("");
    _copyButton->setWidth(30);
    _copyButton->setHeight(30);
    _copyButton->setTexture("icons\\copy.png");
    _copyButton->registerOnClickEvent(iClickDelegate(this, &MenuDialog::onCopy));

    _pasteButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allwidgets.push_back(_pasteButton);
    _pasteButton->setText("");
    _pasteButton->setWidth(30);
    _pasteButton->setHeight(30);
    _pasteButton->setTexture("icons\\paste.png");
    _pasteButton->registerOnClickEvent(iClickDelegate(this, &MenuDialog::onPaste));

    _deleteButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allwidgets.push_back(_deleteButton);
    _deleteButton->setText("");
    _deleteButton->setWidth(30);
    _deleteButton->setHeight(30);
    _deleteButton->setTexture("icons\\delete.png");
    _deleteButton->registerOnClickEvent(iClickDelegate(this, &MenuDialog::onDelete));

    getDialog()->addWidget(_grid);
    _grid->addWidget(_gridButtons, 0, 0);
	_gridButtons->addWidget(_loadButton, 0, 0);
	_gridButtons->addWidget(_saveButton, 1, 0);
	_gridButtons->addWidget(_exitButton, 2, 0);
	_gridButtons->addWidget(_spacer1, 3, 0);
	_gridButtons->addWidget(_pasteButton, 4, 0);
	_gridButtons->addWidget(_cutButton, 5, 0);
	_gridButtons->addWidget(_copyButton, 6, 0);
	_gridButtons->addWidget(_spacer2, 7, 0);
	_gridButtons->addWidget(_deleteButton, 8, 0);

	_grid->addWidget(_gridRadioButtons, 0, 1);
	_gridRadioButtons->addWidget(_checkBoxGraph, 0, 0);
	_gridRadioButtons->addWidget(_checkBoxMaterial, 1, 0);

    setViewType(ViewType::GraphView);
}

void MenuDialog::onGraphViewSelected(iWidget* source)
{
    setViewType(ViewType::GraphView);
}

void MenuDialog::onMaterialViewSelected(iWidget* source)
{
    setViewType(ViewType::MaterialView);
}

void MenuDialog::setViewType(ViewType viewType)
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

void MenuDialog::deinitMaterialView()
{
    if (_userControlMaterialView != nullptr)
    {
		_userControlMaterialView->unregisterOnMaterialSelectionChanged(MaterialSelectionChangedDelegate(this, &MenuDialog::onMaterialSelectionChanged));
		_userControlMaterialView->unregisterOnAddMaterial(AddMaterialDelegate(this, &MenuDialog::onAddMaterial));

        delete _userControlMaterialView;
        _userControlMaterialView = nullptr;
    }
}

void MenuDialog::initMaterialView()
{
    if (_userControlMaterialView == nullptr)
    {
        _userControlMaterialView = new UserControlMaterialView();
		_userControlMaterialView->registerOnMaterialSelectionChanged(MaterialSelectionChangedDelegate(this, &MenuDialog::onMaterialSelectionChanged));
		_userControlMaterialView->registerOnAddMaterial(AddMaterialDelegate(this, &MenuDialog::onAddMaterial));

        _grid->addWidget(_userControlMaterialView->getWidget(), 0, 2);        
    }
    else
    {
        con_err("internal error");
    }
}

void MenuDialog::deinitGraphView()
{
    if (_userControlGraphView != nullptr)
    {
        _userControlGraphView->unregisterOnSelectionChange(GraphSelectionChangedDelegate(this, &MenuDialog::onGraphSelectionChanged));
        _userControlGraphView->unregisterOnAddEmitter(AddEmitterDelegate(this, &MenuDialog::onAddEmitter));
        _userControlGraphView->unregisterOnAddGroup(AddGroupDelegate(this, &MenuDialog::onAddGroup));
        _userControlGraphView->unregisterOnAddModel(AddModelDelegate(this, &MenuDialog::onAddModel));
        _userControlGraphView->unregisterOnAddParticleSystem(AddParticleSystemDelegate(this, &MenuDialog::onAddParticleSystem));
        _userControlGraphView->unregisterOnAddSwitch(AddSwitchDelegate(this, &MenuDialog::onAddSwitch));
        _userControlGraphView->unregisterOnAddTransformation(AddTransformationDelegate(this, &MenuDialog::onAddTransformation));
        delete _userControlGraphView;
        _userControlGraphView = nullptr;
    }
}

void MenuDialog::initGraphView()
{
    if (_userControlGraphView == nullptr)
    {
        _userControlGraphView = new UserControlGraphView();
        _userControlGraphView->registerOnSelectionChange(GraphSelectionChangedDelegate(this, &MenuDialog::onGraphSelectionChanged));
        _userControlGraphView->registerOnAddEmitter(AddEmitterDelegate(this, &MenuDialog::onAddEmitter));
        _userControlGraphView->registerOnAddGroup(AddGroupDelegate(this, &MenuDialog::onAddGroup));
        _userControlGraphView->registerOnAddModel(AddModelDelegate(this, &MenuDialog::onAddModel));
        _userControlGraphView->registerOnAddParticleSystem(AddParticleSystemDelegate(this, &MenuDialog::onAddParticleSystem));
        _userControlGraphView->registerOnAddSwitch(AddSwitchDelegate(this, &MenuDialog::onAddSwitch));
        _userControlGraphView->registerOnAddTransformation(AddTransformationDelegate(this, &MenuDialog::onAddTransformation));

        _grid->addWidget(_userControlGraphView->getWidget(), 0, 2);

        refreshView();
    }
    else
    {
        con_err("internal error");
    }
}

void MenuDialog::onDelete(iWidget* source)
{
    if (_userControlGraphView != nullptr)
    {
        _graphSelectionChanged(iNode::INVALID_NODE_ID);

        iNode* node = iNodeFactory::getInstance().getNode(_userControlGraphView->getSelectedNode());
        if (node != nullptr)
        {
            iNode* parent = node->getParent();
            if (parent != nullptr)
            {
                parent->removeNode(node);
                iNodeFactory::getInstance().destroyNode(node);
                refreshView();
            }
            else
            {
                _messageBox->show("can't delete root node");
            }
        }
    }
}

void MenuDialog::onCopy(iWidget* source)
{
    if (_userControlGraphView != nullptr)
    {
        _cutNodeID = 0;

        iNode* node = iNodeFactory::getInstance().getNode(_userControlGraphView->getSelectedNode());
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

void MenuDialog::onPaste(iWidget* source)
{
    if (_userControlGraphView != nullptr)
    {
        if (_copiedNodeID != 0)
        {
            iNode* pasteNode = nullptr;
            iNode* copiedNode = iNodeFactory::getInstance().getNode(_copiedNodeID);
            if (copiedNode != nullptr)
            {
                pasteNode = iNodeFactory::getInstance().createCopy(copiedNode);
            }

            if (pasteNode != nullptr)
            {
                iNode* destination = iNodeFactory::getInstance().getNode(_userControlGraphView->getSelectedNode());

                if (destination != nullptr)
                {
                    destination->insertNode(pasteNode);
                    refreshView();
                }
            }
        }
        else if (_cutNodeID != 0)
        {
            iNode* cutNode = iNodeFactory::getInstance().getNode(_cutNodeID);
            if (cutNode != nullptr)
            {
                iNode* destination = iNodeFactory::getInstance().getNode(_userControlGraphView->getSelectedNode());

                if (destination != nullptr)
                {
                    iNode* parent = cutNode->getParent();
                    if (parent != nullptr)
                    {
                        parent->removeNode(cutNode);

                        destination->insertNode(cutNode);
                        refreshView();
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

void MenuDialog::onCut(iWidget* source)
{
    _copiedNodeID = 0;

    if (_userControlGraphView != nullptr)
    {
        iNode* node = iNodeFactory::getInstance().getNode(_userControlGraphView->getSelectedNode());
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

void MenuDialog::onGraphSelectionChanged(uint32 nodeID)
{
    _graphSelectionChanged(nodeID);
}

void MenuDialog::onMaterialSelectionChanged(uint32 materialID)
{
	_materialSelectionChanged(materialID);
}

void MenuDialog::onAddMaterial()
{
	_addMaterial();
}

void MenuDialog::deinitGUI()
{
    _checkBoxGraph->unregisterOnClickEvent(iClickDelegate(this, &MenuDialog::onGraphViewSelected));
    _checkBoxMaterial->unregisterOnClickEvent(iClickDelegate(this, &MenuDialog::onMaterialViewSelected));
    _loadButton->unregisterOnClickEvent(iClickDelegate(this, &MenuDialog::onLoadFile));
    _saveButton->unregisterOnClickEvent(iClickDelegate(this, &MenuDialog::onSaveFile));
    _exitButton->unregisterOnClickEvent(iClickDelegate(this, &MenuDialog::onExitModelViewer));

    _cutButton->unregisterOnClickEvent(iClickDelegate(this, &MenuDialog::onCut));
    _copyButton->unregisterOnClickEvent(iClickDelegate(this, &MenuDialog::onCopy));
    _pasteButton->unregisterOnClickEvent(iClickDelegate(this, &MenuDialog::onPaste));
    _deleteButton->unregisterOnClickEvent(iClickDelegate(this, &MenuDialog::onDelete));

    deinitGraphView();
    deinitMaterialView();

    getDialog()->removeWidget(_grid);

    for (auto widget : _allwidgets)
    {
        iWidgetManager::getInstance().destroyWidget(widget);
    }

    if (_messageBox != nullptr)
    {
        delete _messageBox;
        _messageBox = nullptr;
    }
}

void MenuDialog::setRootNode(iNode* root)
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

void MenuDialog::refreshView()
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

void MenuDialog::onLoadFile(iWidget* source)
{
    _loadFile();
}

void MenuDialog::onSaveFile(iWidget* source)
{
    _saveFile();
}

void MenuDialog::onExitModelViewer(iWidget* source)
{
    _exitModelViewer();
}

void MenuDialog::registerOnCopyNode(CopyNodeDelegate copyNodeDelegate)
{
    _copyNode.append(copyNodeDelegate);
}

void MenuDialog::unregisterOnCopyNode(CopyNodeDelegate copyNodeDelegate)
{
    _copyNode.remove(copyNodeDelegate);
}

void MenuDialog::registerOnPasteNode(PasteNodeDelegate pasteNodeDelegate)
{
    _pasteNode.append(pasteNodeDelegate);
}

void MenuDialog::unregisterOnPasteNode(PasteNodeDelegate pasteNodeDelegate)
{
    _pasteNode.remove(pasteNodeDelegate);
}

void MenuDialog::registerOnGraphSelectionChanged(GraphSelectionChangedDelegate graphSelectionChangedDelegate)
{
    _graphSelectionChanged.append(graphSelectionChangedDelegate);
}

void MenuDialog::unregisterOnGraphSelectionChanged(GraphSelectionChangedDelegate graphSelectionChangedDelegate)
{
    _graphSelectionChanged.remove(graphSelectionChangedDelegate);
}

void MenuDialog::registerOnCutNode(CutNodeDelegate cutNodeDelegate)
{
    _cutNode.append(cutNodeDelegate);
}

void MenuDialog::unregisterOnCutNode(CutNodeDelegate cutNodeDelegate)
{
    _cutNode.remove(cutNodeDelegate);
}

void MenuDialog::registerOnAddTransformation(AddTransformationDelegate addTransformationDelegate)
{
    _addTransformation.append(addTransformationDelegate);
}

void MenuDialog::unregisterOnAddTransformation(AddTransformationDelegate addTransformationDelegate)
{
    _addTransformation.remove(addTransformationDelegate);
}

void MenuDialog::registerOnAddGroup(AddGroupDelegate addGroupDelegate)
{
    _addGroup.append(addGroupDelegate);
}

void MenuDialog::unregisterOnAddGroup(AddGroupDelegate addGroupDelegate)
{
    _addGroup.remove(addGroupDelegate);
}

void MenuDialog::registerOnAddEmitter(AddEmitterDelegate addEmitterDelegate)
{
    _addEmitter.append(addEmitterDelegate);
}

void MenuDialog::unregisterOnAddEmitter(AddEmitterDelegate addEmitterDelegate)
{
    _addEmitter.remove(addEmitterDelegate);
}

void MenuDialog::registerOnAddParticleSystem(AddParticleSystemDelegate addParticleSystemDelegate)
{
    _addParticleSystem.append(addParticleSystemDelegate);
}

void MenuDialog::unregisterOnAddParticleSystem(AddParticleSystemDelegate addParticleSystemDelegate)
{
    _addParticleSystem.remove(addParticleSystemDelegate);
}

void MenuDialog::registerOnAddSwitch(AddSwitchDelegate addSwitchDelegate)
{
    _addSwitch.append(addSwitchDelegate);
}

void MenuDialog::unregisterOnAddSwitch(AddSwitchDelegate addSwitchDelegate)
{
    _addSwitch.remove(addSwitchDelegate);
}

void MenuDialog::registerOnImportFile(ImportFileDelegate importFileDelegate)
{
    _importFile.append(importFileDelegate);
}

void MenuDialog::unregisterOnImportFile(ImportFileDelegate importFileDelegate)
{
    _importFile.remove(importFileDelegate);
}

void MenuDialog::registerOnImportFileReference(ImportFileReferenceDelegate importFileReferenceDelegate)
{
    _importFileReference.append(importFileReferenceDelegate);
}

void MenuDialog::unregisterOnImportFileReference(ImportFileReferenceDelegate importFileReferenceDelegate)
{
    _importFileReference.remove(importFileReferenceDelegate);
}

void MenuDialog::registerOnLoadFile(LoadFileDelegate loadFileDelegate)
{
    _loadFile.append(loadFileDelegate);
}

void MenuDialog::unregisterOnLoadFile(LoadFileDelegate loadFileDelegate)
{
    _loadFile.remove(loadFileDelegate);
}

void MenuDialog::registerOnSaveFile(SaveFileDelegate saveFileDelegate)
{
    _saveFile.append(saveFileDelegate);
}

void MenuDialog::unregisterOnSaveFile(SaveFileDelegate saveFileDelegate)
{
    _saveFile.remove(saveFileDelegate);
}

void MenuDialog::registerOnExitModelViewer(ExitModelViewerDelegate exitModelViewerDelegate)
{
    _exitModelViewer.append(exitModelViewerDelegate);
}

void MenuDialog::unregisterOnExitModelViewer(ExitModelViewerDelegate exitModelViewerDelegate)
{
    _exitModelViewer.remove(exitModelViewerDelegate);
}

void MenuDialog::onAddModel(uint32 addAt)
{
	if (_decisionBoxModelRef == nullptr)
	{
		_decisionBoxModelRef = new iDecisionBox();
	}

	_decisionBoxModelRef->show("Import model ...", iDecisionBoxCloseDelegate(this, &MenuDialog::onAddModelDecision), { "embedded", "as reference" }, 0);
}

void MenuDialog::onAddModelDecision(bool ok, int32 selection)
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

void MenuDialog::onAddTransformation(uint32 addAt)
{
	_addTransformation(addAt);
}

void MenuDialog::onAddGroup(uint32 addAt)
{
	_addGroup(addAt);
}

void MenuDialog::onAddEmitter(uint32 addAt)
{
	_addEmitter(addAt);
}

void MenuDialog::onAddParticleSystem(uint32 addAt)
{
	_addParticleSystem(addAt);
}

void MenuDialog::onAddSwitch(uint32 addAt)
{
	_addSwitch(addAt);
}

void MenuDialog::registerOnMaterialSelectionChanged(MaterialSelectionChangedDelegate materialSelectionChangedDelegate)
{
	_materialSelectionChanged.append(materialSelectionChangedDelegate);
}

void MenuDialog::unregisterOnMaterialSelectionChanged(MaterialSelectionChangedDelegate materialSelectionChangedDelegate)
{
	_materialSelectionChanged.remove(materialSelectionChangedDelegate);
}

void MenuDialog::registerOnAddMaterial(AddMaterialDelegate addMaterialDelegate)
{
	_addMaterial.append(addMaterialDelegate);
}

void MenuDialog::unregisterOnAddMaterial(AddMaterialDelegate addMaterialDelegate)
{
	_addMaterial.remove(addMaterialDelegate);
}