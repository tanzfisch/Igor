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
    
    _gridButtons1 = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid)); 
    _allwidgets.push_back(_gridButtons1);
    _gridButtons1->setBorder(0);
    _gridButtons1->setCellSpacing(2);
    _gridButtons1->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _gridButtons1->setVerticalAlignment(iVerticalAlignment::Top);
    _gridButtons1->appendCollumns(9);

    _gridButtons2 = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allwidgets.push_back(_gridButtons2);
    _gridButtons2->setBorder(0);
    _gridButtons2->setCellSpacing(2);
    _gridButtons2->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _gridButtons2->setVerticalAlignment(iVerticalAlignment::Top);
    _gridButtons2->appendCollumns(9);
    
    _gridButtons = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allwidgets.push_back(_gridButtons);
    _gridButtons->setBorder(0);
    _gridButtons->setCellSpacing(2);
    _gridButtons->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _gridButtons->setVerticalAlignment(iVerticalAlignment::Top);
    _gridButtons->appendRows(1);

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

    _addTransformationButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allwidgets.push_back(_addTransformationButton);
    _addTransformationButton->setText("");
    _addTransformationButton->setWidth(30);
    _addTransformationButton->setHeight(30);
    _addTransformationButton->setTexture("icons\\addTransformation.png");
    _addTransformationButton->registerOnClickEvent(iClickDelegate(this, &MenuDialog::onAddTransformation));

    _addModelButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allwidgets.push_back(_addModelButton);
    _addModelButton->setText("");
    _addModelButton->setWidth(30);
    _addModelButton->setHeight(30);
    _addModelButton->setTexture("icons\\addModel.png");
    _addModelButton->registerOnClickEvent(iClickDelegate(this, &MenuDialog::onAddModel));

    _addGroupButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allwidgets.push_back(_addGroupButton);
    _addGroupButton->setText("");
    _addGroupButton->setWidth(30);
    _addGroupButton->setHeight(30);
    _addGroupButton->setTexture("icons\\addGroup.png");
    _addGroupButton->registerOnClickEvent(iClickDelegate(this, &MenuDialog::onAddGroup));

    _addEmitterButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allwidgets.push_back(_addEmitterButton);
    _addEmitterButton->setText("");
    _addEmitterButton->setWidth(30);
    _addEmitterButton->setHeight(30);
    _addEmitterButton->setTexture("icons\\addEmitter.png");
    _addEmitterButton->registerOnClickEvent(iClickDelegate(this, &MenuDialog::onAddEmitter));

    _addParticleSystemButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allwidgets.push_back(_addParticleSystemButton);
    _addParticleSystemButton->setText("");
    _addParticleSystemButton->setWidth(30);
    _addParticleSystemButton->setHeight(30);
    _addParticleSystemButton->setTexture("icons\\addParticleSystem.png");
    _addParticleSystemButton->registerOnClickEvent(iClickDelegate(this, &MenuDialog::onAddParticleSystem));

    _addSwitchButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
    _allwidgets.push_back(_addSwitchButton);
    _addSwitchButton->setText("");
    _addSwitchButton->setWidth(30);
    _addSwitchButton->setHeight(30);
    _addSwitchButton->setTexture("icons\\addSwitch.png");
    _addSwitchButton->registerOnClickEvent(iClickDelegate(this, &MenuDialog::onAddSwitch));

    _userControlGraphView = new UserControlGraphView();
    _userControlGraphView->registerOnSelectionChange(SelectionChangeDelegate(this, &MenuDialog::onGraphViewSelectionChanged));

    getDialog()->addWidget(_grid);
    _grid->addWidget(_gridButtons, 0, 0);
    _gridButtons->addWidget(_gridButtons1, 0, 0);
    _gridButtons->addWidget(_gridButtons2, 0, 1);
    _gridButtons1->addWidget(_loadButton, 0, 0);
    _gridButtons1->addWidget(_saveButton, 1, 0);
    _gridButtons1->addWidget(_exitButton, 2, 0);
    _gridButtons1->addWidget(_spacer1, 3, 0);
    _gridButtons1->addWidget(_pasteButton, 4, 0);
    _gridButtons1->addWidget(_cutButton, 5, 0);
    _gridButtons1->addWidget(_copyButton, 6, 0);
    _gridButtons1->addWidget(_spacer2, 7, 0);
    _gridButtons1->addWidget(_deleteButton, 8, 0);

    _gridButtons2->addWidget(_addTransformationButton, 0, 0);
    _gridButtons2->addWidget(_addGroupButton, 1, 0);
    _gridButtons2->addWidget(_addSwitchButton, 2, 0);
    _gridButtons2->addWidget(_addModelButton, 3, 0);
    _gridButtons2->addWidget(_addEmitterButton, 4, 0);
    _gridButtons2->addWidget(_addParticleSystemButton, 5, 0);
    
    _grid->addWidget(_userControlGraphView->getWidget(), 0, 1);
}

