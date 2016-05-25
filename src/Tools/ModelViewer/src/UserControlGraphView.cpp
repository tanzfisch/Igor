// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlGraphView.h"

#include <iNode.h>
#include <iWidgetGrid.h>
#include <iWidgetDialog.h>
#include <iWidgetManager.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetPicture.h>
#include <iNodeFactory.h>
#include <iWidgetScroll.h>
#include <iWidgetDialog.h>
#include <iWidgetGroupBox.h>
#include <iDecisionBox.h>
using namespace Igor;

#include <IgorAux.h>
#include <iaConsole.h>
using namespace IgorAux;

UserControlGraphView::UserControlGraphView()
{
    initGUI();
}

UserControlGraphView::~UserControlGraphView()
{
    deinitGUI();
}

void UserControlGraphView::setRootNode(uint32 root)
{
    _root = root;

    refresh();
}

void UserControlGraphView::refresh()
{
    if (_root != iNode::INVALID_NODE_ID)
    {
        traverseTree(iNodeFactory::getInstance().getNode(_root));
    }
    else
    {
        clearGraph();
    }
}

uint32 UserControlGraphView::getRootNode()
{
    return _root;
}

iWidget* UserControlGraphView::getWidget()
{
    return _rootWidget;
}

void UserControlGraphView::initGUI()
{
    iWidgetGrid* grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _rootWidget = grid;
	_allWidgets.push_back(grid);
	grid->setBorder(0);
	grid->appendRows(1);
	grid->setCellSpacing(2);
	grid->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
	grid->setVerticalAlignment(iVerticalAlignment::Strech);

    iWidgetGrid* gridButtons = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
	_allWidgets.push_back(gridButtons);
	gridButtons->setBorder(0);
	gridButtons->appendCollumns(10);
	gridButtons->setCellSpacing(2);
	gridButtons->setHorrizontalAlignment(iHorrizontalAlignment::Left);
	gridButtons->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetButton* addTransformationButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
	_allWidgets.push_back(addTransformationButton);
    addTransformationButton->setText("");
    addTransformationButton->setWidth(30);
    addTransformationButton->setHeight(30);
    addTransformationButton->setTexture("icons\\addTransformation.png");
    addTransformationButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddTransformation));

    iWidgetButton* addModelButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
	_allWidgets.push_back(addModelButton);
    addModelButton->setText("");
    addModelButton->setWidth(30);
    addModelButton->setHeight(30);
    addModelButton->setTexture("icons\\addModel.png");
    addModelButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddModel));

    iWidgetButton* addGroupButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
	_allWidgets.push_back(addGroupButton);
    addGroupButton->setText("");
    addGroupButton->setWidth(30);
    addGroupButton->setHeight(30);
    addGroupButton->setTexture("icons\\addGroup.png");
    addGroupButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddGroup));

    iWidgetButton* addEmitterButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
	_allWidgets.push_back(addEmitterButton);
    addEmitterButton->setText("");
    addEmitterButton->setWidth(30);
    addEmitterButton->setHeight(30);
    addEmitterButton->setTexture("icons\\addEmitter.png");
    addEmitterButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddEmitter));

    iWidgetButton* addParticleSystemButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
	_allWidgets.push_back(addParticleSystemButton);
    addParticleSystemButton->setText("");
    addParticleSystemButton->setWidth(30);
    addParticleSystemButton->setHeight(30);
    addParticleSystemButton->setTexture("icons\\addParticleSystem.png");
    addParticleSystemButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddParticleSystem));

    iWidgetButton* addSwitchButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
	_allWidgets.push_back(addSwitchButton);
    addSwitchButton->setText("");
    addSwitchButton->setWidth(30);
    addSwitchButton->setHeight(30);
    addSwitchButton->setTexture("icons\\addSwitch.png");
    addSwitchButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddSwitch));

    iWidgetGroupBox* groupBox = static_cast<iWidgetGroupBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::GroupBox));
    _allWidgets.push_back(groupBox);
    groupBox->setText("Graph");
    groupBox->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    groupBox->setVerticalAlignment(iVerticalAlignment::Strech);

    iWidgetScroll* scroll = static_cast<iWidgetScroll*>(iWidgetManager::getInstance().createWidget(iWidgetType::Scroll));
    _allWidgets.push_back(scroll);
    scroll->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    scroll->setVerticalAlignment(iVerticalAlignment::Strech);

    _gridGraph = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allWidgets.push_back(_gridGraph);
    _gridGraph->setBorder(0);
    _gridGraph->setSelectMode(iSelectionMode::Row);
    _gridGraph->setCellSpacing(0);
    _gridGraph->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _gridGraph->setVerticalAlignment(iVerticalAlignment::Top);
    _gridGraph->registerOnChangeEvent(iChangeDelegate(this, &UserControlGraphView::OnSelectionChange));

	gridButtons->addWidget(addTransformationButton, 0, 0);
	gridButtons->addWidget(addGroupButton, 1, 0);
	gridButtons->addWidget(addSwitchButton, 2, 0);
	gridButtons->addWidget(addModelButton, 3, 0);
	gridButtons->addWidget(addEmitterButton, 4, 0);
	gridButtons->addWidget(addParticleSystemButton, 5, 0);

	grid->addWidget(gridButtons, 0, 0);
	grid->addWidget(groupBox, 0, 1);
    groupBox->addWidget(scroll);
    scroll->addWidget(_gridGraph);
}

