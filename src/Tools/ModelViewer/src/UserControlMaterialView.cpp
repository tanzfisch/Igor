// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlMaterialView.h"

#include <iWidgetManager.h>
#include <iWidgetGrid.h>
#include <iWidgetTextEdit.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetSlider.h>
#include <iWidgetNumberChooser.h>
#include <iNodeEmitter.h>
#include <iWidgetSelectBox.h>
#include <iWidgetGroupBox.h>
#include <iWidgetScroll.h>
#include <iMesh.h>
#include <iNodeFactory.h>
#include <iTargetMaterial.h>
#include <iMaterialResourceFactory.h>
#include <iMaterial.h>
using namespace Igor;

UserControlMaterialView::UserControlMaterialView()
{
    initGUI();
}

void UserControlMaterialView::initGUI()
{
    iWidgetGrid* grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    grid->appendRows(1);
    grid->setBorder(2);
    grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
    grid->setVerticalAlignment(iVerticalAlignment::Strech);
    grid->setStrechColumn(0);
    grid->setStrechRow(1);
	addWidget(grid);

    iWidgetGrid* gridButtons = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    gridButtons->setBorder(0);
    gridButtons->appendCollumns(10);
    gridButtons->setCellSpacing(2);
    gridButtons->setHorizontalAlignment(iHorizontalAlignment::Left);
    gridButtons->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetButton* addMaterialButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
    addMaterialButton->setText("");
	addMaterialButton->setTooltip("Create new material.");
    addMaterialButton->setWidth(30);
    addMaterialButton->setHeight(30);
    addMaterialButton->setTexture("icons\\addMaterial.png");
    addMaterialButton->registerOnClickEvent(iClickDelegate(this, &UserControlMaterialView::onAddMaterial));

    iWidgetGroupBox* groupBox = static_cast<iWidgetGroupBox*>(iWidgetManager::getInstance().createWidget("GroupBox"));
    groupBox->setText("Graph");
    groupBox->setHorizontalAlignment(iHorizontalAlignment::Strech);
    groupBox->setVerticalAlignment(iVerticalAlignment::Strech);

    iWidgetScroll* scroll = static_cast<iWidgetScroll*>(iWidgetManager::getInstance().createWidget("Scroll"));
    scroll->setHorizontalAlignment(iHorizontalAlignment::Strech);
    scroll->setVerticalAlignment(iVerticalAlignment::Strech);

    _gridGraph = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
    _gridGraph->setBorder(0);
    _gridGraph->setSelectMode(iSelectionMode::Row);
    _gridGraph->setCellSpacing(0);
    _gridGraph->setHorizontalAlignment(iHorizontalAlignment::Left);
    _gridGraph->setVerticalAlignment(iVerticalAlignment::Top);
    _gridGraph->registerOnChangeEvent(iChangeDelegate(this, &UserControlMaterialView::OnSelectionChange));

    grid->addWidget(gridButtons, 0, 0);
    gridButtons->addWidget(addMaterialButton, 0, 0);
    grid->addWidget(groupBox, 0, 1);
    groupBox->addWidget(scroll);
    scroll->addWidget(_gridGraph);

    refresh();
}

void UserControlMaterialView::onAddMaterial(iWidget* source)
{
    _addMaterial();
}

void UserControlMaterialView::OnSelectionChange(iWidget* widget)
{
	uint32* materialIDPtr = static_cast<uint32*>(_gridGraph->getSelectedUserData());
	uint32 materialID = iMaterial::INVALID_MATERIAL_ID;
	if (materialIDPtr != nullptr)
	{
		materialID = *materialIDPtr;
	}

	_selectedMaterial = materialID;
	_materialSelectionChanged(_selectedMaterial);
}

void UserControlMaterialView::clear()
{
    for (auto widget : _gridEntryWidgets)
    {
        iWidgetManager::getInstance().destroyWidget(widget);
    }

    _gridEntryWidgets.clear();

    for (auto iter : _userData)
    {
        delete[] iter;
    }

    _userData.clear();
}

void UserControlMaterialView::refresh()
{
    clear();

    uint32 currentRowIndex = 0;
    auto materials = iMaterialResourceFactory::getInstance().getSortedMaterials();
    for(auto material : materials)
    {
        iWidgetGrid* entry = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
        entry->setSelectMode(iSelectionMode::NoSelection);
        entry->setBorder(0);
        entry->setCellSpacing(2);
        entry->setHorizontalAlignment(iHorizontalAlignment::Left);
        uint32* userData = new uint32();
        _userData.push_back(userData);
        *userData = material->getID();

        iWidgetLabel* label = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
        label->setHorizontalAlignment(iHorizontalAlignment::Right);
        label->setVerticalAlignment(iVerticalAlignment::Center);
        if (material->getName() != "")
        {
            label->setText(material->getName());
        }
        else
        {
            label->setText("<internal>");
        }

        entry->addWidget(label, 0, 0);
        _gridGraph->addWidget(entry, 0, currentRowIndex++, userData);
        _gridGraph->appendRows(1);

        _gridEntryWidgets.push_back(label);
        _gridEntryWidgets.push_back(entry);
    }
}

void UserControlMaterialView::registerOnAddMaterial(AddMaterialDelegate addMaterialDelegate)
{
    _addMaterial.append(addMaterialDelegate);
}

void UserControlMaterialView::unregisterOnAddMaterial(AddMaterialDelegate addMaterialDelegate)
{
    _addMaterial.remove(addMaterialDelegate);
}

void UserControlMaterialView::registerOnMaterialSelectionChanged(MaterialSelectionChangedDelegate materialSelectionChangedDelegate)
{
	_materialSelectionChanged.append(materialSelectionChangedDelegate);
}

void UserControlMaterialView::unregisterOnMaterialSelectionChanged(MaterialSelectionChangedDelegate materialSelectionChangedDelegate)
{
	_materialSelectionChanged.remove(materialSelectionChangedDelegate);
}
