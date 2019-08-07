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
        
        initGUI(color, useAlpha);
    }

    void iDialogColorChooser::deinitGUI()
    {
        if (_grid != nullptr)
        {
            removeWidget(_grid);
			iWidgetManager::getInstance().destroyWidget(_grid);
			_grid = nullptr;
        }
    }

    void iDialogColorChooser::initGUI(const iaColor4f& color, bool useAlpha)
    {
        iWidgetManager::setModal(this);
        setActive();
        setVisible();
        setWidth(20);
        setHeight(20);

        _grid = new iWidgetGrid();
        _grid->appendRows(2);
        _grid->setHorizontalAlignment(iHorizontalAlignment::Center);
        _grid->setVerticalAlignment(iVerticalAlignment::Center);
        _grid->setCellSpacing(4);
        _grid->setBorder(4);

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

		deinitGUI();
    }
}