void UserControlGraphView::deinitGUI()
{
    _gridGraph->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlGraphView::OnSelectionChange));

    clearGraph();

    for (auto widget : _allWidgets)
    {
        iWidgetManager::getInstance().destroyWidget(widget);
    }

    _allWidgets.clear();

    _rootWidget = nullptr;
    _gridGraph = nullptr;
}

iaString UserControlGraphView::getIconTexture(iNodeType type)
{
    switch (type)
    {
    case iNodeType::iNodeCamera:
        return "icons\\camera.png";
    case iNodeType::iNodeLight:
        return "icons\\light.png";
    case iNodeType::iNodeMesh:
        return "icons\\mesh.png";
    case iNodeType::iNodeModel:
        return "icons\\model.png";
    case iNodeType::iNodeSkyBox:
        return "icons\\skybox.png";
    case iNodeType::iNode:
        return "icons\\group.png";
    case iNodeType::iNodeSwitch:
        return "icons\\switch.png";
    case iNodeType::iNodeTransform:
        return "icons\\transformation.png";
    case iNodeType::iNodeEmitter:
        return "icons\\emitter.png";
    case iNodeType::iNodeParticleSystem:
        return "icons\\ParticleSystem.png";

    case iNodeType::iSkyLightNode:
    case iNodeType::iCelestialNode:
    case iNodeType::iNodeRender:
    case iNodeType::Undefined:
    default:
        con_assert(false, "unexpected node type");
    }

    return "";
}

void UserControlGraphView::OnSelectionChange(iWidget* widget)
{
    uint32* nodeIDPtr = static_cast<uint32*>(_gridGraph->getSelectedUserData());
    uint32 nodeID = iNode::INVALID_NODE_ID;
    if (nodeIDPtr != nullptr)
    {
        nodeID = *nodeIDPtr;
    }

    _selectedNode = nodeID;
    _selectionChange(_selectedNode);
}

uint32 UserControlGraphView::getSelectedNode()
{
    return _selectedNode;
}

void UserControlGraphView::registerOnSelectionChange(GraphSelectionChangedDelegate selectionChangeDelegate)
{
    _selectionChange.append(selectionChangeDelegate);
}

void UserControlGraphView::unregisterOnSelectionChange(GraphSelectionChangedDelegate selectionChangeDelegate)
{
    _selectionChange.remove(selectionChangeDelegate);
}

void UserControlGraphView::clearGraph()
{
    _gridGraph->clear();

    for (auto iter : _userData)
    {
        delete [] iter;
    }

    _userData.clear();

    for(auto widget : _gridEntryWidgets)
    {
        iWidgetManager::getInstance().destroyWidget(widget);
    }

    _gridEntryWidgets.clear();
    
    _selectedNode = iNode::INVALID_NODE_ID;
}

