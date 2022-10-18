// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/dialogs/iDialogIndexMenu.h>

#include <igor/ui/dialogs/iDialog.h>
#include <igor/ui/iWidgetManager.h>
#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetPicture.h>
#include <igor/ui/widgets/iWidgetGrid.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

	iDialogIndexMenu::iDialogIndexMenu(const iWidgetPtr parent)
		: iDialog(iWidgetType::iDialogIndexMenu, parent)
	{
	}

	void iDialogIndexMenu::open(iDialogCloseDelegate dialogCloseDelegate, std::vector<iaString> &texts)
	{
		std::vector<iaString> pictures;
		open(dialogCloseDelegate, texts, pictures);
	}

	void iDialogIndexMenu::open(iDialogCloseDelegate dialogCloseDelegate, std::vector<iaString> &texts, std::vector<iaString> &pictures)
	{
		iDialog::open(dialogCloseDelegate);

		if (pictures.empty())
		{
			initGUI(texts);
		}
		else
		{
			initGUI(texts, pictures);
		}
	}

	void iDialogIndexMenu::initGUI(std::vector<iaString> &texts)
	{
		iWidgetManager::getInstance().setModal(this);
		setActive();
		setVisible();
		setHeight(0);
		setAcceptOutOfBoundsClicks();

		registerOnMouseOffClickEvent(iMouseOffClickDelegate(this, &iDialogIndexMenu::onMouseOffClick));

		iWidgetGridPtr grid = new iWidgetGrid();
		grid->appendRows(static_cast<uint32>(texts.size()) - 1);
		grid->setHorizontalAlignment(iHorizontalAlignment::Left);
		grid->setVerticalAlignment(iVerticalAlignment::Top);
		grid->setSelectMode(iSelectionMode::Row);
		grid->setCellSpacing(4);
		grid->setBorder(4);
		grid->registerOnChangeEvent(iChangeDelegate(this, &iDialogIndexMenu::onChange));
		addWidget(grid);

		for (int i = 0; i < texts.size(); ++i)
		{
			iWidgetLabel *label = new iWidgetLabel();
			label->setHorizontalAlignment(iHorizontalAlignment::Left);
			label->setText(texts[i]);
			grid->addWidget(label, 0, i);
		}
	}

	void iDialogIndexMenu::initGUI(std::vector<iaString> &texts, std::vector<iaString> &pictures)
	{
		con_assert_sticky(texts.size() == pictures.size(), "invalid data");

		iWidgetManager::getInstance().setModal(this);
		setActive();
		setVisible();
		setHeight(0);
		setAcceptOutOfBoundsClicks();

		registerOnMouseOffClickEvent(iMouseOffClickDelegate(this, &iDialogIndexMenu::onMouseOffClick));

		iWidgetGridPtr grid = new iWidgetGrid();
		grid->appendColumns(1);
		grid->appendRows(static_cast<uint32>(texts.size()) - 1);
		grid->setHorizontalAlignment(iHorizontalAlignment::Left);
		grid->setVerticalAlignment(iVerticalAlignment::Top);
		grid->setSelectMode(iSelectionMode::Row);
		grid->setCellSpacing(4);
		grid->setBorder(4);
		grid->registerOnChangeEvent(iChangeDelegate(this, &iDialogIndexMenu::onChange));
		addWidget(grid);

		for (int i = 0; i < texts.size(); ++i)
		{
			if (!pictures[i].isEmpty())
			{
				iWidgetPicture *picture = new iWidgetPicture();
				picture->setTexture(pictures[i]);
				picture->setMaxSize(20, 20);
				grid->addWidget(picture, 0, i);
			}

			iWidgetLabel *label = new iWidgetLabel();
			label->setHorizontalAlignment(iHorizontalAlignment::Left);
			label->setText(texts[i]);
			grid->addWidget(label, 1, i);
		}
	}

	int32 iDialogIndexMenu::getSelectionIndex() const
	{
		return _returnValue;
	}

	void iDialogIndexMenu::onMouseOffClick(const iWidgetPtr source)
	{
		close();
	}

	void iDialogIndexMenu::onChange(const iWidgetPtr source)
	{
		_returnValue = static_cast<iWidgetGridPtr>(source)->getSelectedRow();
		close();
	}

} // namespace igor