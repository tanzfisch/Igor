// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iDialogDecisionBox.h>

#include <iDialog.h>
#include <iWidgetManager.h>
#include <iWidgetLabel.h>
#include <iWidgetButton.h>
#include <iWidgetGrid.h>
#include <iWidgetSpacer.h>
#include <iWidgetCheckBox.h>
#include <iWidgetManager.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iDialogDecisionBox::~iDialogDecisionBox()
    {
        deinitGUI();
    }

    iDialog* iDialogDecisionBox::createInstance()
    {
        return new iDialogDecisionBox();
    }

    void iDialogDecisionBox::deinitGUI()
    {
        if (_okButton != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_okButton);
        }

        if (_cancelButton != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_cancelButton);
        }

        if (_grid != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_grid);
        }

        if (_buttonGrid != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_buttonGrid);
        }

        if (_messageLabel != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_messageLabel);
        }

        if (_spacerLittle != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_spacerLittle);
        }

        if (_spacerLineTop != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_spacerLineTop);
        }

        if (_spacerLineBottom != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_spacerLineBottom);
        }

        auto iter = _radioButtons.begin();
        while (iter != _radioButtons.end())
        {
            iWidgetManager::getInstance().destroyWidget((*iter));
        }

        _radioButtons.clear();

        if (_radioGrid != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_radioGrid);
        }
    }

    void iDialogDecisionBox::show(const iaString& message, iDecisionBoxCloseDelegate closeDelegate, std::initializer_list<iaString> radioButtonTexts, int32 preSelection)
    {
        _decisionBoxCloseEvent.append(closeDelegate);

        iWidgetManager::setModal(this);
        setActive();
        setVisible();
        setWidth(20);
        setHeight(20);

        _grid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
        _grid->appendRows(4);
        _grid->setHorizontalAlignment(iHorizontalAlignment::Center);
        _grid->setVerticalAlignment(iVerticalAlignment::Center);
        _grid->setCellSpacing(4);
        _grid->setBorder(4);

        _messageLabel = static_cast<iWidgetLabel*>(iWidgetManager::getInstance().createWidget("Label"));
        _messageLabel->setText(message);
        _messageLabel->setMaxTextWidth(280);
        
        _spacerLineTop = static_cast<iWidgetSpacer*>(iWidgetManager::getInstance().createWidget("Spacer"));
        _spacerLineTop->setWidth(280);
        _spacerLineTop->setHeight(1);
        _spacerLineTop->setVisible(true);

        _spacerLineBottom = static_cast<iWidgetSpacer*>(iWidgetManager::getInstance().createWidget("Spacer"));
        _spacerLineBottom->setWidth(280);
        _spacerLineBottom->setHeight(1);
        _spacerLineBottom->setVisible(true);

        _buttonGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
        _buttonGrid->appendCollumns(3);
        _buttonGrid->setHorizontalAlignment(iHorizontalAlignment::Right);
        _buttonGrid->setVerticalAlignment(iVerticalAlignment::Bottom);
        _buttonGrid->setCellSpacing(4);
        
        _okButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
        _okButton->setText("OK");
        _okButton->registerOnClickEvent(iClickDelegate(this, &iDialogDecisionBox::onOK));

        _cancelButton = static_cast<iWidgetButton*>(iWidgetManager::getInstance().createWidget("Button"));
        _cancelButton->setText("Cancel");
        _cancelButton->registerOnClickEvent(iClickDelegate(this, &iDialogDecisionBox::onCancel));

        _radioGrid = static_cast<iWidgetGrid*>(iWidgetManager::getInstance().createWidget("Grid"));
        _radioGrid->appendRows(static_cast<uint32>(radioButtonTexts.size()) - 1);
        _radioGrid->setHorizontalAlignment(iHorizontalAlignment::Left);
        _radioGrid->setVerticalAlignment(iVerticalAlignment::Top);
        _radioGrid->setCellSpacing(4);

        iWidgetCheckBox::beginRadioButtonGroup();
        int i = 0;
        auto iter = radioButtonTexts.begin();
        while (iter != radioButtonTexts.end())
        {
            iWidgetCheckBox* checkBox = static_cast<iWidgetCheckBox*>(iWidgetManager::getInstance().createWidget("CheckBox"));
            _radioButtons.push_back(checkBox);
            checkBox->setHorizontalAlignment(iHorizontalAlignment::Left);
            checkBox->setText((*iter));
            _radioGrid->addWidget(checkBox, 0, i);

            if (preSelection != -1 && preSelection == i)
            {
                checkBox->setChecked();
            }

            i++;
            iter++;
        }
        iWidgetCheckBox::endRadioButtonGroup();

        addWidget(_grid);
        _grid->addWidget(_messageLabel, 0, 0);
        _grid->addWidget(_spacerLineTop, 0, 1);
        _grid->addWidget(_radioGrid, 0, 2);
        _grid->addWidget(_spacerLineBottom, 0, 3);
        _grid->addWidget(_buttonGrid, 0, 4);

        _buttonGrid->addWidget(_okButton, 0, 0);
        _buttonGrid->addWidget(_cancelButton, 1, 0);
    }

    void iDialogDecisionBox::onOK(iWidget* source)
    {
        close();

        int i = 0;
        bool selected = false;
        auto iter = _radioButtons.begin();
        while (iter != _radioButtons.end())
        {
            if ((*iter)->isChecked())
            {
                _decisionBoxCloseEvent(true, i);
                selected = true;
                break;
            }
            iter++;
            i++;
        }

        if (!selected)
        {
            _decisionBoxCloseEvent(false, -1);
        }
        _decisionBoxCloseEvent.clear();
    }

    void iDialogDecisionBox::onCancel(iWidget* source)
    {
        close();

        _decisionBoxCloseEvent(false, -1);
        _decisionBoxCloseEvent.clear();
    }

    void iDialogDecisionBox::close()
    {
        setActive(false);
        setVisible(false);
        iWidgetManager::resetModal();
    }
}