bool UserControlGraphView::preOrderVisit(iNode* node)
{
    if (!_firstNode)
    {
        _indentation++;

        iaString indentString;
        for (int i = 0; i < _indentation; ++i)
        {
            indentString += ' ';
        }

        uint32 currentRowIndex = _gridGraph->getRowCount() - 1;
        uint32 currentCollumnIndex = _indentation;

        iWidgetGrid* entry = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
        entry->setSelectMode(iSelectionMode::NoSelection);
        entry->setBorder(0);
        entry->setCellSpacing(2);
        entry->appendCollumns(2);
        entry->setHorrizontalAlignment(iHorrizontalAlignment::Left);
        uint32* userData = new uint32();
        _userData.push_back(userData);
        *userData = node->getID();
        _gridGraph->addWidget(entry, 0, currentRowIndex, userData);

        iWidgetLabel* indentLabel = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
        indentLabel->setHorrizontalAlignment(iHorrizontalAlignment::Left);
        indentLabel->setVerticalAlignment(iVerticalAlignment::Center);
        indentLabel->setText(indentString);

        iWidgetLabel* label = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
        label->setHorrizontalAlignment(iHorrizontalAlignment::Right);
        label->setVerticalAlignment(iVerticalAlignment::Center);

        iWidgetPicture* icon = static_cast<iWidgetPicture*>(iWidgetManager::getInstance().createWidget(iWidgetType::Picture));
        icon->setMaxSize(24, 24);

        if (node->getParent() != nullptr)
        {
            icon->setTexture(getIconTexture(node->getType()));
            label->setText(node->getName());
        }
        else
        {
            icon->setTexture("icons\\root.png");
            label->setText(node->getName());
        }

        entry->addWidget(indentLabel, 0, 0);
        entry->addWidget(icon, 1, 0);
        entry->addWidget(label, 2, 0);

        _gridEntryWidgets.push_back(indentLabel);
        _gridEntryWidgets.push_back(label);
        _gridEntryWidgets.push_back(icon);
        _gridEntryWidgets.push_back(entry);

        _gridGraph->appendRows(1);
    }

    _firstNode = false;

    return true;
}

void UserControlGraphView::postOrderVisit(iNode* node)
{
    _indentation--;
}

void UserControlGraphView::preTraverse()
{
    clearGraph();
    _firstNode = true;
    _indentation = 0;
}

void UserControlGraphView::postTraverse()
{
}

void UserControlGraphView::registerOnAddTransformation(AddTransformationDelegate addTransformationDelegate)
{
	_addTransformation.append(addTransformationDelegate);
}

void UserControlGraphView::unregisterOnAddTransformation(AddTransformationDelegate addTransformationDelegate)
{
	_addTransformation.remove(addTransformationDelegate);
}

void UserControlGraphView::registerOnAddGroup(AddGroupDelegate addGroupDelegate)
{
	_addGroup.append(addGroupDelegate);
}

void UserControlGraphView::unregisterOnAddGroup(AddGroupDelegate addGroupDelegate)
{
	_addGroup.remove(addGroupDelegate);
}

void UserControlGraphView::registerOnAddEmitter(AddEmitterDelegate addEmitterDelegate)
{
	_addEmitter.append(addEmitterDelegate);
}

void UserControlGraphView::unregisterOnAddEmitter(AddEmitterDelegate addEmitterDelegate)
{
	_addEmitter.remove(addEmitterDelegate);
}

void UserControlGraphView::registerOnAddParticleSystem(AddParticleSystemDelegate addParticleSystemDelegate)
{
	_addParticleSystem.append(addParticleSystemDelegate);
}

void UserControlGraphView::unregisterOnAddParticleSystem(AddParticleSystemDelegate addParticleSystemDelegate)
{
	_addParticleSystem.remove(addParticleSystemDelegate);
}

void UserControlGraphView::registerOnAddSwitch(AddSwitchDelegate addSwitchDelegate)
{
	_addSwitch.append(addSwitchDelegate);
}

void UserControlGraphView::unregisterOnAddSwitch(AddSwitchDelegate addSwitchDelegate)
{
	_addSwitch.remove(addSwitchDelegate);
}

void UserControlGraphView::registerOnAddModel(AddModelDelegate addModelDelegate)
{
	_addModel.append(addModelDelegate);
}

void UserControlGraphView::unregisterOnAddModel(AddModelDelegate addModelDelegate)
{
	_addModel.remove(addModelDelegate);
}

void UserControlGraphView::onAddTransformation(iWidget* source)
{
	_addTransformation(_selectedNode);
}

void UserControlGraphView::onAddGroup(iWidget* source)
{
	_addGroup(_selectedNode);
}

void UserControlGraphView::onAddEmitter(iWidget* source)
{
	_addEmitter(_selectedNode);
}

void UserControlGraphView::onAddParticleSystem(iWidget* source)
{
	_addParticleSystem(_selectedNode);
}

void UserControlGraphView::onAddSwitch(iWidget* source)
{
	_addSwitch(_selectedNode);
}

void UserControlGraphView::onAddModel(iWidget* source)
{
	_addModel(_selectedNode);
}