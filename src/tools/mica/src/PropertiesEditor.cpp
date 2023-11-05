// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "PropertiesEditor.h"

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
#include <igor/ui/widgets/iWidgetGridLayout.h>
#include <igor/ui/dialogs/iDialog.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/ui/widgets/iWidgetSpacer.h>
#include <igor/ui/dialogs/iDialogMessageBox.h>
#include <igor/ui/dialogs/iDialogDecisionBox.h>
using namespace igor;

#include <iaux/system/iaConsole.h>
using namespace iaux;

PropertiesEditor::PropertiesEditor()
{
    initGUI();
}

void PropertiesEditor::initGUI()
{
    setTitle("Properties Editor");

    setDockable(true);
    setWidth(370);
    setHorizontalAlignment(iHorizontalAlignment::Right);
    setVerticalAlignment(iVerticalAlignment::Stretch);

    iWidgetGridLayoutPtr grid = new iWidgetGridLayout(this);
    grid->setBorder(2);
    grid->setCellSpacing(8);
    grid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    grid->setVerticalAlignment(iVerticalAlignment::Stretch);
    grid->setStretchRow(0);
    grid->setStretchColumn(0);

    _userControlProperties = new UserControlProperties();
    _userControlProperties->registerStructureChangedDelegate(StructureChangedDelegate(this, &PropertiesEditor::onStructureChanged));
    grid->addWidget(_userControlProperties, 0, 0);
}

void PropertiesEditor::onStructureChanged()
{
    _structureChangedEvent();
}

void PropertiesEditor::onGraphViewSelectionChanged(uint64 nodeID)
{
    _userControlProperties->setNode(nodeID);
}

void PropertiesEditor::onMaterialSelectionChanged(const iMaterialID &materialID)
{
    _userControlProperties->setMaterial(materialID);
}

void PropertiesEditor::registerPropertiesChangedDelegate(PropertiesChangedDelegate propertiesChangedDelegate)
{
    _propertiesChangedEvent.add(propertiesChangedDelegate);
}

void PropertiesEditor::unregisterPropertiesChangedDelegate(PropertiesChangedDelegate propertiesChangedDelegate)
{
    _propertiesChangedEvent.remove(propertiesChangedDelegate);
}

void PropertiesEditor::registerStructureChangedDelegate(StructureChangedDelegate structureChangedDelegate)
{
    _structureChangedEvent.add(structureChangedDelegate);
}

void PropertiesEditor::unregisterStructureChangedDelegate(StructureChangedDelegate structureChangedDelegate)
{
    _structureChangedEvent.remove(structureChangedDelegate);
}
