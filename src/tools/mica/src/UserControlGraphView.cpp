// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlGraphView.h"

#include <igor/graphics/scene/nodes/iNode.h>
#include <igor/ui/widgets/iWidgetGrid.h>
#include <igor/ui/dialogs/iDialog.h>
#include <igor/ui/iWidgetManager.h>
#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/ui/widgets/iWidgetPicture.h>
#include <igor/graphics/scene/nodes/iNodeManager.h>
#include <igor/ui/widgets/iWidgetScroll.h>
#include <igor/ui/dialogs/iDialog.h>
#include <igor/ui/widgets/iWidgetGroupBox.h>
#include <igor/ui/dialogs/iDialogIndexMenu.h>
#include <igor/os/iMouse.h>
using namespace Igor;

#include <iaux/system/iaConsole.h>
using namespace IgorAux;

UserControlGraphView::UserControlGraphView()
{
    initGUI();
}

UserControlGraphView::~UserControlGraphView()
{
	if (_dialogMenu != nullptr)
	{
		delete _dialogMenu;
		_dialogMenu = nullptr;
	}
}

void UserControlGraphView::setRootNode(uint64 root)
{
    _root = root;
    refresh();
}

void UserControlGraphView::refresh()
{
    if (_root != iNode::INVALID_NODE_ID)
    {
        traverseTree(iNodeManager::getInstance().getNode(_root));
    }
    else
    {
        clearGraph();
    }
}

uint64 UserControlGraphView::getRootNode()
{
    return _root;
}

void UserControlGraphView::initGUI()
{
    iWidgetGrid* grid = new iWidgetGrid();
    grid->setBorder(0);
    grid->appendRows(1);
    grid->setCellSpacing(2);
    grid->setStrechRow(1);
    grid->setStrechColumn(0);
    grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
    grid->setVerticalAlignment(iVerticalAlignment::Strech);
	addWidget(grid);

    iWidgetGrid* gridButtons = new iWidgetGrid();
    gridButtons->setBorder(0);
    gridButtons->appendCollumns(10);
    gridButtons->setCellSpacing(2);
    gridButtons->setHorizontalAlignment(iHorizontalAlignment::Left);
    gridButtons->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetButton* addTransformationButton = new iWidgetButton();
    addTransformationButton->setText("");
    addTransformationButton->setWidth(30);
    addTransformationButton->setHeight(30);
	addTransformationButton->setTooltip("Add transformation node. You need to move something you need a transform node.");
    addTransformationButton->setTexture("icons\\addTransformation.png");
    addTransformationButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddTransformation));

    iWidgetButton* addModelButton = new iWidgetButton();
    addModelButton->setText("");
    addModelButton->setWidth(30);
    addModelButton->setHeight(30);
	addModelButton->setTooltip("Add model node. Adds a model embedded or referenced to the scene.");
    addModelButton->setTexture("icons\\addModel.png");
    addModelButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddModel));

    iWidgetButton* addGroupButton = new iWidgetButton();
    addGroupButton->setText("");
    addGroupButton->setWidth(30);
    addGroupButton->setHeight(30);
	addGroupButton->setTooltip("Add group node. Used to create node hierarchies in the scene.");
    addGroupButton->setTexture("icons\\addGroup.png");
    addGroupButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddGroup));

    iWidgetButton* addEmitterButton = new iWidgetButton();
    addEmitterButton->setText("");
    addEmitterButton->setWidth(30);
    addEmitterButton->setHeight(30);
	addEmitterButton->setTooltip("Add emitter node. To emit particles from.");
    addEmitterButton->setTexture("icons\\addEmitter.png");
    addEmitterButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddEmitter));

    iWidgetButton* addParticleSystemButton = new iWidgetButton();
    addParticleSystemButton->setText("");
    addParticleSystemButton->setWidth(30);
    addParticleSystemButton->setHeight(30);
	addParticleSystemButton->setTooltip("Add particle system node. Needs emitter node to function");
    addParticleSystemButton->setTexture("icons\\addParticleSystem.png");
    addParticleSystemButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddParticleSystem));

    iWidgetButton* addSwitchButton = new iWidgetButton();
    addSwitchButton->setText("");
    addSwitchButton->setWidth(30);
    addSwitchButton->setHeight(30);
	addSwitchButton->setTooltip("Add switch node. This node can switch the active state of it's child nodes. Only the active node will be visible.");
    addSwitchButton->setTexture("icons\\addSwitch.png");
    addSwitchButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddSwitch));

    iWidgetGroupBox* groupBox = new iWidgetGroupBox();
    groupBox->setText("Graph");
    groupBox->setHorizontalAlignment(iHorizontalAlignment::Strech);
    groupBox->setVerticalAlignment(iVerticalAlignment::Strech);

    iWidgetScroll* scroll = new iWidgetScroll();
    scroll->setVerticalAlignment(iVerticalAlignment::Strech);
    scroll->setHorizontalAlignment(iHorizontalAlignment::Strech);

    _gridGraph = new iWidgetGrid();
    _gridGraph->setBorder(0);
    _gridGraph->setWidth(300);
    _gridGraph->setSelectMode(iSelectionMode::Row);
    _gridGraph->setCellSpacing(0);
    _gridGraph->setHorizontalAlignment(iHorizontalAlignment::Left);
    _gridGraph->setVerticalAlignment(iVerticalAlignment::Top);
    _gridGraph->registerOnChangeEvent(iChangeDelegate(this, &UserControlGraphView::OnSelectionChange));
    _gridGraph->registerOnContextMenuEvent(iContextMenuDelegate(this, &UserControlGraphView::OnContextMenu));

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

    case iNodeType::iNodeRender:
    case iNodeType::Undefined:
    default:
        con_assert(false, "unexpected node type");
    }

    return "";
}

