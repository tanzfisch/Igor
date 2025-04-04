// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "ShopDialog.h"

ShopDialog::ShopDialog(const iWidgetPtr parent)
{
	initGUI();
}

void ShopDialog::open(iDialogCloseDelegate dialogCloseDelegate, int coins, const std::vector<ShopItem> &shopItems)
{
	_shopItems = shopItems;
	_buy = false;
	_itemGrid->unselect();	

	std::set<int> indices;

	do
	{
		indices.insert(iaRandom::getNext() % _shopItems.size());
	} while (indices.size() < 3);

	auto iter = indices.begin();
	_option1 = (*iter++);
	_option2 = (*iter++);
	_option3 = (*iter++);

	updateGUI(coins);
	iDialog::open(dialogCloseDelegate, true);
}

void ShopDialog::updateGUI(int coins)
{
	_labelCoins->setText(iaString::toString(coins));

	const ShopItem &shopItem1 = _shopItems[_option1];
	const ShopItem &shopItem2 = _shopItems[_option2];
	const ShopItem &shopItem3 = _shopItems[_option3];

	_labelName1->setText(shopItem1._name);
	_labelName2->setText(shopItem2._name);
	_labelName3->setText(shopItem3._name);

	_labelDescription1->setText(shopItem1._description);
	_labelDescription2->setText(shopItem2._description);
	_labelDescription3->setText(shopItem3._description);

	
	_picture1->setTexture(iResourceManager::getInstance().loadResource<iTexture>(shopItem1._icon));
	_picture2->setTexture(iResourceManager::getInstance().loadResource<iTexture>(shopItem2._icon));
	_picture3->setTexture(iResourceManager::getInstance().loadResource<iTexture>(shopItem3._icon));

	_labelPrice1->setText(iaString("$") + iaString::toString(shopItem1._price));
	_labelPrice2->setText(iaString("$") + iaString::toString(shopItem2._price));
	_labelPrice3->setText(iaString("$") + iaString::toString(shopItem3._price));
}

const ShopItem &ShopDialog::getSelection() const
{
	return _shopItems[_selection];
}

