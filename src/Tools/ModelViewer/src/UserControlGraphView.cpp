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

    updateGraph();
}

void UserControlGraphView::updateGraph()
{
    if (_root != iNode::INVALID_NODE_ID)
    {
        traverseTree(iNodeFactory::getInstance().getNode(_root));
    }
    else
    {
        clear();
    }
}

uint32 UserControlGraphView::getRootNode()
{
    return _root;
}

iWidget* UserControlGraphView::getWidget()
{
    return _grid;
}

void UserControlGraphView::initGUI()
{
	_grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
	_allwidgets.push_back(_grid);
	_grid->setBorder(0);
	_grid->appendRows(1);
	_grid->setCellSpacing(2);
	_grid->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
	_grid->setVerticalAlignment(iVerticalAlignment::Strech);

	_gridButtons = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
	_allwidgets.push_back(_gridButtons);
	_gridButtons->setBorder(0);
	_gridButtons->appendCollumns(10);
	_gridButtons->setCellSpacing(2);
	_gridButtons->setHorrizontalAlignment(iHorrizontalAlignment::Left);
	_gridButtons->setVerticalAlignment(iVerticalAlignment::Top);

	_addTransformationButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
	_allwidgets.push_back(_addTransformationButton);
	_addTransformationButton->setText("");
	_addTransformationButton->setWidth(30);
	_addTransformationButton->setHeight(30);
	_addTransformationButton->setTexture("icons\\addTransformation.png");
	_addTransformationButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddTransformation));

	_addModelButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
	_allwidgets.push_back(_addModelButton);
	_addModelButton->setText("");
	_addModelButton->setWidth(30);
	_addModelButton->setHeight(30);
	_addModelButton->setTexture("icons\\addModel.png");
	_addModelButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddModel));

	_addGroupButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
	_allwidgets.push_back(_addGroupButton);
	_addGroupButton->setText("");
	_addGroupButton->setWidth(30);
	_addGroupButton->setHeight(30);
	_addGroupButton->setTexture("icons\\addGroup.png");
	_addGroupButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddGroup));

	_addEmitterButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
	_allwidgets.push_back(_addEmitterButton);
	_addEmitterButton->setText("");
	_addEmitterButton->setWidth(30);
	_addEmitterButton->setHeight(30);
	_addEmitterButton->setTexture("icons\\addEmitter.png");
	_addEmitterButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddEmitter));

	_addParticleSystemButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
	_allwidgets.push_back(_addParticleSystemButton);
	_addParticleSystemButton->setText("");
	_addParticleSystemButton->setWidth(30);
	_addParticleSystemButton->setHeight(30);
	_addParticleSystemButton->setTexture("icons\\addParticleSystem.png");
	_addParticleSystemButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddParticleSystem));

	_addSwitchButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
	_allwidgets.push_back(_addSwitchButton);
	_addSwitchButton->setText("");
	_addSwitchButton->setWidth(30);
	_addSwitchButton->setHeight(30);
	_addSwitchButton->setTexture("icons\\addSwitch.png");
	_addSwitchButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddSwitch));

    _groupBox = static_cast<iWidgetGroupBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::GroupBox));
    _groupBox->setText("Graph");
	_groupBox->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
	_groupBox->setVerticalAlignment(iVerticalAlignment::Strech);

    _scroll = static_cast<iWidgetScroll*>(iWidgetManager::getInstance().createWidget(iWidgetType::Scroll));
    _scroll->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    _scroll->setVerticalAlignment(iVerticalAlignment::Strech);

    _gridGraph = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _gridGraph->setBorder(0);
    _gridGraph->setSelectMode(iSelectionMode::Row);
    _gridGraph->setCellSpacing(0);
    _gridGraph->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _gridGraph->setVerticalAlignment(iVerticalAlignment::Top);
    _gridGraph->registerOnChangeEvent(iChangeDelegate(this, &UserControlGraphView::OnSelectionChange));   

	_gridButtons->addWidget(_addTransformationButton, 0, 0);
	_gridButtons->addWidget(_addGroupButton, 1, 0);
	_gridButtons->addWidget(_addSwitchButton, 2, 0);
	_gridButtons->addWidget(_addModelButton, 3, 0);
	_gridButtons->addWidget(_addEmitterButton, 4, 0);
	_gridButtons->addWidget(_addParticleSystemButton, 5, 0);

	_grid->addWidget(_gridButtons, 0, 0);
	_grid->addWidget(_groupBox, 0, 1);
    _groupBox->addWidget(_scroll);
    _scroll->addWidget(_gridGraph);
}

void UserControlGraphView::deinitGUI()
{
    _gridGraph->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlGraphView::OnSelectionChange));
    clear();
    iWidgetManager::getInstance().destroyWidget(_gridGraph);
    iWidgetManager::getInstance().destroyWidget(_scroll);
    iWidgetManager::getInstance().destroyWidget(_groupBox);
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
    uint32 nodeID = 0;
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

void UserControlGraphView::clear()
{
    _gridGraph->clear();

    for (auto iter : _userData)
    {
        delete [] iter;
    }

    _userData.clear();

    auto iterWidget = _gridEntryWidgets.begin();
    while (iterWidget != _gridEntryWidgets.end())
    {
        iWidgetManager::getInstance().destroyWidget((*iterWidget));
        iterWidget++;
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
    clear();
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