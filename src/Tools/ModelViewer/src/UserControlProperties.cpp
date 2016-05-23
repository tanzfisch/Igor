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
#include "UserControlModel.h"
#include "UserControlEmitter.h"
#include "UserControlParticleSystem.h"

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
	_groupBox->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
	_groupBox->setVerticalAlignment(iVerticalAlignment::Strech);

    _scroll = static_cast<iWidgetScroll*>(iWidgetManager::getInstance().createWidget(iWidgetType::Scroll));
    _scroll->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    _scroll->setVerticalAlignment(iVerticalAlignment::Strech);

    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _grid->appendRows(1);
    _grid->setBorder(2);
    _grid->setCellSpacing(2);
    _grid->setWidth(0);
    _grid->setHeight(0);
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

        case iNodeType::iNodeModel:
            deinitModel();
            break;

        case iNodeType::iNodeEmitter:
            deinitEmitter();
            break;

        case iNodeType::iNodeParticleSystem:
            deinitParticleSystem();
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

        case iNodeType::iNodeModel:
            initModel();
            break;

        case iNodeType::iNodeEmitter:
            initEmitter();
            break;

        case iNodeType::iNodeParticleSystem:
            initParticleSystem();
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

void UserControlProperties::deinitMeshNode()
{
    if (_userControlMesh != nullptr)
    {
        _grid->removeWidget(_userControlMesh->getWidget());

        delete _userControlMesh;
        _userControlMesh = nullptr;
    }
}

void UserControlProperties::initModel()
{
    con_assert(_userControlModel == nullptr, "mem allocation error");

    _userControlModel = new UserControlModel();
    _grid->addWidget(_userControlModel->getWidget(), 0, 1);
    _userControlModel->setNode(_nodeID);
}

void UserControlProperties::deinitModel()
{
    if (_userControlModel != nullptr)
    {
        _grid->removeWidget(_userControlModel->getWidget());

        delete _userControlModel;
        _userControlModel = nullptr;
    }
}

void UserControlProperties::initEmitter()
{
    con_assert(_userControlEmitter == nullptr, "mem allocation error");

    _userControlEmitter = new UserControlEmitter();
    _grid->addWidget(_userControlEmitter->getWidget(), 0, 1);
    _userControlEmitter->setNode(_nodeID);
}

void UserControlProperties::deinitEmitter()
{
    if (_userControlEmitter != nullptr)
    {
        _grid->removeWidget(_userControlEmitter->getWidget());

        delete _userControlEmitter;
        _userControlEmitter = nullptr;
    }
}

void UserControlProperties::initParticleSystem()
{
    con_assert(_userControlParticleSystem == nullptr, "mem allocation error");

    _userControlParticleSystem = new UserControlParticleSystem();
    _grid->addWidget(_userControlParticleSystem->getWidget(), 0, 1);
    _userControlParticleSystem->setNode(_nodeID);
}

void UserControlProperties::deinitParticleSystem()
{
    if (_userControlParticleSystem != nullptr)
    {
        _grid->removeWidget(_userControlParticleSystem->getWidget());

        delete _userControlParticleSystem;
        _userControlParticleSystem = nullptr;
    }
}

void UserControlProperties::clear()
{
    setNode(iNode::INVALID_NODE_ID);
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