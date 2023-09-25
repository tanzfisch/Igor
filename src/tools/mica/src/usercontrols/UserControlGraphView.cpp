// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlGraphView.h"

#include "../actions/ActionContext.h"

UserControlGraphView::UserControlGraphView(WorkspacePtr workspace, Outliner *outliner) // TODO UserControlGraphView should not know the Outliner
    : _outliner(outliner), _workspace(workspace)
{
    initGUI();
}

UserControlGraphView::~UserControlGraphView()
{
    if (_graphContextMenu != nullptr)
    {
        delete _graphContextMenu;
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

iNodeID UserControlGraphView::getRootNode()
{
    return _root;
}

void UserControlGraphView::initGUI()
{
    iWidgetGrid *grid = new iWidgetGrid();
    grid->setBorder(0);
    grid->appendRows(1);
    grid->setCellSpacing(2);
    grid->setStretchRow(1);
    grid->setStretchColumn(0);
    grid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    grid->setVerticalAlignment(iVerticalAlignment::Stretch);
    addWidget(grid);

    iWidgetGrid *gridButtons = new iWidgetGrid();
    gridButtons->setBorder(0);
    gridButtons->appendColumns(10);
    gridButtons->setCellSpacing(2);
    gridButtons->setHorizontalAlignment(iHorizontalAlignment::Left);
    gridButtons->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetButton *addTransformationButton = new iWidgetButton();
    addTransformationButton->setText("");
    addTransformationButton->setWidth(30);
    addTransformationButton->setHeight(30);
    addTransformationButton->setTooltip("Add transformation node. You need to move something you need a transform node.");
    addTransformationButton->setTexture("igor_icon_add_transformation");
    addTransformationButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddTransformation));

    iWidgetButton *addModelButton = new iWidgetButton();
    addModelButton->setText("");
    addModelButton->setWidth(30);
    addModelButton->setHeight(30);
    addModelButton->setTooltip("Add model node. Adds a model embedded or referenced to the scene.");
    addModelButton->setTexture("igor_icon_add_model");
    addModelButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddModel));

    iWidgetButton *addGroupButton = new iWidgetButton();
    addGroupButton->setText("");
    addGroupButton->setWidth(30);
    addGroupButton->setHeight(30);
    addGroupButton->setTooltip("Add group node. Used to create node hierarchies in the scene.");
    addGroupButton->setTexture("igor_icon_add_group");
    addGroupButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddGroup));

    iWidgetButton *addEmitterButton = new iWidgetButton();
    addEmitterButton->setText("");
    addEmitterButton->setWidth(30);
    addEmitterButton->setHeight(30);
    addEmitterButton->setTooltip("Add emitter node. To emit particles from.");
    addEmitterButton->setTexture("igor_icon_add_emitter");
    addEmitterButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddEmitter));

    iWidgetButton *addParticleSystemButton = new iWidgetButton();
    addParticleSystemButton->setText("");
    addParticleSystemButton->setWidth(30);
    addParticleSystemButton->setHeight(30);
    addParticleSystemButton->setTooltip("Add particle system node. Needs emitter node to function");
    addParticleSystemButton->setTexture("igor_icon_add_particle_system");
    addParticleSystemButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddParticleSystem));

    iWidgetButton *addSwitchButton = new iWidgetButton();
    addSwitchButton->setText("");
    addSwitchButton->setWidth(30);
    addSwitchButton->setHeight(30);
    addSwitchButton->setTooltip("Add switch node. This node can switch the active state of it's child nodes. Only the active node will be visible.");
    addSwitchButton->setTexture("igor_icon_add_switch");
    addSwitchButton->registerOnClickEvent(iClickDelegate(this, &UserControlGraphView::onAddSwitch));

    iWidgetGroupBox *groupBox = new iWidgetGroupBox();
    groupBox->setText("Graph");
    groupBox->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    groupBox->setVerticalAlignment(iVerticalAlignment::Stretch);

    iWidgetScroll *scroll = new iWidgetScroll();
    scroll->setVerticalAlignment(iVerticalAlignment::Stretch);
    scroll->setHorizontalAlignment(iHorizontalAlignment::Stretch);

    _gridGraph = new iWidgetGrid();
    _gridGraph->setAcceptOutOfBoundsClicks();
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
        return "igor_icon_camera";
    case iNodeType::iNodeLight:
        return "igor_icon_light";
    case iNodeType::iNodeMesh:
        return "igor_icon_mesh";
    case iNodeType::iNodeModel:
        return "igor_icon_model";
    case iNodeType::iNodeSkyBox:
        return "igor_icon_skybox";
    case iNodeType::iNode:
        return "igor_icon_group";
    case iNodeType::iNodeSwitch:
        return "igor_icon_switch";
    case iNodeType::iNodeTransform:
        return "igor_icon_transformation";
    case iNodeType::iNodeEmitter:
        return "igor_icon_emitter";
    case iNodeType::iNodeParticleSystem:
        return "igor_icon_particleSystem";

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

iActionContextPtr UserControlGraphView::getContext()
{
    return iActionContextPtr(new ActionContext(_workspace, _outliner));
}

