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

    void iDialogColorChooser::open(iDialogClosedDelegate dialogCloseDelegate, const iaColor4f& color, bool useAlpha)
    {
        iDialog::open(dialogCloseDelegate);

        _oldColor = color;
        initGUI(color, useAlpha);
    }

    void iDialogColorChooser::initGUI(const iaColor4f& color, bool useAlpha)
    {
        iWidgetManager::setModal(this);
        setActive();
        setVisible();
        setWidth(20);
        setHeight(20);

        iWidgetGridPtr grid = new iWidgetGrid(this);
        grid->appendRows(2);
        grid->setHorizontalAlignment(iHorizontalAlignment::Center);
        grid->setVerticalAlignment(iVerticalAlignment::Center);
        grid->setCellSpacing(4);
        grid->setBorder(4);

        iWidgetLabel* headerLabel = new iWidgetLabel();
        headerLabel->setHorizontalAlignment(iHorizontalAlignment::Left);
        headerLabel->setText("Choose Color");

        _userControlColorChooser = new iUserControlColorChooser();
        _userControlColorChooser->setMode(useAlpha ? iColorChooserMode::RGBA : iColorChooserMode::RGB);
        _userControlColorChooser->setExpand();
        _userControlColorChooser->setHeadlineVisible(false);
        _userControlColorChooser->setColor(color);

        iWidgetGrid* buttonGrid = new iWidgetGrid();
        buttonGrid->appendCollumns(2);
        buttonGrid->setHorizontalAlignment(iHorizontalAlignment::Right);

        iWidgetButton* okButton = new iWidgetButton();
        okButton->setText("OK");
        okButton->setTooltip("Close the dialog and set new color.");
        okButton->registerOnClickEvent(iClickDelegate(this, &iDialogColorChooser::onOK));

        iWidgetButton* cancelButton = new iWidgetButton();
        cancelButton->setText("Cancel");
        cancelButton->setTooltip("Close the dialog without changes.");
        cancelButton->registerOnClickEvent(iClickDelegate(this, &iDialogColorChooser::onCancel));

        iWidgetButton* resetButton = new iWidgetButton();
        resetButton->setText("Reset");
        resetButton->setTooltip("Resets dialog to previous color.");
        resetButton->registerOnClickEvent(iClickDelegate(this, &iDialogColorChooser::onReset));

        grid->addWidget(headerLabel, 0, 0);
        grid->addWidget(_userControlColorChooser, 0, 1);
        grid->addWidget(buttonGrid, 0, 2);

        buttonGrid->addWidget(resetButton, 0, 0);
        buttonGrid->addWidget(cancelButton, 1, 0);
        buttonGrid->addWidget(okButton, 2, 0);
    }

    const iaColor4f& iDialogColorChooser::getColor() const
    {
        return _userControlColorChooser->getColor();
    }

    const iaColor4f& iDialogColorChooser::getResetColor() const
    {
        return _oldColor;
    }

    void iDialogColorChooser::onOK(iWidgetPtr source)
    {
        setReturnState(iDialogReturnState::Ok);
        close();
    }

    void iDialogColorChooser::onCancel(iWidgetPtr source)
    {
        setReturnState(iDialogReturnState::Cancel);
        close();
    }

    void iDialogColorChooser::onReset(iWidgetPtr source)
    {
        _userControlColorChooser->setColor(_oldColor);
    }

}