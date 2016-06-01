// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iDialogMenu.h>

#include <iWidgetDialog.h>
#include <iWidgetManager.h>
#include <iWidgetLabel.h>
#include <iWidgetGrid.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iDialogMenu::~iDialogMenu()
    {
        deinitGUI();
    }

    void iDialogMenu::deinitGUI()
    {
		getDialog()->unregisterOnMouseOffEvent(iMouseOffDelegate(this, &iDialogMenu::onMouseOff));

        if (_grid != nullptr)
        {
            _grid->unregisterOnChangeEvent(iChangeDelegate(this, &iDialogMenu::onChange));

            if (_grid->hasParent())
            {
                getDialog()->removeWidget(_grid);
            }
        }

        for (auto widget : _allWidgets)
        {
            iWidgetManager::getInstance().destroyWidget(widget);
        }

        _allWidgets.clear();
    }

    void iDialogMenu::show(vector<iaString>& texts, iDialogMenuCloseDelegate closeDelegate)
    {
        deinitGUI();
        _selectBoxCloseEvent.append(closeDelegate);
        initGUI(texts);
    }

    void iDialogMenu::initGUI(vector<iaString>& texts)
    {
        getDialog()->setModal();
        getDialog()->setActive();
        getDialog()->setVisible();
        getDialog()->setHeight(0);

		getDialog()->registerOnMouseOffEvent(iMouseOffDelegate(this, &iDialogMenu::onMouseOff));

        _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
        _allWidgets.push_back(_grid);
        _grid->appendRows(texts.size() -1);
        _grid->setHorrizontalAlignment(iHorrizontalAlignment::Left);
        _grid->setVerticalAlignment(iVerticalAlignment::Top);
        _grid->setSelectMode(iSelectionMode::Row);
        _grid->setCellSpacing(4);
        _grid->setBorder(4);
        _grid->registerOnChangeEvent(iChangeDelegate(this, &iDialogMenu::onChange));
        getDialog()->addWidget(_grid);

        for (int i = 0; i < texts.size(); ++i)
        {
            iWidgetLabel* label = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
            label->setHorrizontalAlignment(iHorrizontalAlignment::Left);
            _allWidgets.push_back(label);
            label->setText(texts[i]);
            _grid->addWidget(label, 0, i);
        }
    }

	void iDialogMenu::onMouseOff(iWidget* source)
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
        getDialog()->setActive(false);
        getDialog()->setVisible(false);
        getDialog()->resetModal();

        _selectBoxCloseEvent(_returnValue);
        _selectBoxCloseEvent.clear();
    }
}