void UserControlGraphView::OnSelectionChange(iWidgetPtr widget)
{
    std::any userData = _gridGraph->getSelectedUserData();
    uint64 nodeID = iNode::INVALID_NODE_ID;
    if (userData.has_value())
    {
        nodeID = std::any_cast<uint64>(userData);
    }

    _selectedNode = nodeID;
    _selectionChange(_selectedNode);
}

void UserControlGraphView::OnContextMenu(iWidgetPtr widget)
{
    if (_dialogMenu == nullptr)
    {
        _dialogMenu = new iDialogIndexMenu();

        _dialogMenu->setWidth(24);

        iaVector2i pos = iMouse::getInstance().getPos();
        _dialogMenu->setX(pos._x);
        _dialogMenu->setY(pos._y);

        std::vector<iaString> dialogMenuTexts;
        std::vector<iaString> dialogMenuPictures;

        dialogMenuTexts.push_back("Cut");
        dialogMenuPictures.push_back("icons\\cut.png");

        dialogMenuTexts.push_back("Copy");
        dialogMenuPictures.push_back("icons\\copy.png");

        dialogMenuTexts.push_back("Paste");
        dialogMenuPictures.push_back("icons\\paste.png");

        dialogMenuTexts.push_back("Delete");
        dialogMenuPictures.push_back("icons\\delete.png");

        dialogMenuTexts.push_back("Add Transformation");
        dialogMenuPictures.push_back("icons\\Transformation.png");

        dialogMenuTexts.push_back("Add Group");
        dialogMenuPictures.push_back("icons\\Group.png");

        dialogMenuTexts.push_back("Add Switch");
        dialogMenuPictures.push_back("icons\\Switch.png");

        dialogMenuTexts.push_back("Add Model");
        dialogMenuPictures.push_back("icons\\Model.png");

        dialogMenuTexts.push_back("Add Emitter");
        dialogMenuPictures.push_back("icons\\Emitter.png");

        dialogMenuTexts.push_back("Add Particle System");
        dialogMenuPictures.push_back("icons\\ParticleSystem.png");

        _dialogMenu->open(iDialogCloseDelegate(this, &UserControlGraphView::OnContextMenuClose), dialogMenuTexts, dialogMenuPictures);
    }
}