void MenuDialog::onStructureChanged()
{
    _userControlGraphView->updateGraph();
}

void MenuDialog::onDelete(iWidget* source)
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
            updateGraph();
        }
        else
        {
            _messageBox->show("can't delete root node");
        }
    }
}

void MenuDialog::onAddModel(iWidget* source)
{
    if (_decisionBoxModelRef == nullptr)
    {
        _decisionBoxModelRef = new iDecisionBox();
    }

    _decisionBoxModelRef->show("Import model ...", iDecisionBoxCloseDelegate(this, &MenuDialog::onAddModelDecision), { "embedded", "as reference" }, 0);
}

void MenuDialog::onAddModelDecision(bool ok, int32 selection)
{
    if (ok)
    {
        switch (selection)
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
}

void MenuDialog::onAddTransformation(iWidget* source)
{
    _addTransformation(_userControlGraphView->getSelectedNode());
}

void MenuDialog::onAddGroup(iWidget* source)
{
    _addGroup(_userControlGraphView->getSelectedNode());
}

void MenuDialog::onAddEmitter(iWidget* source)
{
    _addEmitter(_userControlGraphView->getSelectedNode());
}

void MenuDialog::onAddParticleSystem(iWidget* source)
{
    _addParticleSystem(_userControlGraphView->getSelectedNode());
}

void MenuDialog::onAddSwitch(iWidget* source)
{
    _addSwitch(_userControlGraphView->getSelectedNode());
}

void MenuDialog::onCopy(iWidget* source)
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

void MenuDialog::onPaste(iWidget* source)
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
                updateGraph();
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
                    updateGraph();
                }
            }
        }
    }
    else
    {
        _messageBox->show("clipboard is empty");
    }
}

void MenuDialog::onCut(iWidget* source)
{
    _copiedNodeID = 0;

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

void MenuDialog::onGraphViewSelectionChanged(uint32 nodeID)
{
    _graphSelectionChanged(nodeID);
}

void MenuDialog::deinitGUI()
{
    getDialog()->removeWidget(_grid);

    _loadButton->unregisterOnClickEvent(iClickDelegate(this, &MenuDialog::onLoadFile));
    _saveButton->unregisterOnClickEvent(iClickDelegate(this, &MenuDialog::onSaveFile));
    _exitButton->unregisterOnClickEvent(iClickDelegate(this, &MenuDialog::onExitModelViewer));

    for (auto widget : _allwidgets)
    {
        iWidgetManager::getInstance().destroyWidget(widget);
    }

    if (_userControlGraphView)
    {
        _userControlGraphView->unregisterOnSelectionChange(SelectionChangeDelegate(this, &MenuDialog::onGraphViewSelectionChanged));
        delete _userControlGraphView;
        _userControlGraphView = nullptr;
    }

    if (_messageBox != nullptr)
    {
        delete _messageBox;
        _messageBox = nullptr;
    }
}

void MenuDialog::setRootNode(iNode* root)
{
    _userControlGraphView->setRootNode(root->getID());
}

void MenuDialog::updateGraph()
{
    _userControlGraphView->updateGraph();
    // todo clear properties
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