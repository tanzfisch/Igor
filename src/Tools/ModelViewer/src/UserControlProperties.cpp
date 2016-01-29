#include "UserControlProperties.h"

#include <iWidgetManager.h>
#include <iWidgetDialog.h>
#include <iNode.h>
#include <iNodeTransform.h>
#include <iNodeLight.h>
#include <iNodeMesh.h>
#include <iNodeFactory.h>

#include <iWidgetScroll.h>
#include <iWidgetGrid.h>
#include <iWidgetGroupBox.h>
using namespace Igor;

#include <iaConsole.h>
using namespace IgorAux;

#include "UserControlTransformation.h"
#include "UserControlLight.h"
#include "UserControlMesh.h"
#include "UserControlNode.h"

UserControlProperties::UserControlProperties()
{
    initGUI();
}

UserControlProperties::~UserControlProperties()
{
    deinitGUI();
}

iWidget* UserControlProperties::getWidget()
{
    return _groupBox;
}

void UserControlProperties::initGUI()
{
    _groupBox = static_cast<iWidgetGroupBox*>(iWidgetManager::getInstance().createWidget(iWidgetType::GroupBox));
    _groupBox->setText("Properties");
    _groupBox->setWidth(300);

    _scroll = static_cast<iWidgetScroll*>(iWidgetManager::getInstance().createWidget(iWidgetType::Scroll));
    _scroll->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _scroll->setVerticalAlignment(iVerticalAlignment::Top);
    _scroll->setWidth(360);
    _scroll->setHeight(400);

    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _grid->appendRows(1);
    _grid->setBorder(2);
    _grid->setCellSpacing(2);
    _grid->setHorrizontalAlignment(iHorrizontalAlignment::Left);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);

    _userControlNode = new UserControlNode();
    _userControlNode->registerNameChangeDelegate(NameChangedDelegate(this, &UserControlProperties::onNodeNameChanged));
    _userControlNode->getWidget()->setVisible(false);

    _groupBox->addWidget(_scroll);
    _scroll->addWidget(_grid);
    _grid->addWidget(_userControlNode->getWidget(), 0, 0);
}

void UserControlProperties::deinitGUI()
{
    setNode(iNode::INVALID_NODE_ID);
    iWidgetManager::getInstance().destroyWidget(_grid);
    iWidgetManager::getInstance().destroyWidget(_scroll);
    iWidgetManager::getInstance().destroyWidget(_groupBox);

    if (_userControlNode != nullptr)
    {
        _userControlNode->unregisterNameChangeDelegate(NameChangedDelegate(this, &UserControlProperties::onNodeNameChanged));
        delete _userControlNode;
        _userControlNode = nullptr;
    }
}

iWidgetDialog* UserControlProperties::getDialog()
{
    return _dialog;
}

uint32 UserControlProperties::getNode()
{
    return _nodeID;
}

void UserControlProperties::setNode(uint32 id)
{
    if (_nodeID == id)
    {
        return;
    }

    _userControlNode->getWidget()->setVisible();

    iNodeTransform* node = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_nodeID));
    if (node != nullptr)
    {
        switch (_currentNodeType)
        {
        case iNodeType::iNodeTransform:
            deinitTransformNode();
            break;

        case iNodeType::iNodeLight:
            deinitLightNode();
            break;

        case iNodeType::iNodeMesh:
            deinitMeshNode();
            break;
        }

        _currentNodeType = iNodeType::Undefined;
    }

    _nodeID = id;
    
    node = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_nodeID));
    if (node != nullptr)
    {
        _currentNodeType = node->getType();

        switch (_currentNodeType)
        {
        case iNodeType::iNode:
            // nothing to do
            break;

        case iNodeType::iNodeTransform:
            initTransformNode();
            break;

        case iNodeType::iNodeLight:
            initLightNode();
            break;

        case iNodeType::iNodeMesh:
            initMeshNode();
            break;

        default:
            con_warn("not implemented");
        }

        _userControlNode->setNode(_nodeID);
    }
}

void UserControlProperties::onNodeNameChanged()
{
    _structureChangedEvent();
}

void UserControlProperties::registerPropertiesChangedDelegate(PropertiesChangedDelegate propertiesChangedDelegate)
{
    _propertiesChangedEvent.append(propertiesChangedDelegate);
}

void UserControlProperties::unregisterPropertiesChangedDelegate(PropertiesChangedDelegate propertiesChangedDelegate)
{
    _propertiesChangedEvent.remove(propertiesChangedDelegate);
}

void UserControlProperties::registerStructureChangedDelegate(StructureChangedDelegate structureChangedDelegate)
{
    _structureChangedEvent.append(structureChangedDelegate);
}

void UserControlProperties::unregisterStructureChangedDelegate(StructureChangedDelegate structureChangedDelegate)
{
    _structureChangedEvent.remove(structureChangedDelegate);
}

void UserControlProperties::initMeshNode()
{
    con_assert(_userControlMesh == nullptr, "mem allocation error");

    _userControlMesh = new UserControlMesh();
    _grid->addWidget(_userControlMesh->getWidget(), 0, 1);
    _userControlMesh->setNode(_nodeID);
}

void UserControlProperties::clear()
{
    setNode(iNode::INVALID_NODE_ID);
}

void UserControlProperties::deinitMeshNode()
{
    if (_userControlMesh != nullptr)
    {
        _grid->removeWidget(_userControlMesh->getWidget());

        delete _userControlMesh;
        _userControlMesh = nullptr;
    }
}

void UserControlProperties::initLightNode()
{
    con_assert(_userControlLight == nullptr, "mem allocation error");

    _userControlLight = new UserControlLight();
    _grid->addWidget(_userControlLight->getWidget(), 0, 1);
    _userControlLight->setNode(_nodeID);
}

void UserControlProperties::deinitLightNode()
{
    if (_userControlLight != nullptr)
    {
        _grid->removeWidget(_userControlLight->getWidget());

        delete _userControlLight;
        _userControlLight = nullptr;
    }
}

void UserControlProperties::initTransformNode()
{
    con_assert(_userControlTransformation == nullptr, "mem allocation error");

    _userControlTransformation = new UserControlTransformation();
    _grid->addWidget(_userControlTransformation->getWidget(), 0, 1);
    _userControlTransformation->setNode(_nodeID);
}

void UserControlProperties::deinitTransformNode()
{
    if (_userControlTransformation != nullptr)
    {
        _grid->removeWidget(_userControlTransformation->getWidget());

        delete _userControlTransformation;
        _userControlTransformation = nullptr;
    }
}