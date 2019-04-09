// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iDialogColorChooser.h>

#include <iDialog.h>
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

    iDialog* iDialogColorChooser::createInstance()
    {
        return new iDialogColorChooser();
    }

    void iDialogColorChooser::show(iColorChooserCloseDelegate closeDelegate, const iaColor4f& color, bool useAlpha)
    {
        _oldColor = color;
        _closeEvent.append(closeDelegate);

        deinitGUI();
        initGUI(color, useAlpha);
    }

    void iDialogColorChooser::deinitGUI()
    {
        if (_grid != nullptr &&
            _grid->hasParent())
        {
            removeWidget(_grid);
        }

        if (_userControlColorChooser != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_userControlColorChooser);
            _userControlColorChooser = nullptr;
        }

        for (auto widget : _allWidgets)
        {
            iWidgetManager::getInstance().destroyWidget(widget);
        }
        _allWidgets.clear();
    }

    void iDialogColorChooser::initGUI(const iaColor4f& color, bool useAlpha)
    {
        iWidgetManager::setModal(this);
        setActive();
        setVisible();
        setWidth(20);
        setHeight(20);

        _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
        _allWidgets.push_back(_grid);
        _grid->appendRows(2);
        _grid->setHorizontalAlignment(iHorizontalAlignment::Center);
        _grid->setVerticalAlignment(iVerticalAlignment::Center);
        _grid->setCellSpacing(4);
        _grid->setBorder(4);

        iWidgetLabel* headerLabel = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
        _allWidgets.push_back(headerLabel);
        headerLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
        headerLabel->setText("Choose Color");

        _userControlColorChooser = static_cast<iUserControlColorChooser*>(iWidgetManager::getInstance().createWidget("UserControlColorChooser"));
        _userControlColorChooser->setMode(useAlpha ? iColorChooserMode::RGBA : iColorChooserMode::RGB);
        _userControlColorChooser->setExpand();
        _userControlColorChooser->setHeadlineVisible(false);
        _userControlColorChooser->setColor(color);

        iWidgetGrid* buttonGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
        _allWidgets.push_back(buttonGrid);
        buttonGrid->appendCollumns(2);
        buttonGrid->setHorizontalAlignment(iHorizontalAlignment::Right);

        iWidgetButton* okButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
        _allWidgets.push_back(okButton);
        okButton->setText("OK");
        okButton->registerOnClickEvent(iClickDelegate(this, &iDialogColorChooser::onOK));

        iWidgetButton* cancelButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
        _allWidgets.push_back(cancelButton);
        cancelButton->setText("Cancel");
        cancelButton->registerOnClickEvent(iClickDelegate(this, &iDialogColorChooser::onCancel));

        iWidgetButton* resetButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
        _allWidgets.push_back(resetButton);
        resetButton->setText("Reset");
        resetButton->registerOnClickEvent(iClickDelegate(this, &iDialogColorChooser::onReset));

        addWidget(_grid);

        _grid->addWidget(headerLabel, 0, 0);
        _grid->addWidget(_userControlColorChooser, 0, 1);
        _grid->addWidget(buttonGrid, 0, 2);

        buttonGrid->addWidget(resetButton, 0, 0);
        buttonGrid->addWidget(cancelButton, 1, 0);
        buttonGrid->addWidget(okButton, 2, 0);
    }

    void iDialogColorChooser::onOK(iWidget* source)
    {
        close();
        
        _closeEvent(true, _userControlColorChooser->getColor());
        _closeEvent.clear();
    }

    void iDialogColorChooser::onCancel(iWidget* source)
    {
        close();

        _closeEvent(false, _userControlColorChooser->getColor());
        _closeEvent.clear();
    }

    void iDialogColorChooser::onReset(iWidget* source)
    {
        _userControlColorChooser->setColor(_oldColor);
    }

    void iDialogColorChooser::close()
    {
        setActive(false);
        setVisible(false);
        iWidgetManager::resetModal();
    }
}