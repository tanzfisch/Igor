// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include "UpgradeDialog.h"

UpgradeDialog::UpgradeDialog(const iWidgetPtr parent)
{
	initGUI();
}

void UpgradeDialog::open(iDialogCloseDelegate dialogCloseDelegate, const UpgradeConfiguration &config1, const UpgradeConfiguration &config2, const UpgradeConfiguration &config3)
{
	_option1 = config1._type;
	_option2 = config2._type;
	_option3 = config3._type;

	updateGUI(config1, config2, config3);
	iDialog::open(dialogCloseDelegate);
	iWidgetManager::getInstance().setModal(this);

//	setActive();
//	setVisible();
}

void UpgradeDialog::updateGUI(const UpgradeConfiguration &config1, const UpgradeConfiguration &config2, const UpgradeConfiguration &config3)
{
	_labelName1->setText(config1._name);
	_labelName2->setText(config2._name);
	_labelName3->setText(config3._name);
	_labelDescription1->setText(config1._description);
	_labelDescription2->setText(config2._description);
	_labelDescription3->setText(config3._description);
}

void UpgradeDialog::initGUI()
{
	setWidth(500);
	setHeight(200);

	setHorizontalAlignment(iHorizontalAlignment::Center);
	setVerticalAlignment(iVerticalAlignment::Center);

	iWidgetGrid *grid = new iWidgetGrid(this);
	grid->appendColumns(2);
	grid->appendRows(2);
	grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
	grid->setVerticalAlignment(iVerticalAlignment::Strech);
	grid->setBorder(10);
	grid->setCellSpacing(20);
	grid->setStrechColumn(1);
	grid->setStrechRow(1);
	grid->setSelectMode(iSelectionMode::NoSelection);

	_labelName1 = new iWidgetLabel();
	_labelName2 = new iWidgetLabel();
	_labelName3 = new iWidgetLabel();

	_labelDescription1 = new iWidgetLabel();
	_labelDescription2 = new iWidgetLabel();
	_labelDescription3 = new iWidgetLabel();

	iWidgetButton *button1 = new iWidgetButton();
	button1->setSize(100, 100);
	button1->setVerticalAlignment(iVerticalAlignment::Center);
	button1->setHorizontalAlignment(iHorizontalAlignment::Center);
	button1->setText("1");
	button1->registerOnClickEvent(iClickDelegate(this, &UpgradeDialog::onSelect1));

	iWidgetButton *button2 = new iWidgetButton();
	button2->setSize(100, 100);
	button2->setVerticalAlignment(iVerticalAlignment::Center);
	button2->setHorizontalAlignment(iHorizontalAlignment::Center);
	button2->setText("2");
	button2->registerOnClickEvent(iClickDelegate(this, &UpgradeDialog::onSelect2));

	iWidgetButton *button3 = new iWidgetButton();
	button3->setSize(100, 100);
	button3->setVerticalAlignment(iVerticalAlignment::Center);
	button3->setHorizontalAlignment(iHorizontalAlignment::Center);
	button3->setText("3");
	button3->registerOnClickEvent(iClickDelegate(this, &UpgradeDialog::onSelect3));

	grid->addWidget(_labelName1, 0, 0);
	grid->addWidget(_labelName2, 1, 0);
	grid->addWidget(_labelName3, 2, 0);

	grid->addWidget(button1, 0, 1);
	grid->addWidget(button2, 1, 1);
	grid->addWidget(button3, 2, 1);

	grid->addWidget(_labelDescription1, 0, 2);
	grid->addWidget(_labelDescription2, 1, 2);
	grid->addWidget(_labelDescription3, 2, 2);
}

void UpgradeDialog::onSelect1(const iWidgetPtr source)
{
	_selection = _option1;
	close();
}

void UpgradeDialog::onSelect2(const iWidgetPtr source)
{
	_selection = _option2;
	close();
}

void UpgradeDialog::onSelect3(const iWidgetPtr source)
{
	_selection = _option3;
	close();
}

UpgradeType UpgradeDialog::getSelection() const
{
	return _selection;
}
