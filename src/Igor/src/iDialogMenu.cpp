// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iDialogMenu.h>

#include <iWidgetDialog.h>
#include <iWidgetManager.h>
#include <iWidgetLabel.h>
#include <iWidgetPicture.h>
#include <iWidgetGrid.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    void iDialogMenu::initGUI(vector<iaString>& texts)
    {
        iWidgetManager::setModal(getDialog());
        getDialog()->setActive();
        getDialog()->setVisible();
        getDialog()->setHeight(0);
        getDialog()->setAcceptOutOfBoundsClicks();

        getDialog()->registerOnMouseOffClickEvent(iMouseOffClickDelegate(this, &iDialogMenu::onMouseOffClick));

        _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
        _allWidgets.push_back(_grid);
        _grid->appendRows(texts.size() - 1);
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

    void iDialogMenu::initGUI(vector<iaString>& texts, vector<iaString>& pictures)
    {
        con_assert_sticky(texts.size() == pictures.size(), "invalid data");

        iWidgetManager::setModal(getDialog());
        getDialog()->setActive();
        getDialog()->setVisible();
        getDialog()->setHeight(0);
        getDialog()->setAcceptOutOfBoundsClicks();

        getDialog()->registerOnMouseOffClickEvent(iMouseOffClickDelegate(this, &iDialogMenu::onMouseOffClick));

        _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
        _allWidgets.push_back(_grid);
        _grid->appendCollumns(1);
        _grid->appendRows(texts.size() - 1);
        _grid->setHorrizontalAlignment(iHorrizontalAlignment::Left);
        _grid->setVerticalAlignment(iVerticalAlignment::Top);
        _grid->setSelectMode(iSelectionMode::Row);
        _grid->setCellSpacing(4);
        _grid->setBorder(4);
        _grid->registerOnChangeEvent(iChangeDelegate(this, &iDialogMenu::onChange));
        getDialog()->addWidget(_grid);

        for (int i = 0; i < texts.size(); ++i)
        {
            if (!pictures[i].isEmpty())
            {
                iWidgetPicture* picture = static_cast<iWidgetPicture*>(iWidgetManager::getInstance().createWidget(iWidgetType::Picture));
                _allWidgets.push_back(picture);
                picture->setTexture(pictures[i]);
                picture->setMaxSize(_entryHeight, _entryHeight);
                _grid->addWidget(picture, 0, i);
            }

            iWidgetLabel* label = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
            _allWidgets.push_back(label);
            label->setHorrizontalAlignment(iHorrizontalAlignment::Left);
            label->setText(texts[i]);
            _grid->addWidget(label, 1, i);
        }
    }

    iDialogMenu::~iDialogMenu()
    {
        deinitGUI();
    }

    void iDialogMenu::deinitGUI()
    {
        getDialog()->unregisterOnMouseOffClickEvent(iMouseOffClickDelegate(this, &iDialogMenu::onMouseOffClick));

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

    void iDialogMenu::show(vector<iaString>& texts, vector<iaString>& pictures, iDialogMenuCloseDelegate closeDelegate)
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
        getDialog()->setActive(false);
        getDialog()->setVisible(false);
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