void UserControlGraphView::OnContextMenu(iWidgetPtr widget)
{
    if (_graphContextMenu != nullptr)
    {
        delete _graphContextMenu;
        _graphContextMenu = nullptr;
    }

    _graphContextMenu = new iDialogMenu(this);
    _graphContextMenu->setWidth(100);

    iaVector2i pos = iMouse::getInstance().getPos();
    _graphContextMenu->setX(pos._x);
    _graphContextMenu->setY(pos._y);

    ActionContext *actionContext = new ActionContext(_workspace, _outliner);
    iActionContextPtr ac(actionContext);

    if (!actionContext->getWorkspace()->getSelection().empty())
    {

        iWidgetMenuPtr addMenu = new iWidgetMenu();
        addMenu->setTitle("Add");
        addMenu->addAction("mica:addTransform", ac);
        addMenu->addAction("mica:addGroup", ac);
        addMenu->addAction("mica:addSwitch", ac);
        addMenu->addAction("mica:addModel", ac);
        addMenu->addAction("mica:addEmitter", ac);
        addMenu->addAction("mica:addParticleSystem", ac);
        _graphContextMenu->addMenu(addMenu);
    }

    _graphContextMenu->addSpacer();

    _graphContextMenu->addAction("mica:cutNodes", ac);
    _graphContextMenu->addAction("mica:copyNodes", ac);
    _graphContextMenu->addAction("mica:pasteNodes", ac);
    _graphContextMenu->addAction("mica:deleteNodes", ac);

    _graphContextMenu->addSpacer();

    iWidgetMenuPtr actionsMenu = new iWidgetMenu();
    actionsMenu->setTitle("Actions");
    actionsMenu->addAction("mica:bakeMeshToWorld", ac);
    _graphContextMenu->addMenu(actionsMenu);

    _graphContextMenu->open(iDialogCloseDelegate(this, &UserControlGraphView::OnContextMenuClose));
}

void UserControlGraphView::OnContextMenuClose(iDialogPtr dialog)
{
    delete _graphContextMenu;
    _graphContextMenu = nullptr;
}

void UserControlGraphView::setSelectedNode(uint64 nodeID)
{
    _selectedNode = nodeID;
    _selectionChange(_selectedNode);

    if (_selectedNode == iNode::INVALID_NODE_ID)
    {
        _gridGraph->blockEvents();
        _gridGraph->unselect();
        _gridGraph->unblockEvents();
        return;
    }

    int32 rowCount = _gridGraph->getRowCount();
    for (int row = 0; row < rowCount; ++row)
    {
        iNodeID id = std::any_cast<iNodeID>(_gridGraph->getUserData(0, row));
        if (_selectedNode == id)
        {
            _gridGraph->blockEvents();
            _gridGraph->select(0, row);
            _gridGraph->unblockEvents();
            return;
        }
    }

    _gridGraph->blockEvents();
    _gridGraph->unselect();
    _gridGraph->unblockEvents();
    _selectedNode = iNode::INVALID_NODE_ID;
}

iNodeID UserControlGraphView::getSelectedNode() const
{
    return _selectedNode;
}

void UserControlGraphView::registerOnSelectionChange(GraphSelectionChangedDelegate selectionChangeDelegate)
{
    _selectionChange.add(selectionChangeDelegate);
}

void UserControlGraphView::unregisterOnSelectionChange(GraphSelectionChangedDelegate selectionChangeDelegate)
{
    _selectionChange.remove(selectionChangeDelegate);
}

void UserControlGraphView::clearGraph()
{
    _gridGraph->blockEvents();
    _gridGraph->clear();
    _gridGraph->unblockEvents();
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
        uint32 currentColumnIndex = _indentation++;

        iWidgetGrid *entry = new iWidgetGrid();
        entry->setSelectMode(iSelectionMode::NoSelection);
        entry->setBorder(0);
        entry->setCellSpacing(2);
        entry->appendColumns(2);
        entry->setHorizontalAlignment(iHorizontalAlignment::Left);
        entry->setWidth(330);
        _gridGraph->addWidget(entry, 0, currentRowIndex, node->getID());

        iWidgetLabel *indentLabel = new iWidgetLabel();
        indentLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
        indentLabel->setVerticalAlignment(iVerticalAlignment::Center);
        indentLabel->setText(indentString);

        iWidgetLabel *label = new iWidgetLabel();
        label->setHorizontalAlignment(iHorizontalAlignment::Right);
        label->setVerticalAlignment(iVerticalAlignment::Center);

        iWidgetPicture *icon = new iWidgetPicture();
        icon->setMaxSize(24, 24);

        if (node->getParent() != nullptr)
        {
            icon->setTexture(getIconTexture(node->getType()));
            label->setText(node->getName());
        }
        else
        {
            icon->setTexture("igor_icon_root");
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

void UserControlGraphView::onAddTransformation(const iWidgetPtr source)
{
    iActionManager::getInstance().getAction("mica:addTransform")->execute(*getContext());
}

void UserControlGraphView::onAddGroup(const iWidgetPtr source)
{
    iActionManager::getInstance().getAction("mica:addGroup")->execute(*getContext());
}

void UserControlGraphView::onAddEmitter(const iWidgetPtr source)
{
    iActionManager::getInstance().getAction("mica:addEmitter")->execute(*getContext());
}

void UserControlGraphView::onAddParticleSystem(const iWidgetPtr source)
{
    iActionManager::getInstance().getAction("mica:addParticleSystem")->execute(*getContext());
}

void UserControlGraphView::onAddSwitch(const iWidgetPtr source)
{
    iActionManager::getInstance().getAction("mica:addSwitch")->execute(*getContext());
}

void UserControlGraphView::onAddModel(const iWidgetPtr source)
{
    iActionManager::getInstance().getAction("mica:addModel")->execute(*getContext());
}