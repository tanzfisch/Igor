#include "PropertiesDialog.h"

#include "usercontrols/UserControlTransformation.h"
#include "usercontrols/UserControlLight.h"

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/dialogs/iDialog.h>
#include <igor/scene/nodes/iNode.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodeLight.h>
#include <igor/scene/nodes/iNodeSwitch.h>
#include <igor/scene/nodes/iNodeManager.h>

#include <igor/ui/widgets/iWidgetScroll.h>
#include <igor/ui/widgets/iWidgetGrid.h>
#include <igor/ui/dialogs/iDialog.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/ui/widgets/iWidgetSpacer.h>
#include <igor/ui/dialogs/iDialogMessageBox.h>
#include <igor/ui/dialogs/iDialogDecisionBox.h>
using namespace igor;

#include <iaux/system/iaConsole.h>
using namespace iaux;

PropertiesDialog::PropertiesDialog()
{
    initGUI();
}

void PropertiesDialog::initGUI()
{
    setWidth(370);
    setHorizontalAlignment(iHorizontalAlignment::Right);
    setVerticalAlignment(iVerticalAlignment::Stretch);

    iWidgetGridPtr grid = new iWidgetGrid(this);
    grid->setBorder(2);
    grid->setCellSpacing(8);
    grid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    grid->setVerticalAlignment(iVerticalAlignment::Stretch);
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
    _userControlProperties->setNode(nodeID);
}

void PropertiesDialog::onMaterialSelectionChanged(const iMaterialID &materialID)
{
    _userControlProperties->setMaterial(materialID);
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
