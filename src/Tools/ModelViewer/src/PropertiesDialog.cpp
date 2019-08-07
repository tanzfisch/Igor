#include "PropertiesDialog.h"

#include <iWidgetManager.h>
#include <iDialog.h>
#include <iNode.h>
#include <iNodeTransform.h>
#include <iNodeLight.h>
#include <iNodeSwitch.h>
#include <iNodeFactory.h>

#include <iWidgetScroll.h>
#include <iWidgetGrid.h>
#include <iDialog.h>
#include <iWidgetButton.h>
#include <iWidgetSpacer.h>
#include <iDialogMessageBox.h>
#include <iDialogDecisionBox.h>
using namespace Igor;

#include <iaConsole.h>
using namespace IgorAux;

#include "UserControlTransformation.h"
#include "UserControlLight.h"

PropertiesDialog::PropertiesDialog()
{
    initGUI();
}

PropertiesDialog::~PropertiesDialog()
{
}

void PropertiesDialog::initGUI()
{
	setWidth(350);
    setHorizontalAlignment(iHorizontalAlignment::Right);
    setVerticalAlignment(iVerticalAlignment::Strech);

    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _grid->setBorder(2);
    _grid->setCellSpacing(8);
	_grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
    _grid->setVerticalAlignment(iVerticalAlignment::Strech);
	_grid->setStrechRow(0);
    _grid->setStrechColumn(0);
    
    _userControlProperties.registerStructureChangedDelegate(StructureChangedDelegate(this, &PropertiesDialog::onStructureChanged));
	_grid->addWidget(&_userControlProperties, 0, 0);

    addWidget(_grid);
}

void PropertiesDialog::onStructureChanged()
{
    _structureChangedEvent();
}

void PropertiesDialog::onGraphViewSelectionChanged(uint64 nodeID)
{
    _userControlProperties.setProperty(nodeID, PropertyType::Node);
}

void PropertiesDialog::onMaterialSelectionChanged(uint64 materialID)
{
	_userControlProperties.setProperty(materialID, PropertyType::Material);
}

void PropertiesDialog::registerPropertiesChangedDelegate(PropertiesChangedDelegate propertiesChangedDelegate)
{
    _propertiesChangedEvent.append(propertiesChangedDelegate);
}

void PropertiesDialog::unregisterPropertiesChangedDelegate(PropertiesChangedDelegate propertiesChangedDelegate)
{
    _propertiesChangedEvent.remove(propertiesChangedDelegate);
}

void PropertiesDialog::registerStructureChangedDelegate(StructureChangedDelegate structureChangedDelegate)
{
    _structureChangedEvent.append(structureChangedDelegate);
}

void PropertiesDialog::unregisterStructureChangedDelegate(StructureChangedDelegate structureChangedDelegate)
{
    _structureChangedEvent.remove(structureChangedDelegate);
}
