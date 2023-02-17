// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include "ShopDialog.h"

ShopDialog::ShopDialog(const iWidgetPtr parent)
{
	initGUI();
}

void ShopDialog::open(iDialogCloseDelegate dialogCloseDelegate, int coins)
{
	_selection = ShopItemType::None;

	updateGUI(coins);
	iDialog::open(dialogCloseDelegate);
	iWidgetManager::getInstance().setModal(this);
}

void ShopDialog::updateGUI(int coins)
{
	_labelCoins->setText(iaString::toString(coins));
}

ShopItemType ShopDialog::getSelection() const
{
	return _selection;
}

void ShopDialog::initGUI()
{
	setWidth(500);
	setHeight(200);

	setHorizontalAlignment(iHorizontalAlignment::Center);
	setVerticalAlignment(iVerticalAlignment::Center);

	iWidgetGrid *grid = new iWidgetGrid(this);
	grid->appendRows(1);
	grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
	grid->setVerticalAlignment(iVerticalAlignment::Strech);
	grid->setBorder(10);
	grid->setCellSpacing(20);

	iWidgetGrid *coinsGrid = new iWidgetGrid();
	coinsGrid->appendColumns(1);
	coinsGrid->setStrechColumn(1);
	coinsGrid->setHorizontalAlignment(iHorizontalAlignment::Strech);
	coinsGrid->setVerticalAlignment(iVerticalAlignment::Strech);

	iWidgetGrid *headerGrid = new iWidgetGrid();
	headerGrid->appendColumns(2);
	headerGrid->setHorizontalAlignment(iHorizontalAlignment::Strech);
	headerGrid->setVerticalAlignment(iVerticalAlignment::Strech);
	headerGrid->setCellSpacing(20);
	headerGrid->setStrechColumn(1);

	iWidgetGrid *itemGrid = new iWidgetGrid();
	itemGrid->appendColumns(2);
	itemGrid->appendRows(4);
	itemGrid->setHorizontalAlignment(iHorizontalAlignment::Strech);
	itemGrid->setVerticalAlignment(iVerticalAlignment::Strech);
	itemGrid->setCellSpacing(20);
	itemGrid->setStrechColumn(1);
	itemGrid->setStrechRow(0);

	_labelName1 = new iWidgetLabel();
	_labelName1->setText("megagun");
	_labelName2 = new iWidgetLabel();
	_labelName3 = new iWidgetLabel();

	_labelPrice1 = new iWidgetLabel();
	_labelPrice1->setText("1000$");
	_labelPrice2 = new iWidgetLabel();
	_labelPrice3 = new iWidgetLabel();

	_labelDescription1 = new iWidgetLabel();
	_labelDescription1->setText("mega booom gun hurt mucho");
	_labelDescription2 = new iWidgetLabel();
	_labelDescription3 = new iWidgetLabel();

	iWidgetButton *button1 = new iWidgetButton();
	button1->setSize(100, 100);
	button1->setVerticalAlignment(iVerticalAlignment::Center);
	button1->setHorizontalAlignment(iHorizontalAlignment::Center);
	// button1->setText("picture of gun");
	button1->setTexture("rocket.png");
	button1->registerOnClickEvent(iClickDelegate(this, &ShopDialog::onSelect1));

	iWidgetButton *button2 = new iWidgetButton();
	button2->setSize(100, 100);
	button2->setVerticalAlignment(iVerticalAlignment::Center);
	button2->setHorizontalAlignment(iHorizontalAlignment::Center);
	button2->setText("2");
	button2->registerOnClickEvent(iClickDelegate(this, &ShopDialog::onSelect2));

	iWidgetButton *button3 = new iWidgetButton();
	button3->setSize(100, 100);
	button3->setVerticalAlignment(iVerticalAlignment::Center);
	button3->setHorizontalAlignment(iHorizontalAlignment::Center);
	button3->setText("3");
	button3->registerOnClickEvent(iClickDelegate(this, &ShopDialog::onSelect3));

	itemGrid->addWidget(button1, 0, 0);
	itemGrid->addWidget(button2, 1, 0);
	itemGrid->addWidget(button3, 2, 0);

	itemGrid->addWidget(_labelName1, 0, 1);
	itemGrid->addWidget(_labelName2, 1, 1);
	itemGrid->addWidget(_labelName3, 2, 1);

	itemGrid->addWidget(_labelPrice1, 0, 2);
	itemGrid->addWidget(_labelPrice2, 1, 2);
	itemGrid->addWidget(_labelPrice3, 2, 2);

	itemGrid->addWidget(_labelDescription1, 0, 3);
	itemGrid->addWidget(_labelDescription2, 1, 3);
	itemGrid->addWidget(_labelDescription3, 2, 3);	

	_labelCoins = new iWidgetLabel();
	iWidgetPicturePtr coinPicture = new iWidgetPicture();
	coinPicture->setTexture("coin.png");

	iWidgetButton *exitButton = new iWidgetButton();
	exitButton->setSize(50, 20);
	exitButton->setVerticalAlignment(iVerticalAlignment::Center);
	exitButton->setHorizontalAlignment(iHorizontalAlignment::Center);
	exitButton->setText("exit");
	exitButton->registerOnClickEvent(iClickDelegate(this, &ShopDialog::onExit));	

	coinsGrid->addWidget(coinPicture, 0, 0);
	coinsGrid->addWidget(_labelCoins, 1, 0);

	headerGrid->addWidget(coinsGrid, 0, 0);
	headerGrid->addWidget(exitButton, 2, 0);

	grid->addWidget(headerGrid, 0, 0);
	grid->addWidget(itemGrid, 0, 1);
}

void ShopDialog::onSelect1(const iWidgetPtr source)
{
	_selection = _option1;
	close();
}

void ShopDialog::onSelect2(const iWidgetPtr source)
{
	_selection = _option2;
	close();
}

void ShopDialog::onSelect3(const iWidgetPtr source)
{
	_selection = _option3;
	close();
}

void ShopDialog::onExit(const iWidgetPtr source)
{
	close();
}