void ShopDialog::initGUI()
{
	setMinWidth(500);
	setMinHeight(200);

	setHorizontalAlignment(iHorizontalAlignment::Center);
	setVerticalAlignment(iVerticalAlignment::Center);

	iWidgetGridLayout *grid = new iWidgetGridLayout(this);
	grid->appendRows(1);
	grid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
	grid->setVerticalAlignment(iVerticalAlignment::Stretch);
	grid->setBorder(10);
	grid->setCellSpacing(20);

	iWidgetGridLayout *coinsGrid = new iWidgetGridLayout();
	coinsGrid->appendColumns(1);
	coinsGrid->setStretchColumn(1);
	coinsGrid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
	coinsGrid->setVerticalAlignment(iVerticalAlignment::Stretch);

	iWidgetGridLayout *headerGrid = new iWidgetGridLayout();
	headerGrid->appendColumns(3);
	headerGrid->setHorizontalAlignment(iHorizontalAlignment::Stretch);
	headerGrid->setVerticalAlignment(iVerticalAlignment::Stretch);
	headerGrid->setCellSpacing(20);
	headerGrid->setStretchColumn(1);

	_itemGrid = new iWidgetGridLayout();
	_itemGrid->appendColumns(2);
	_itemGrid->setHorizontalAlignment(iHorizontalAlignment::Center);
	_itemGrid->setVerticalAlignment(iVerticalAlignment::Stretch);
	_itemGrid->setCellSpacing(0);
	_itemGrid->setStretchColumn(1);
	_itemGrid->setStretchRow(0);
	_itemGrid->setSelectMode(iSelectionMode::Column);
	_itemGrid->setEmptyCellsSelectable();
	_itemGrid->registerOnChangeEvent(iChangeDelegate(this, &ShopDialog::onSelectionChanged));

	iWidgetGridLayout *item1Grid = new iWidgetGridLayout();
	item1Grid->appendRows(4);
	item1Grid->setHorizontalAlignment(iHorizontalAlignment::Center);
	item1Grid->setVerticalAlignment(iVerticalAlignment::Top);
	item1Grid->setStretchRow(3);

	iWidgetGridLayout *item2Grid = new iWidgetGridLayout();
	item2Grid->appendRows(4);
	item2Grid->setHorizontalAlignment(iHorizontalAlignment::Center);
	item2Grid->setVerticalAlignment(iVerticalAlignment::Top);
	item1Grid->setStretchRow(3);

	iWidgetGridLayout *item3Grid = new iWidgetGridLayout();
	item3Grid->appendRows(4);
	item3Grid->setHorizontalAlignment(iHorizontalAlignment::Center);
	item3Grid->setVerticalAlignment(iVerticalAlignment::Top);
	item1Grid->setStretchRow(3);

	_labelName1 = new iWidgetLabel();
	_labelName2 = new iWidgetLabel();
	_labelName3 = new iWidgetLabel();

	_labelPrice1 = new iWidgetLabel();
	_labelPrice1->setHorizontalAlignment(iHorizontalAlignment::Center);
	_labelPrice2 = new iWidgetLabel();
	_labelPrice2->setHorizontalAlignment(iHorizontalAlignment::Center);
	_labelPrice3 = new iWidgetLabel();
	_labelPrice3->setHorizontalAlignment(iHorizontalAlignment::Center);

	_labelDescription1 = new iWidgetLabel();
	_labelDescription1->setMaxTextWidth(128);
	_labelDescription1->setHorizontalAlignment(iHorizontalAlignment::Center);
	_labelDescription2 = new iWidgetLabel();
	_labelDescription2->setMaxTextWidth(128);
	_labelDescription2->setHorizontalAlignment(iHorizontalAlignment::Center);
	_labelDescription3 = new iWidgetLabel();
	_labelDescription3->setMaxTextWidth(128);
	_labelDescription3->setHorizontalAlignment(iHorizontalAlignment::Center);

	_picture1 = new iWidgetPicture();
	_picture1->setMinSize(128, 128);
	_picture1->setVerticalAlignment(iVerticalAlignment::Center);
	_picture1->setHorizontalAlignment(iHorizontalAlignment::Center);

	_picture2 = new iWidgetPicture();
	_picture2->setMinSize(128, 128);
	_picture2->setVerticalAlignment(iVerticalAlignment::Center);
	_picture2->setHorizontalAlignment(iHorizontalAlignment::Center);

	_picture3 = new iWidgetPicture();
	_picture3->setMinSize(128, 128);
	_picture3->setVerticalAlignment(iVerticalAlignment::Center);
	_picture3->setHorizontalAlignment(iHorizontalAlignment::Center);

	_itemGrid->addWidget(item1Grid, 0, 0);
	_itemGrid->addWidget(item2Grid, 1, 0);
	_itemGrid->addWidget(item3Grid, 2, 0);

	item1Grid->addWidget(_picture1, 0, 0);
	item1Grid->addWidget(_labelName1, 0, 1);
	item1Grid->addWidget(_labelPrice1, 0, 2);
	item1Grid->addWidget(_labelDescription1, 0, 3);

	item2Grid->addWidget(_picture2, 0, 0);
	item2Grid->addWidget(_labelName2, 0, 1);
	item2Grid->addWidget(_labelPrice2, 0, 2);
	item2Grid->addWidget(_labelDescription2, 0, 3);

	item3Grid->addWidget(_picture3, 0, 0);
	item3Grid->addWidget(_labelName3, 0, 1);
	item3Grid->addWidget(_labelPrice3, 0, 2);
	item3Grid->addWidget(_labelDescription3, 0, 3);

	_labelCoins = new iWidgetLabel();
	iWidgetPicturePtr coinPicture = new iWidgetPicture();
	coinPicture->setTexture(iResourceManager::getInstance().loadResource<iTexture>("example_texture_supremacy_coin"));

	iWidgetButton *buyButton = new iWidgetButton();
	buyButton->setMinSize(50, 20);
	buyButton->setVerticalAlignment(iVerticalAlignment::Center);
	buyButton->setHorizontalAlignment(iHorizontalAlignment::Center);
	buyButton->setText("buy");
	buyButton->registerOnClickEvent(iClickDelegate(this, &ShopDialog::onBuy));

	iWidgetButton *cancelButton = new iWidgetButton();
	cancelButton->setMinSize(50, 20);
	cancelButton->setVerticalAlignment(iVerticalAlignment::Center);
	cancelButton->setHorizontalAlignment(iHorizontalAlignment::Center);
	cancelButton->setText("cancel");
	cancelButton->registerOnClickEvent(iClickDelegate(this, &ShopDialog::onCancel));

	coinsGrid->addWidget(coinPicture, 0, 0);
	coinsGrid->addWidget(_labelCoins, 1, 0);

	headerGrid->addWidget(coinsGrid, 0, 0);
	headerGrid->addWidget(cancelButton, 2, 0);
	headerGrid->addWidget(buyButton, 3, 0);

	grid->addWidget(headerGrid, 0, 0);
	grid->addWidget(_itemGrid, 0, 1);
}

void ShopDialog::onSelectionChanged(const iWidgetPtr source)
{
	iWidgetGridLayoutPtr grid = static_cast<iWidgetGridLayoutPtr>(source);

	switch (grid->getSelectedColumn())
	{
	case 0:
		_selection = _option1;
		break;
	case 1:
		_selection = _option2;
		break;
	case 2:
		_selection = _option3;
		break;
	}

	_buy = true;
}

void ShopDialog::onBuy(const iWidgetPtr source)
{
	close();
}

void ShopDialog::onCancel(const iWidgetPtr source)
{
	_buy = false;
	close();
}

bool ShopDialog::bought() const
{
	return _buy;
}