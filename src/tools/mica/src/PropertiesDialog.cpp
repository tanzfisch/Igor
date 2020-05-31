#include "PropertiesDialog.h"

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/dialogs/iDialog.h>
#include <igor/graphics/scene/nodes/iNode.h>
#include <igor/graphics/scene/nodes/iNodeTransform.h>
#include <igor/graphics/scene/nodes/iNodeLight.h>
#include <igor/graphics/scene/nodes/iNodeSwitch.h>
#include <igor/graphics/scene/nodes/iNodeManager.h>

#include <igor/ui/widgets/iWidgetScroll.h>
#include <igor/ui/widgets/iWidgetGrid.h>
#include <igor/ui/dialogs/iDialog.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/ui/widgets/iWidgetSpacer.h>
#include <igor/ui/dialogs/iDialogMessageBox.h>
#include <igor/ui/dialogs/iDialogDecisionBox.h>
using namespace Igor;

#include <iaux/system/iaConsole.h>
using namespace IgorAux;

#include "UserControlTransformation.h"
#include "UserControlLight.h"

PropertiesDialog::PropertiesDialog()
{
    initGUI();
}

void PropertiesDialog::initGUI()
{
	setWidth(350);
    setHorizontalAlignment(iHorizontalAlignment::Right);
    setVerticalAlignment(iVerticalAlignment::Strech);

    iWidgetGridPtr grid = new iWidgetGrid(this);
    grid->setBorder(2);
    grid->setCellSpacing(8);
	grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
    grid->setVerticalAlignment(iVerticalAlignment::Strech);
	grid->setStrechRow(0);
    grid->setStrechColumn(0);
    
    _userControlProperties = new UserControlProperties();
    _userControlProperties->registerStructureChangedDelegate(StructureChangedDelegate(this, &PropertiesDialog::onStructureChanged));
	grid->addWidget(_userControlProperties, 0, 0);
}

void PropertiesDialog::onStructureChanged()
{
    _structureChangedEvent();
}

void PropertiesDialog::onGraphViewSelectionChanged(uint64 nodeID)
{
    _userControlProperties->setProperty(nodeID, PropertyType::Node);
}

void PropertiesDialog::onMaterialSelectionChanged(uint64 materialID)
{
	_userControlProperties->setProperty(materialID, PropertyType::Material);
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
