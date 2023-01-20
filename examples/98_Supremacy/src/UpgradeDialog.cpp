// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include "UpgradeDialog.h"

UpgradeDialog::UpgradeDialog(const iWidgetPtr parent)
{
}

void UpgradeDialog::open(iDialogCloseDelegate dialogCloseDelegate, const UpgradeConfiguration &config1, const UpgradeConfiguration &config2, const UpgradeConfiguration &config3)
{
	iDialog::open(dialogCloseDelegate);
	
	initGUI(config1, config2, config3);
}

void UpgradeDialog::initGUI(const UpgradeConfiguration &config1, const UpgradeConfiguration &config2, const UpgradeConfiguration &config3)
{
	iWidgetManager::getInstance().setModal(this);
	setActive();
	setVisible();
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

	iWidgetLabel *labelName1 = new iWidgetLabel();
	labelName1->setText(config1._name);
	iWidgetLabel *labelName2 = new iWidgetLabel();
	labelName2->setText(config2._name);
	iWidgetLabel *labelName3 = new iWidgetLabel();
	labelName3->setText(config3._name);

	iWidgetLabel *labelDescription1 = new iWidgetLabel();
	labelDescription1->setText(config1._description);
	iWidgetLabel *labelDescription2 = new iWidgetLabel();
	labelDescription2->setText(config2._description);
	iWidgetLabel *labelDescription3 = new iWidgetLabel();
	labelDescription3->setText(config3._description);

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

	grid->addWidget(labelName1, 0, 0);
	grid->addWidget(labelName2, 1, 0);
	grid->addWidget(labelName3, 2, 0);

	grid->addWidget(button1, 0, 1);
	grid->addWidget(button2, 1, 1);
	grid->addWidget(button3, 2, 1);

	grid->addWidget(labelDescription1, 0, 2);
	grid->addWidget(labelDescription2, 1, 2);
	grid->addWidget(labelDescription3, 2, 2);

	_option1 = config1._type;
	_option2 = config2._type;
	_option3 = config3._type;
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
