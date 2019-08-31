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

	iDialogMenu::~iDialogMenu()
	{
		deinitGUI();
	}

	void iDialogMenu::initGUI(std::vector<iaString>& texts)
	{
		iWidgetManager::setModal(this);
		setActive();
		setVisible();
		setHeight(0);
		setAcceptOutOfBoundsClicks();

		registerOnMouseOffClickEvent(iMouseOffClickDelegate(this, &iDialogMenu::onMouseOffClick));

		_grid = new iWidgetGrid();
		_grid->appendRows(static_cast<uint32>(texts.size()) - 1);
		_grid->setHorizontalAlignment(iHorizontalAlignment::Left);
		_grid->setVerticalAlignment(iVerticalAlignment::Top);
		_grid->setSelectMode(iSelectionMode::Row);
		_grid->setCellSpacing(4);
		_grid->setBorder(4);
		_grid->registerOnChangeEvent(iChangeDelegate(this, &iDialogMenu::onChange));
		addWidget(_grid);

		for (int i = 0; i < texts.size(); ++i)
		{
			iWidgetLabel* label = new iWidgetLabel();
			label->setHorizontalAlignment(iHorizontalAlignment::Left);
			label->setText(texts[i]);
			_grid->addWidget(label, 0, i);
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

		_grid = new iWidgetGrid();
		_grid->appendCollumns(1);
		_grid->appendRows(static_cast<uint32>(texts.size()) - 1);
		_grid->setHorizontalAlignment(iHorizontalAlignment::Left);
		_grid->setVerticalAlignment(iVerticalAlignment::Top);
		_grid->setSelectMode(iSelectionMode::Row);
		_grid->setCellSpacing(4);
		_grid->setBorder(4);
		_grid->registerOnChangeEvent(iChangeDelegate(this, &iDialogMenu::onChange));
		addWidget(_grid);

		for (int i = 0; i < texts.size(); ++i)
		{
			if (!pictures[i].isEmpty())
			{
				iWidgetPicture* picture = new iWidgetPicture();
				picture->setTexture(pictures[i]);
				picture->setMaxSize(_entryHeight, _entryHeight);
				_grid->addWidget(picture, 0, i);
			}

			iWidgetLabel* label = new iWidgetLabel();
			label->setHorizontalAlignment(iHorizontalAlignment::Left);
			label->setText(texts[i]);
			_grid->addWidget(label, 1, i);
		}
	}

	void iDialogMenu::deinitGUI()
	{
		unregisterOnMouseOffClickEvent(iMouseOffClickDelegate(this, &iDialogMenu::onMouseOffClick));

		if (_grid != nullptr)
		{
			_grid->unregisterOnChangeEvent(iChangeDelegate(this, &iDialogMenu::onChange));
			removeWidget(_grid);
			iWidgetManager::getInstance().destroyWidget(_grid);
		}
	}

	void iDialogMenu::show(std::vector<iaString>& texts, iDialogMenuCloseDelegate closeDelegate)
	{
		deinitGUI();
		_selectBoxCloseEvent.append(closeDelegate);
		initGUI(texts);
	}

	void iDialogMenu::show(std::vector<iaString>& texts, std::vector<iaString>& pictures, iDialogMenuCloseDelegate closeDelegate)
	{
		deinitGUI();
		_selectBoxCloseEvent.append(closeDelegate);

		if (pictures.empty())
		{
			initGUI(texts);
		}
		else
		{
			initGUI(texts, pictures);
		}
	}

	void iDialogMenu::onMouseOffClick(iWidget* source)
	{
		close();
	}

	void iDialogMenu::onChange(iWidget* source)
	{
		con_assert(_grid == source, "unexpected source");

		if (_grid == source)
		{
			_returnValue = _grid->getSelectedRow();
		}

		close();
	}

	void iDialogMenu::close()
	{
		setActive(false);
		setVisible(false);
		iWidgetManager::resetModal();

		_selectBoxCloseEvent(_returnValue);
		_selectBoxCloseEvent.clear();
	}

	void iDialogMenu::setEntryHeight(int32 height)
	{
		_entryHeight = height;
	}

	int32 iDialogMenu::getEntryHeight() const
	{
		return _entryHeight;
	}
}