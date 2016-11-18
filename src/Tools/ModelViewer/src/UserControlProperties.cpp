#include "UserControlProperties.h"

#include <iWidgetManager.h>
#include <iDialog.h>
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
#include "UserControlMaterial.h"

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
    _groupBox = static_cast<iWidgetGroupBox*>(iWidgetManager::getInstance().createWidget("GroupBox"));
    _groupBox->setText("Properties");
	_groupBox->setHorizontalAlignment(iHorizontalAlignment::Strech);
	_groupBox->setVerticalAlignment(iVerticalAlignment::Strech);

    _scroll = static_cast<iWidgetScroll*>(iWidgetManager::getInstance().createWidget("Scroll"));
    _scroll->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _scroll->setVerticalAlignment(iVerticalAlignment::Strech);

    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _grid->appendRows(1);
    _grid->setCellSpacing(2);
    _grid->setWidth(300);
    _grid->setHeight(0);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Left);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);

    _groupBox->addWidget(_scroll);
    _scroll->addWidget(_grid);
}

void UserControlProperties::deinitGUI()
{
	setProperty(0, PropertyType::Undefined);
    iWidgetManager::getInstance().destroyWidget(_grid);
    iWidgetManager::getInstance().destroyWidget(_scroll);
    iWidgetManager::getInstance().destroyWidget(_groupBox);
}

iDialog* UserControlProperties::getDialog()
{
    return _dialog;
}

void UserControlProperties::setProperty(uint64 id, PropertyType propertyType)
{
	iNodeTransform* node = nullptr;

	switch (_propertyType)
	{
	case PropertyType::Node:
		node = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(static_cast<uint32>(_propertyID)));
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

			if (_userControlNode != nullptr)
			{
                _grid->removeWidget(_userControlNode->getWidget());
				_userControlNode->unregisterNameChangeDelegate(NameChangedDelegate(this, &UserControlProperties::onNameChanged));
				delete _userControlNode;
				_userControlNode = nullptr;
			}
		}
		break;

	case PropertyType::Material:
        deinitMaterial();
		break;

	case PropertyType::Undefined:
		// nothing to do
		break;

	default:
		con_err("unknown type");
	}

	_propertyType = propertyType;
    _propertyID = id;

	switch (_propertyType)
	{
	case PropertyType::Node:
		node = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_propertyID));
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

			if (_userControlNode == nullptr)
			{
				_userControlNode = new UserControlNode();
				_userControlNode->registerNameChangeDelegate(NameChangedDelegate(this, &UserControlProperties::onNameChanged));
				_userControlNode->setNode(_propertyID);
				_grid->addWidget(_userControlNode->getWidget(), 0, 0);
			}
		}
		break;

	case PropertyType::Material:
        initMaterial();
		break;

	case PropertyType::Undefined:
		// nothing to do
		break;

	default:
		con_err("unknown type");
	}
    
    
}

void UserControlProperties::onNameChanged()
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

    if (_userControlMesh == nullptr)
    {
        _userControlMesh = new UserControlMesh();
        _grid->addWidget(_userControlMesh->getWidget(), 0, 1);
        _userControlMesh->setNode(static_cast<uint32>(_propertyID));
    }
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

    if (_userControlModel == nullptr)
    {
        _userControlModel = new UserControlModel();
        _grid->addWidget(_userControlModel->getWidget(), 0, 1);
        _userControlModel->setNode(static_cast<uint32>(_propertyID));
    }
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

    if (_userControlEmitter == nullptr)
    {
        _userControlEmitter = new UserControlEmitter();
        _grid->addWidget(_userControlEmitter->getWidget(), 0, 1);
        _userControlEmitter->setNode(static_cast<uint32>(_propertyID));
    }
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

    if (_userControlParticleSystem == nullptr)
    {
        _userControlParticleSystem = new UserControlParticleSystem();
        _grid->addWidget(_userControlParticleSystem->getWidget(), 0, 1);
        _userControlParticleSystem->setNode(static_cast<uint32>(_propertyID));
    }
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

void UserControlProperties::initMaterial()
{
    con_assert(_userControlMaterial == nullptr, "mem allocation error");

    if (_userControlMaterial == nullptr)
    {
        _userControlMaterial = new UserControlMaterial();
        _grid->addWidget(_userControlMaterial->getWidget(), 0, 1);
        _userControlMaterial->setMaterial(static_cast<uint32>(_propertyID));
        _userControlMaterial->registerNameChangeDelegate(MaterialNameChangedDelegate(this, &UserControlProperties::onNameChanged));
    }
    
}

void UserControlProperties::deinitMaterial()
{
    if (_userControlMaterial != nullptr)
    {
        _userControlMaterial->unregisterNameChangeDelegate(MaterialNameChangedDelegate(this, &UserControlProperties::onNameChanged));

        _grid->removeWidget(_userControlMaterial->getWidget());

        delete _userControlMaterial;
        _userControlMaterial = nullptr;
    }
}

void UserControlProperties::clear()
{
    setProperty(0, PropertyType::Undefined);
}

void UserControlProperties::initLightNode()
{
    con_assert(_userControlLight == nullptr, "mem allocation error");

    _userControlLight = new UserControlLight();
    _grid->addWidget(_userControlLight->getWidget(), 0, 1);
    _userControlLight->setNode(static_cast<uint32>(_propertyID));
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
    _userControlTransformation->setNode(static_cast<uint32>(_propertyID));
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