// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iDialogColorChooser.h>

#include <iWidgetDialog.h>
#include <iWidgetManager.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetGrid.h>
#include <iWidgetSpacer.h>
#include <iWidgetCheckBox.h>
#include <iWidgetManager.h>
#include <iUserControlColorChooser.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iDialogColorChooser::~iDialogColorChooser()
    {
        deinitGUI();
    }

    void iDialogColorChooser::deinitGUI()
    {
        for (auto widget : _allWidgets)
        {
            iWidgetManager::getInstance().destroyWidget(widget);
        }
        _allWidgets.clear();
    }

    void iDialogColorChooser::show(iColorChooserCloseDelegate closeDelegate, iaColor4f color, bool useAlpha)
    {
        _color = color;
        _oldColor = color;

        _closeEvent.append(closeDelegate);

        iWidgetManager::setModal(getDialog());
        getDialog()->setActive();
        getDialog()->setVisible();
        getDialog()->setWidth(200);
        getDialog()->setHeight(200);

        _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
        _allWidgets.push_back(_grid);
        _grid->appendRows(1);
        _grid->setHorizontalAlignment(iHorizontalAlignment::Center);
        _grid->setVerticalAlignment(iVerticalAlignment::Center);
        _grid->setCellSpacing(4);
        _grid->setBorder(4);

        _userControlColorChooser = new iUserControlColorChooser(useAlpha ? iColorChooserMode::RGBA : iColorChooserMode::RGB);
        _userControlColorChooser->setColor(_color);

        _okButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
        _cancelButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
        _resetButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));

        _grid->addWidget(_userControlColorChooser->getWidget(), 0, 0);
    }

    void iDialogColorChooser::onOK(iWidget* source)
    {
        close();

        // TODO _closeEvent
        _closeEvent.clear();
    }

    void iDialogColorChooser::onCancel(iWidget* source)
    {
        close();

        // TODO _closeEvent
        _closeEvent.clear();
    }

    void iDialogColorChooser::close()
    {
        getDialog()->setActive(false);
        getDialog()->setVisible(false);
        iWidgetManager::resetModal();
    }
}