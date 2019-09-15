// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
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

	void iDialogDecisionBox::show(const iaString& message, iDecisionBoxCloseDelegate closeDelegate, std::initializer_list<iaString> radioButtonTexts, int32 preSelection)
	{
		_decisionBoxCloseEvent.append(closeDelegate);

		initGUI(message, radioButtonTexts, preSelection);
	}

	void iDialogDecisionBox::initGUI(const iaString& message, std::initializer_list<iaString> radioButtonTexts, int32 preSelection)
	{
		iWidgetManager::setModal(this);
		setActive();
		setVisible();
		setWidth(20);
		setHeight(20);

		iWidgetGridPtr grid = new iWidgetGrid();
		grid->appendRows(4);
		grid->setHorizontalAlignment(iHorizontalAlignment::Center);
		grid->setVerticalAlignment(iVerticalAlignment::Center);
		grid->setCellSpacing(4);
		grid->setBorder(4);

		iWidgetLabelPtr messageLabel = new iWidgetLabel();
		messageLabel->setText(message);
		messageLabel->setMaxTextWidth(280);

		iWidgetSpacerPtr spacerLineTop = new iWidgetSpacer();
		spacerLineTop->setWidth(280);
		spacerLineTop->setHeight(1);
		spacerLineTop->setVisible(true);

		iWidgetSpacerPtr spacerLineBottom = new iWidgetSpacer();
		spacerLineBottom->setWidth(280);
		spacerLineBottom->setHeight(1);
		spacerLineBottom->setVisible(true);

		iWidgetGridPtr buttonGrid = new iWidgetGrid();
		buttonGrid->appendCollumns(3);
		buttonGrid->setHorizontalAlignment(iHorizontalAlignment::Right);
		buttonGrid->setVerticalAlignment(iVerticalAlignment::Bottom);
		buttonGrid->setCellSpacing(4);

		iWidgetButtonPtr okButton = new iWidgetButton();
		okButton->setText("OK");
		okButton->registerOnClickEvent(iClickDelegate(this, &iDialogDecisionBox::onOK));

		iWidgetButtonPtr cancelButton = new iWidgetButton();
		cancelButton->setText("Cancel");
		cancelButton->registerOnClickEvent(iClickDelegate(this, &iDialogDecisionBox::onCancel));

		iWidgetGridPtr radioGrid = new iWidgetGrid();
		radioGrid->appendRows(static_cast<uint32>(radioButtonTexts.size()) - 1);
		radioGrid->setHorizontalAlignment(iHorizontalAlignment::Left);
		radioGrid->setVerticalAlignment(iVerticalAlignment::Top);
		radioGrid->setCellSpacing(4);

		iWidgetCheckBox::beginRadioButtonGroup();
		int i = 0;
		auto iter = radioButtonTexts.begin();
		while (iter != radioButtonTexts.end())
		{
			iWidgetCheckBox* checkBox = new iWidgetCheckBox();
			_radioButtons.push_back(checkBox);
			checkBox->setHorizontalAlignment(iHorizontalAlignment::Left);
			checkBox->setText((*iter));
			radioGrid->addWidget(checkBox, 0, i);

			if (preSelection != -1 && preSelection == i)
			{
				checkBox->setChecked();
			}

			i++;
			iter++;
		}
		iWidgetCheckBox::endRadioButtonGroup();

		addWidget(grid);

		grid->addWidget(messageLabel, 0, 0);
		grid->addWidget(spacerLineTop, 0, 1);
		grid->addWidget(radioGrid, 0, 2);
		grid->addWidget(spacerLineBottom, 0, 3);
		grid->addWidget(buttonGrid, 0, 4);

		buttonGrid->addWidget(okButton, 0, 0);
		buttonGrid->addWidget(cancelButton, 1, 0);
	}

	void iDialogDecisionBox::onOK(iWidget* source)
	{		
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

		close();
	}

	void iDialogDecisionBox::onCancel(iWidget* source)
	{
		_decisionBoxCloseEvent(false, -1);
		_decisionBoxCloseEvent.clear();

		close();
	}

	void iDialogDecisionBox::close()
	{
		setActive(false);
		setVisible(false);
		iWidgetManager::resetModal();
	}
}