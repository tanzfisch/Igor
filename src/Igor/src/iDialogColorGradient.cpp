// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iDialogColorGradient.h>

#include <iDialog.h>
#include <iWidgetManager.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetGrid.h>
#include <iWidgetSpacer.h>
#include <iWidgetCheckBox.h>
#include <iWidgetManager.h>
#include <iUserControlColorChooser.h>
#include <iWidgetColorGradient.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iDialogColorGradient::~iDialogColorGradient()
    {
        deinitGUI();
    }

    iDialog* iDialogColorGradient::createInstance()
    {
        return new iDialogColorGradient();
    }

    void iDialogColorGradient::show(iColorGradientCloseDelegate closeDelegate, const iGradientColor4f& gradient, bool useAlpha)
    {
        _oldGradient = gradient;
        _closeEvent.append(closeDelegate);

        deinitGUI();
        initGUI(gradient, useAlpha);
    }

    void iDialogColorGradient::deinitGUI()
    {
        if (_grid != nullptr &&
            _grid->hasParent())
        {
            removeWidget(_grid);
        }

        for (auto widget : _allWidgets)
        {
            iWidgetManager::getInstance().destroyWidget(widget);
        }
        _allWidgets.clear();
    }

    void iDialogColorGradient::initGUI(const iGradientColor4f& gradient, bool useAlpha)
    {
        iWidgetManager::setModal(this);
        setActive();
        setVisible();
        setWidth(300);
        setHeight(100);

        _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
        _allWidgets.push_back(_grid);
        _grid->appendRows(3);
        _grid->setStrechRow(2);
        _grid->setHorizontalAlignment(iHorizontalAlignment::Strech);
        _grid->setVerticalAlignment(iVerticalAlignment::Strech);
        _grid->setCellSpacing(4);
        _grid->setBorder(4);

        iWidgetLabel* headerLabel = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
        _allWidgets.push_back(headerLabel);
        headerLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
        headerLabel->setText("Edit Gradient");

        _gradientWidget = static_cast<iWidgetColorGradient*>(iWidgetManager::getInstance().createWidget("ColorGradient"));
        _allWidgets.push_back(_gradientWidget);
        _gradientWidget->setGradient(gradient);
        _gradientWidget->setHorizontalAlignment(iHorizontalAlignment::Strech);
        _gradientWidget->setVerticalAlignment(iVerticalAlignment::Strech);
        _gradientWidget->setUseAlpha(useAlpha);
        _gradientWidget->setInteractive();
        _gradientWidget->setHeight(40);

        iWidgetGrid* controlGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
        _allWidgets.push_back(controlGrid);
        controlGrid->appendCollumns(4);
        controlGrid->setHorizontalAlignment(iHorizontalAlignment::Left);

        iWidgetGrid* buttonGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
        _allWidgets.push_back(buttonGrid);
        buttonGrid->appendCollumns(2);
        buttonGrid->setHorizontalAlignment(iHorizontalAlignment::Right);

        iWidgetButton* okButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
        _allWidgets.push_back(okButton);
        okButton->setText("OK");
        okButton->registerOnClickEvent(iClickDelegate(this, &iDialogColorGradient::onOK));

        iWidgetButton* cancelButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
        _allWidgets.push_back(cancelButton);
        cancelButton->setText("Cancel");
        cancelButton->registerOnClickEvent(iClickDelegate(this, &iDialogColorGradient::onCancel));

        iWidgetButton* resetButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
        _allWidgets.push_back(resetButton);
        resetButton->setText("Reset");
        resetButton->registerOnClickEvent(iClickDelegate(this, &iDialogColorGradient::onReset));

        addWidget(_grid);

        _grid->addWidget(headerLabel, 0, 0);
        _grid->addWidget(_gradientWidget, 0, 1);
        _grid->addWidget(controlGrid, 0, 2);
        _grid->addWidget(buttonGrid, 0, 3);

        buttonGrid->addWidget(resetButton, 0, 0);
        buttonGrid->addWidget(cancelButton, 1, 0);
        buttonGrid->addWidget(okButton, 2, 0);
    }

    void iDialogColorGradient::onOK(iWidget* source)
    {
        close();
        
        // TODO _closeEvent(true, 
        _closeEvent.clear();
    }

    void iDialogColorGradient::onCancel(iWidget* source)
    {
        close();

        // TODO _closeEvent(false, 
        _closeEvent.clear();
    }

    void iDialogColorGradient::onReset(iWidget* source)
    {
        // TODO _oldGradient
    }

    void iDialogColorGradient::close()
    {
        setActive(false);
        setVisible(false);
        iWidgetManager::resetModal();
    }
}