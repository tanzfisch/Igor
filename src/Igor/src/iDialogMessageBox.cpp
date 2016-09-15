// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iDialogMessageBox.h>

#include <iWidgetDialog.h>
#include <iWidgetManager.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetGrid.h>
#include <iWidgetSpacer.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iDialogMessageBox::~iDialogMessageBox()
    {
        deinitGUI();
    }

    void iDialogMessageBox::deinitGUI()
    {
        if (_grid != nullptr && 
            _grid->hasParent())
        {
            getDialog()->removeWidget(_grid);
        }

        for (auto widget : _allWidgets)
        {
            iWidgetManager::getInstance().destroyWidget(widget);
        }

        _allWidgets.clear();
    }

    void iDialogMessageBox::show(iaString message, iDialogMessageBoxCloseDelegate closeDelegate, iMessageBoxButtons buttons)
    {
        _messageBoxCloseEvent.append(closeDelegate);
        initGUI(message, buttons);
    }

    void iDialogMessageBox::show(iaString message, iMessageBoxButtons buttons)
    {
        deinitGUI();
        initGUI(message, buttons);
    }

    void iDialogMessageBox::initGUI(iaString message, iMessageBoxButtons buttons)
    {
        iWidgetManager::setModal(getDialog());
        getDialog()->setActive();
        getDialog()->setVisible();
        getDialog()->setWidth(20);
        getDialog()->setHeight(20);

        _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
        _allWidgets.push_back(_grid);
        _grid->appendRows(2);
        _grid->setHorrizontalAlignment(iHorrizontalAlignment::Center);
        _grid->setVerticalAlignment(iVerticalAlignment::Center);
        _grid->setCellSpacing(4);
        _grid->setBorder(4);
        getDialog()->addWidget(_grid);

        _messageLabel = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget(iWidgetType::Label));
        _allWidgets.push_back(_messageLabel);
        _messageLabel->setText(message);
        _messageLabel->setMaxTextWidth(280);
        _grid->addWidget(_messageLabel, 0, 0);

        _spacerLine = static_cast<iWidgetSpacer*>(iWidgetManager::getInstance().createWidget(iWidgetType::Spacer));
        _allWidgets.push_back(_spacerLine);
        _spacerLine->setWidth(280);
        _spacerLine->setHeight(1);
        _spacerLine->setVisible(true);
        _grid->addWidget(_spacerLine, 0, 1);

        _buttonGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget(iWidgetType::Grid));
        _allWidgets.push_back(_buttonGrid);
        _buttonGrid->appendCollumns(3);
        _buttonGrid->setHorrizontalAlignment(iHorrizontalAlignment::Right);
        _buttonGrid->setVerticalAlignment(iVerticalAlignment::Bottom);
        _buttonGrid->setCellSpacing(4);
        _grid->addWidget(_buttonGrid, 0, 2);
        
        int i = 3;

        if (buttons == iMessageBoxButtons::Ok || buttons == iMessageBoxButtons::CancelOk)
        {
            _okButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
            _allWidgets.push_back(_okButton);
            _okButton->setText("OK");
            _okButton->registerOnClickEvent(iClickDelegate(this, &iDialogMessageBox::onOK));
            _buttonGrid->addWidget(_okButton, i--, 0);
        }

        if (buttons == iMessageBoxButtons::CancelOk || buttons == iMessageBoxButtons::YesNoCancel)
        {
            _cancelButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
            _allWidgets.push_back(_cancelButton);
            _cancelButton->setText("Cancel");
            _cancelButton->registerOnClickEvent(iClickDelegate(this, &iDialogMessageBox::onCancel));
            _buttonGrid->addWidget(_cancelButton, i--, 0);

            _spacerLittle = static_cast<iWidgetSpacer*>(iWidgetManager::getInstance().createWidget(iWidgetType::Spacer));
            _allWidgets.push_back(_spacerLittle);
            _spacerLittle->setWidth(4);
            _spacerLittle->setHeight(1);
            _spacerLittle->setVisible(false);
            _buttonGrid->addWidget(_spacerLittle, i--, 0);
        }

        if (buttons == iMessageBoxButtons::YesNoCancel || buttons == iMessageBoxButtons::YesNo)
        {
            _noButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
            _allWidgets.push_back(_noButton);
            _noButton->setText("No");
            _noButton->registerOnClickEvent(iClickDelegate(this, &iDialogMessageBox::onNo));
            _buttonGrid->addWidget(_noButton, i--, 0);
        }

        if (buttons == iMessageBoxButtons::YesNoCancel || buttons == iMessageBoxButtons::YesNo)
        {
            _yesButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget(iWidgetType::Button));
            _allWidgets.push_back(_yesButton);
            _yesButton->setText("Yes");
            _yesButton->registerOnClickEvent(iClickDelegate(this, &iDialogMessageBox::onYes));
            _buttonGrid->addWidget(_yesButton, i--, 0);
        }
    }

    void iDialogMessageBox::onOK(iWidget* source)
    {
        _messageBoxReturnValue = iMessageBoxReturnValue::Ok;
        close();
    }

    void iDialogMessageBox::onCancel(iWidget* source)
    {
        _messageBoxReturnValue = iMessageBoxReturnValue::Cancel;
        close();
    }

    void iDialogMessageBox::onYes(iWidget* source)
    {
        _messageBoxReturnValue = iMessageBoxReturnValue::Yes;
        close();
    }

    void iDialogMessageBox::onNo(iWidget* source)
    {
        _messageBoxReturnValue = iMessageBoxReturnValue::No;
        close();
    }

    void iDialogMessageBox::close()
    {
        getDialog()->setActive(false);
        getDialog()->setVisible(false);
        iWidgetManager::resetModal();

        _messageBoxCloseEvent(_messageBoxReturnValue);
        _messageBoxCloseEvent.clear();
    }
}