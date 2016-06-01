#include "PropertiesDialog.h"

#include <iWidgetManager.h>
#include <iWidgetDialog.h>
#include <iNode.h>
#include <iNodeTransform.h>
#include <iNodeLight.h>
#include <iNodeSwitch.h>
#include <iNodeFactory.h>

#include <iWidgetScroll.h>
#include <iWidgetGrid.h>
#include <iWidgetDialog.h>
#include <iWidgetButton.h>
#include <iWidgetGroupBox.h>
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
    deinitGUI();
}

void PropertiesDialog::initGUI()
{
	getDialog()->setWidth(350);
    getDialog()->setHorrizontalAlignment(iHorrizontalAlignment::Right);
    getDialog()->setVerticalAlignment(iVerticalAlignment::Strech);

    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allwidgets.push_back(_grid);
    _grid->setBorder(2);
    _grid->setCellSpacing(8);
	_grid->setHorrizontalAlignment(iHorrizontalAlignment::Strech);
    _grid->setVerticalAlignment(iVerticalAlignment::Strech);
	_grid->setStrechRow(0);
    
    _userControlProperties = new UserControlProperties();
    _userControlProperties->registerStructureChangedDelegate(StructureChangedDelegate(this, &PropertiesDialog::onStructureChanged));

    getDialog()->addWidget(_grid);
    _grid->addWidget(_userControlProperties->getWidget(), 0, 0);
}

void PropertiesDialog::onStructureChanged()
{
    _structureChangedEvent();
}

void PropertiesDialog::onGraphViewSelectionChanged(uint32 nodeID)
{
    _userControlProperties->setProperty(static_cast<uint64>(nodeID), PropertyType::Node);
}

void PropertiesDialog::onMaterialSelectionChanged(uint32 materialID)
{
	_userControlProperties->setProperty(static_cast<uint64>(materialID), PropertyType::Material);
}

void PropertiesDialog::deinitGUI()
{
    getDialog()->removeWidget(_grid);

    if (_userControlProperties != nullptr)
    {
        _userControlProperties->unregisterStructureChangedDelegate(StructureChangedDelegate(this, &PropertiesDialog::onStructureChanged));
        delete _userControlProperties;
        _userControlProperties = nullptr;
    }

    for (auto widget : _allwidgets)
    {
        iWidgetManager::getInstance().destroyWidget(widget);
    }
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
