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
    return _groupBox;
}

void UserControlGraphView::initGUI()
{
    _groupBox = static_cast<iWidgetGroupBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::GroupBox));
    _groupBox->setText("Graph");
    _groupBox->setWidth(10);
    _groupBox->setHeight(10);

    _scroll = static_cast<iWidgetScroll*>(iWidgetManager::getInstance().createWidget(iWidgetType::Scroll));
    _scroll->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _scroll->setVerticalAlignment(iVerticalAlignment::Top);
    _scroll->setWidth(360);
    _scroll->setHeight(350);

    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _grid->setBorder(0);
    _grid->setSelectMode(iSelectionMode::Row);
    _grid->setCellSpacing(0);
    _grid->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);
    _grid->registerOnChangeEvent(iChangeDelegate(this, &UserControlGraphView::OnSelectionChange));   

    _groupBox->addWidget(_scroll);
    _scroll->addWidget(_grid);
}

void UserControlGraphView::deinitGUI()
{
    _grid->unregisterOnChangeEvent(iChangeDelegate(this, &UserControlGraphView::OnSelectionChange));
    clear();
    iWidgetManager::getInstance().destroyWidget(_grid);
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
    uint32* nodeIDPtr = static_cast<uint32*>(_grid->getSelectedUserData());
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

void UserControlGraphView::registerOnSelectionChange(SelectionChangeDelegate selectionChangeDelegate)
{
    _selectionChange.append(selectionChangeDelegate);
}

void UserControlGraphView::unregisterOnSelectionChange(SelectionChangeDelegate selectionChangeDelegate)
{
    _selectionChange.remove(selectionChangeDelegate);
}

void UserControlGraphView::clear()
{
    _grid->clear();

    for (auto iter : _userData)
    {
        delete [] iter;
    }

    _userData.clear();

    auto iterWidget = _widgets.begin();
    while (iterWidget != _widgets.end())
    {
        iWidgetManager::getInstance().destroyWidget((*iterWidget));
        iterWidget++;
    }
    _widgets.clear();
    
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

        uint32 currentRowIndex = _grid->getRowCount() - 1;
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
        _grid->addWidget(entry, 0, currentRowIndex, userData);

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

        _widgets.push_back(indentLabel);
        _widgets.push_back(label);
        _widgets.push_back(icon);
        _widgets.push_back(entry);

        _grid->appendRows(1);
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
