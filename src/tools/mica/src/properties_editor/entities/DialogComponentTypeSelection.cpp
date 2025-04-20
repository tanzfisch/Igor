// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "DialogComponentTypeSelection.h"

DialogComponentTypeSelection::DialogComponentTypeSelection(const iWidgetPtr parent)
	: iDialog(iWidgetType::iDialog, parent), _selectedTypeIndex(typeid(void))
{
}

void DialogComponentTypeSelection::open(iDialogCloseDelegate dialogCloseDelegate)
{
	setTitle("Select Component Type");
	setVerticalAlignment(iVerticalAlignment::Center);
	setHorizontalAlignment(iHorizontalAlignment::Center);
	setResizeable(false);

	setMinWidth(20);
	setMinHeight(20);

	initGUI();

	iDialog::open(dialogCloseDelegate, true);
}

void DialogComponentTypeSelection::initGUI()
{
	iWidgetBoxLayoutPtr mainLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);
	mainLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);

	const auto &componentTypes = iEntitySystemModule::getInstance().getRegisteredComponentTypes();

	_selectBoxComponentType = new iWidgetSelectBox(mainLayout);
	for (const auto &componentType : componentTypes)
	{
		_selectBoxComponentType->addItem(componentType.second._typeName, componentType.first);
	}
	_selectBoxComponentType->getChangeEvent().add(iChangeDelegate(this, &DialogComponentTypeSelection::onTypeChanged));

	iWidgetBoxLayoutPtr buttonLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, mainLayout);
	buttonLayout->setHorizontalAlignment(iHorizontalAlignment::Right);

	_cancelButton = new iWidgetButton(buttonLayout);
	_cancelButton->setText("Cancel");
	_cancelButton->getClickEvent().add(iClickDelegate(this, &DialogComponentTypeSelection::onCancel));

	_okButton = new iWidgetButton(buttonLayout);
	_okButton->setText("Ok");
	_okButton->getClickEvent().add(iClickDelegate(this, &DialogComponentTypeSelection::onOK));
}

bool DialogComponentTypeSelection::hasSelectedTypeIndex() const
{
	return _selectedTypeIndex != typeid(void);
}

std::type_index DialogComponentTypeSelection::getSelectedTypeIndex() const
{
	con_assert(hasSelectedTypeIndex(), "no type index was selected");
	return _selectedTypeIndex;
}

void DialogComponentTypeSelection::onTypeChanged(iWidgetPtr source)
{
	_selectedTypeIndex = std::any_cast<std::type_index>(_selectBoxComponentType->getSelectedUserData());
}

void DialogComponentTypeSelection::onCancel(iWidgetPtr source)
{
	setReturnState(iDialogReturnState::Cancel);
	close();
}

void DialogComponentTypeSelection::onOK(iWidgetPtr source)
{
	setReturnState(iDialogReturnState::Ok);
	close();
}
