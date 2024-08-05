// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include "DialogComponentTypeSelection.h"

DialogComponentTypeSelection::DialogComponentTypeSelection(const iWidgetPtr parent)
	: iDialog(iWidgetType::iDialog, parent)
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
		_selectBoxComponentType->addItem(componentType.second.second, componentType.first);
	}

	iWidgetBoxLayoutPtr buttonLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal, mainLayout);
	buttonLayout->setHorizontalAlignment(iHorizontalAlignment::Right);

	_cancelButton = new iWidgetButton(buttonLayout);
	_cancelButton->setText("Cancel");
	_cancelButton->registerOnClickEvent(iClickDelegate(this, &DialogComponentTypeSelection::onCancel));

	_okButton = new iWidgetButton(buttonLayout);
	_okButton->setText("Ok");
	_okButton->registerOnClickEvent(iClickDelegate(this, &DialogComponentTypeSelection::onOK));
}

void DialogComponentTypeSelection::onCancel(iWidgetPtr source)
{
	close();
}

void DialogComponentTypeSelection::onOK(iWidgetPtr source)
{
}
