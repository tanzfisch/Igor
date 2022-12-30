// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlMaterialView.h"

UserControlMaterialView::UserControlMaterialView()
{
    initGUI();
}

void UserControlMaterialView::initGUI()
{
    iWidgetGrid *grid = new iWidgetGrid(this);
    grid->appendRows(1);
    grid->setBorder(2);
    grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
    grid->setVerticalAlignment(iVerticalAlignment::Strech);
    grid->setStrechColumn(0);
    grid->setStrechRow(1);

    iWidgetGrid *gridButtons = new iWidgetGrid();
    gridButtons->setBorder(0);
    gridButtons->appendColumns(10);
    gridButtons->setCellSpacing(2);
    gridButtons->setHorizontalAlignment(iHorizontalAlignment::Left);
    gridButtons->setVerticalAlignment(iVerticalAlignment::Top);

    iWidgetButton *addMaterialButton = new iWidgetButton();
    addMaterialButton->setText("");
    addMaterialButton->setTooltip("Create new material.");
    addMaterialButton->setWidth(30);
    addMaterialButton->setHeight(30);
    addMaterialButton->setTexture("icons/addMaterial.png");
    addMaterialButton->registerOnClickEvent(iClickDelegate(this, &UserControlMaterialView::onAddMaterial));

    iWidgetGroupBox *groupBox = new iWidgetGroupBox();
    groupBox->setText("Graph");
    groupBox->setHorizontalAlignment(iHorizontalAlignment::Strech);
    groupBox->setVerticalAlignment(iVerticalAlignment::Strech);

    iWidgetScroll *scroll = new iWidgetScroll();
    scroll->setHorizontalAlignment(iHorizontalAlignment::Strech);
    scroll->setVerticalAlignment(iVerticalAlignment::Strech);

    _gridGraph = new iWidgetGrid();
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

void UserControlMaterialView::onAddMaterial(const iWidgetPtr source)
{
    _addMaterial();
}

void UserControlMaterialView::OnSelectionChange(iWidgetPtr widget)
{
    iMaterialID materialID;
    std::any userData = _gridGraph->getSelectedUserData();

    if (userData.has_value())
    {
        materialID = std::any_cast<iMaterialID>(userData);
    }

    _selectedMaterial = materialID;
    _materialSelectionChanged(_selectedMaterial);
}

void UserControlMaterialView::clear()
{
    _gridGraph->clearChildren();
}

void UserControlMaterialView::refresh()
{
    clear();

    uint32 currentRowIndex = 0;
    std::vector<iMaterialPtr> materials;
    iMaterialResourceFactory::getInstance().getMaterials(materials);
    
    for (auto material : materials)
    {
        iWidgetGrid *entry = new iWidgetGrid();
        entry->setSelectMode(iSelectionMode::NoSelection);
        entry->setBorder(0);
        entry->setCellSpacing(2);
        entry->setHorizontalAlignment(iHorizontalAlignment::Left);

        iWidgetLabel *label = new iWidgetLabel();
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
        _gridGraph->addWidget(entry, 0, currentRowIndex++, material->getID());
        _gridGraph->appendRows(1);
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
