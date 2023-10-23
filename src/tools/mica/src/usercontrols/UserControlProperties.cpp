#include "UserControlProperties.h"

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

void UserControlProperties::initGUI()
{
    iWidgetScrollPtr scroll = new iWidgetScroll(this);
    scroll->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    scroll->setVerticalAlignment(iVerticalAlignment::Stretch);

    _grid = new iWidgetGridLayout();
    _grid->setBorder(5);
    _grid->appendRows(1);
    _grid->setCellSpacing(2);
    _grid->setWidth(340);
    _grid->setHeight(0);
    _grid->setHorizontalAlignment(iHorizontalAlignment::Left);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);
    scroll->addWidget(_grid);
}

void UserControlProperties::cleanUpPropertyUI()
{
    switch (_propertyType)
    {
    case PropertyType::Node:
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
            _grid->removeWidget(_userControlNode);
            _userControlNode->unregisterNameChangeDelegate(NameChangedDelegate(this, &UserControlProperties::onNameChanged));

            delete _userControlNode;
            _userControlNode = nullptr;
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
}

void UserControlProperties::setMaterial(const iMaterialID &materialID)
{
    cleanUpPropertyUI();

    _materialID = materialID;
    _propertyType = PropertyType::Material;

    initMaterial();
}

void UserControlProperties::setNode(const iNodeID &nodeID)
{
    cleanUpPropertyUI();

    _propertyType = PropertyType::Node;
    _nodeID = nodeID;

    iNodePtr node = iNodeManager::getInstance().getNode(nodeID);
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
            _userControlNode->setNode(nodeID);
            _grid->addWidget(_userControlNode, 0, 0);
        }
    }
}

void UserControlProperties::onNameChanged()
{
    _structureChangedEvent();
}

void UserControlProperties::registerPropertiesChangedDelegate(PropertiesChangedDelegate propertiesChangedDelegate)
{
    _propertiesChangedEvent.add(propertiesChangedDelegate);
}

void UserControlProperties::unregisterPropertiesChangedDelegate(PropertiesChangedDelegate propertiesChangedDelegate)
{
    _propertiesChangedEvent.remove(propertiesChangedDelegate);
}

void UserControlProperties::registerStructureChangedDelegate(StructureChangedDelegate structureChangedDelegate)
{
    _structureChangedEvent.add(structureChangedDelegate);
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
        _grid->addWidget(_userControlMesh, 0, 1);
        _userControlMesh->setNode(static_cast<uint32>(_nodeID));
    }
}

void UserControlProperties::deinitMeshNode()
{
    if (_userControlMesh != nullptr)
    {
        _grid->removeWidget(_userControlMesh);

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
        _grid->addWidget(_userControlModel, 0, 1);
        _userControlModel->setNode(static_cast<uint32>(_nodeID));
    }
}

void UserControlProperties::deinitModel()
{
    if (_userControlModel != nullptr)
    {
        _grid->removeWidget(_userControlModel);

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
        _grid->addWidget(_userControlEmitter, 0, 1);
        _userControlEmitter->setNode(static_cast<uint32>(_nodeID));
    }
}

void UserControlProperties::deinitEmitter()
{
    if (_userControlEmitter != nullptr)
    {
        _grid->removeWidget(_userControlEmitter);

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
        _grid->addWidget(_userControlParticleSystem, 0, 1);
        _userControlParticleSystem->setNode(static_cast<uint32>(_nodeID));
    }
}

void UserControlProperties::deinitParticleSystem()
{
    if (_userControlParticleSystem != nullptr)
    {
        _grid->removeWidget(_userControlParticleSystem);
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
        _grid->addWidget(_userControlMaterial, 0, 1);
        _userControlMaterial->setMaterial(_materialID);
        _userControlMaterial->registerNameChangeDelegate(MaterialNameChangedDelegate(this, &UserControlProperties::onNameChanged));
    }
}

void UserControlProperties::deinitMaterial()
{
    if (_userControlMaterial != nullptr)
    {
        _userControlMaterial->unregisterNameChangeDelegate(MaterialNameChangedDelegate(this, &UserControlProperties::onNameChanged));

        _grid->removeWidget(_userControlMaterial);

        delete _userControlMaterial;
        _userControlMaterial = nullptr;
    }
}

void UserControlProperties::clear()
{
    setNode(0);
}

void UserControlProperties::initLightNode()
{
    con_assert(_userControlLight == nullptr, "mem allocation error");

    _userControlLight = new UserControlLight();
    _grid->addWidget(_userControlLight, 0, 1);
    _userControlLight->setNode(static_cast<uint32>(_nodeID));
}

void UserControlProperties::deinitLightNode()
{
    if (_userControlLight != nullptr)
    {
        _grid->removeWidget(_userControlLight);

        delete _userControlLight;
        _userControlLight = nullptr;
    }
}

void UserControlProperties::initTransformNode()
{
    con_assert(_userControlTransformation == nullptr, "mem allocation error");

    _userControlTransformation = new UserControlTransformation();
    _grid->addWidget(_userControlTransformation, 0, 1);
    _userControlTransformation->setNode(static_cast<uint32>(_nodeID));
}

void UserControlProperties::deinitTransformNode()
{
    if (_userControlTransformation != nullptr)
    {
        _grid->removeWidget(_userControlTransformation);

        delete _userControlTransformation;
        _userControlTransformation = nullptr;
    }
}