void UserControlGraphView::OnContextMenuClose(iDialogPtr dialog)
{
    if (dialog != _dialogMenu)
    {
        return;
    }

    // TODO ??? yeah we need something like Qt QAction
    const int32 cutID = 0;
    const int32 copyID = 1;
    const int32 pasteID = 2;
    const int32 deleteID = 3;
    const int32 addTransformID = 4;
    const int32 addGroupID = 5;
    const int32 addSwitchID = 6;
    const int32 addModelID = 7;
    const int32 addEmitterID = 8;
    const int32 addParticleSystemID = 9;

    switch (_dialogMenu->getSelectionIndex())
    {
    case cutID:
        // TODO
        break;

    case copyID:
        // TODO
        break;

    case pasteID:
        // TODO
        break;

    case deleteID:
        // TODO
        break;

    case addTransformID:
        _addTransformation(_selectedNode);
        break;

    case addGroupID:
        _addGroup(_selectedNode);
        break;

    case addSwitchID:
        _addSwitch(_selectedNode);
        break;

    case addModelID:
        _addModel(_selectedNode);
        break;

    case addEmitterID:
        _addEmitter(_selectedNode);
        break;

    case addParticleSystemID:
        _addParticleSystem(_selectedNode);
        break;
    }

    delete _dialogMenu;
}

void UserControlGraphView::setSelectedNode(uint64 nodeID)
{
    if (nodeID == iNode::INVALID_NODE_ID)
    {
        _gridGraph->unselect();
        return;
    }

    int32 rowCount = _gridGraph->getRowCount();
    for (int row = 0; row < rowCount; ++row)
    {
        std::any userData = _gridGraph->getUserData(0, row);
        uint64 id = iNode::INVALID_NODE_ID;
        if (userData.has_value())
        {
            id = std::any_cast<uint64>(userData);
        }

        if (nodeID == id)
        {
            _gridGraph->select(0, row);
            return;
        }
    }

    _gridGraph->unselect();
}

uint64 UserControlGraphView::getSelectedNode() const
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
    _selectedNode = iNode::INVALID_NODE_ID;
}

bool UserControlGraphView::preOrderVisit(iNodePtr node, iNodePtr nextSibling)
{
    if (!_firstNode)
    {
        iaString indentString;
        for (int i = 0; i < _indentation; ++i)
        {
            indentString += "   ";
        }

        uint32 currentRowIndex = _gridGraph->getRowCount() - 1;
        uint32 currentCollumnIndex = _indentation++;

        iWidgetGrid* entry = new iWidgetGrid();
        entry->setSelectMode(iSelectionMode::NoSelection);
        entry->setBorder(0);
        entry->setCellSpacing(2);
        entry->appendCollumns(2);
        entry->setHorizontalAlignment(iHorizontalAlignment::Left);
        entry->setWidth(330);
        _gridGraph->addWidget(entry, 0, currentRowIndex, node->getID());

        iWidgetLabel* indentLabel = new iWidgetLabel();
        indentLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
        indentLabel->setVerticalAlignment(iVerticalAlignment::Center);
        indentLabel->setText(indentString);

        iWidgetLabel* label = new iWidgetLabel();
        label->setHorizontalAlignment(iHorizontalAlignment::Right);
        label->setVerticalAlignment(iVerticalAlignment::Center);

        iWidgetPicture* icon = new iWidgetPicture();
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

        _gridGraph->appendRows(1);
    }

    _firstNode = false;

    return true;
}

void UserControlGraphView::postOrderVisit(iNodePtr node)
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

void UserControlGraphView::onAddTransformation(const iWidgetPtr source)
{
    _addTransformation(_selectedNode);
}

void UserControlGraphView::onAddGroup(const iWidgetPtr source)
{
    _addGroup(_selectedNode);
}

void UserControlGraphView::onAddEmitter(const iWidgetPtr source)
{
    _addEmitter(_selectedNode);
}

void UserControlGraphView::onAddParticleSystem(const iWidgetPtr source)
{
    _addParticleSystem(_selectedNode);
}

void UserControlGraphView::onAddSwitch(const iWidgetPtr source)
{
    _addSwitch(_selectedNode);
}

void UserControlGraphView::onAddModel(const iWidgetPtr source)
{
    _addModel(_selectedNode);
}