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
#include <iMessageBox.h>
#include <iDecisionBox.h>
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
    getDialog()->setHorrizontalAlignment(iHorrizontalAlignment::Right);
    getDialog()->setVerticalAlignment(iVerticalAlignment::Strech);

    _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
    _allwidgets.push_back(_grid);
    _grid->setBorder(2);
    _grid->setCellSpacing(8);
    _grid->setHorrizontalAlignment(iHorrizontalAlignment::Right);
    _grid->setVerticalAlignment(iVerticalAlignment::Top);
    
    _userControlProperties = new UserControlProperties();
    _userControlProperties->registerStructureChangedDelegate(StructureChangedDelegate(this, &PropertiesDialog::onStructureChanged));

    getDialog()->addWidget(_grid);
    _grid->addWidget(_userControlProperties->getWidget(), 0, 0);
}

void PropertiesDialog::onStructureChanged()
{
    // TODO trigger graph update
}

void PropertiesDialog::onGraphViewSelectionChanged(uint32 nodeID)
{
    _userControlProperties->setNode(nodeID);
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
