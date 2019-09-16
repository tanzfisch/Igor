// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iDialogMenu.h>

#include <iDialog.h>
#include <iWidgetManager.h>
#include <iWidgetLabel.h>
#include <iWidgetPicture.h>
#include <iWidgetGrid.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    void iDialogMenu::open(iDialogClosedDelegate dialogCloseDelegate, std::vector<iaString>& texts, std::vector<iaString>& pictures)
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

	void iDialogMenu::initGUI(std::vector<iaString>& texts)
	{
		iWidgetManager::setModal(this);
		setActive();
		setVisible();
		setHeight(0);
		setAcceptOutOfBoundsClicks();

		registerOnMouseOffClickEvent(iMouseOffClickDelegate(this, &iDialogMenu::onMouseOffClick));

		iWidgetGridPtr grid = new iWidgetGrid();
		grid->appendRows(static_cast<uint32>(texts.size()) - 1);
		grid->setHorizontalAlignment(iHorizontalAlignment::Left);
		grid->setVerticalAlignment(iVerticalAlignment::Top);
		grid->setSelectMode(iSelectionMode::Row);
		grid->setCellSpacing(4);
		grid->setBorder(4);
		grid->registerOnChangeEvent(iChangeDelegate(this, &iDialogMenu::onChange));
		addWidget(grid);

		for (int i = 0; i < texts.size(); ++i)
		{
			iWidgetLabel* label = new iWidgetLabel();
			label->setHorizontalAlignment(iHorizontalAlignment::Left);
			label->setText(texts[i]);
			grid->addWidget(label, 0, i);
		}
	}

	void iDialogMenu::initGUI(std::vector<iaString>& texts, std::vector<iaString>& pictures)
	{
		con_assert_sticky(texts.size() == pictures.size(), "invalid data");

		iWidgetManager::setModal(this);
		setActive();
		setVisible();
		setHeight(0);
		setAcceptOutOfBoundsClicks();

		registerOnMouseOffClickEvent(iMouseOffClickDelegate(this, &iDialogMenu::onMouseOffClick));

		iWidgetGridPtr grid = new iWidgetGrid();
		grid->appendCollumns(1);
		grid->appendRows(static_cast<uint32>(texts.size()) - 1);
		grid->setHorizontalAlignment(iHorizontalAlignment::Left);
		grid->setVerticalAlignment(iVerticalAlignment::Top);
		grid->setSelectMode(iSelectionMode::Row);
		grid->setCellSpacing(4);
		grid->setBorder(4);
		grid->registerOnChangeEvent(iChangeDelegate(this, &iDialogMenu::onChange));
		addWidget(grid);

		for (int i = 0; i < texts.size(); ++i)
		{
			if (!pictures[i].isEmpty())
			{
				iWidgetPicture* picture = new iWidgetPicture();
				picture->setTexture(pictures[i]);
				picture->setMaxSize(20, 20);
				grid->addWidget(picture, 0, i);
			}

			iWidgetLabel* label = new iWidgetLabel();
			label->setHorizontalAlignment(iHorizontalAlignment::Left);
			label->setText(texts[i]);
			grid->addWidget(label, 1, i);
		}
	}

    int32 iDialogMenu::getSelectionIndex() const
    {
        return _returnValue;
    }

	void iDialogMenu::onMouseOffClick(iWidgetPtr source)
	{
		close();
	}

	void iDialogMenu::onChange(iWidgetPtr source)
	{
		_returnValue = static_cast<iWidgetGridPtr>(source)->getSelectedRow();
		close();
	}

}