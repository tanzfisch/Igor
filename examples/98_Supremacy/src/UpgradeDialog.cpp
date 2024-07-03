// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include "UpgradeDialog.h"

UpgradeDialog::UpgradeDialog(const iWidgetPtr parent)
{
	initGUI();
}

void UpgradeDialog::open(iDialogCloseDelegate dialogCloseDelegate, const std::vector<UpgradeConfiguration> &upgrades)
{
	_upgrades = upgrades;

	std::set<int> indices;

    do
    {
        indices.insert(iaRandom::getNext() % _upgrades.size());
    } while (indices.size() < 3);

    auto iter = indices.begin();
	_option1 = (*iter++);
	_option2 = (*iter++);
	_option3 = (*iter++);

	updateGUI();
	iDialog::open(dialogCloseDelegate);
	iWidgetManager::getInstance().setModal(this);
}

void UpgradeDialog::updateGUI()
{
	const UpgradeConfiguration &config1 = _upgrades[_option1];
	const UpgradeConfiguration &config2 = _upgrades[_option2];
	const UpgradeConfiguration &config3 = _upgrades[_option3];

	_labelName1->setText(config1._name);
	_labelName2->setText(config2._name);
	_labelName3->setText(config3._name);

	_labelDescription1->setText(config1._description);
	_labelDescription2->setText(config2._description);
	_labelDescription3->setText(config3._description);

	_button1->setTexture(config1._icon);
	_button2->setTexture(config2._icon);
	_button3->setTexture(config3._icon);
}

void UpgradeDialog::initGUI()
{
	setMinWidth(500);
	setMinHeight(100);

	setBackground(iaColor4f::transparent);

	setHorizontalAlignment(iHorizontalAlignment::Center);
	setVerticalAlignment(iVerticalAlignment::Center);

	iWidgetGridLayout *grid = new iWidgetGridLayout(this);
	grid->appendColumns(2);
	grid->appendRows(2);
	grid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
	grid->setVerticalAlignment(iVerticalAlignment::Stretch);
	grid->setBorder(10);
	grid->setCellSpacing(10);
	grid->setStretchColumn(1);
	grid->setStretchRow(1);
	grid->setSelectMode(iSelectionMode::NoSelection);

	_labelName1 = new iWidgetLabel();
	_labelName2 = new iWidgetLabel();
	_labelName3 = new iWidgetLabel();

	_labelDescription1 = new iWidgetLabel();
	_labelDescription1->setMaxTextWidth(150);
	_labelDescription2 = new iWidgetLabel();
	_labelDescription2->setMaxTextWidth(150);
	_labelDescription3 = new iWidgetLabel();
	_labelDescription3->setMaxTextWidth(150);

	_button1 = new iWidgetButton();
	_button1->setMinSize(128, 128);
	_button1->setVerticalAlignment(iVerticalAlignment::Center);
	_button1->setHorizontalAlignment(iHorizontalAlignment::Center);
	_button1->registerOnClickEvent(iClickDelegate(this, &UpgradeDialog::onSelect1));

	_button2 = new iWidgetButton();
	_button2->setMinSize(128, 128);
	_button2->setVerticalAlignment(iVerticalAlignment::Center);
	_button2->setHorizontalAlignment(iHorizontalAlignment::Center);
	_button2->registerOnClickEvent(iClickDelegate(this, &UpgradeDialog::onSelect2));

	_button3 = new iWidgetButton();
	_button3->setMinSize(128, 128);
	_button3->setVerticalAlignment(iVerticalAlignment::Center);
	_button3->setHorizontalAlignment(iHorizontalAlignment::Center);
	_button3->registerOnClickEvent(iClickDelegate(this, &UpgradeDialog::onSelect3));

	grid->addWidget(_labelName1, 0, 0);
	grid->addWidget(_labelName2, 1, 0);
	grid->addWidget(_labelName3, 2, 0);

	grid->addWidget(_button1, 0, 1);
	grid->addWidget(_button2, 1, 1);
	grid->addWidget(_button3, 2, 1);

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

const UpgradeConfiguration& UpgradeDialog::getSelection() const
{
	return _upgrades[_selection];
}
