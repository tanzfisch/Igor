// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/dialogs/iDialogDecisionBox.h>

#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/ui/widgets/iWidgetGridLayout.h>
#include <igor/ui/widgets/iWidgetSpacer.h>
#include <igor/ui/widgets/iWidgetCheckBox.h>
#include <igor/ui/iWidgetManager.h>

namespace igor
{

	iDialogDecisionBox::iDialogDecisionBox(const iWidgetPtr parent)
		: iDialog(iWidgetType::iDialogDecisionBox, parent)
	{
	}

	void iDialogDecisionBox::open(iDialogCloseDelegate dialogCloseDelegate, const iaString &message, std::initializer_list<iaString> radioButtonTexts, int32 preSelection)
	{
		iDialog::open(dialogCloseDelegate);

		initGUI(message, radioButtonTexts, preSelection);
	}

	int32 iDialogDecisionBox::getSelection() const
	{
		int32 i = 0;
		for (auto radioButton : _radioButtons)
		{
			if (radioButton->isChecked())
			{
				return i;
			}
			i++;
		}

		return -1;
	}

	void iDialogDecisionBox::initGUI(const iaString &message, std::initializer_list<iaString> radioButtonTexts, int32 preSelection)
	{
		iWidgetManager::getInstance().setModal(this);
		setEnabled();
		setVisible();
		setWidth(20);
		setHeight(20);

		iWidgetGridLayoutPtr grid = new iWidgetGridLayout();
		grid->appendRows(4);
		grid->setHorizontalAlignment(iHorizontalAlignment::Center);
		grid->setVerticalAlignment(iVerticalAlignment::Center);
		grid->setCellSpacing(4);
		grid->setBorder(4);

		iWidgetLabelPtr messageLabel = new iWidgetLabel();
		messageLabel->setText(message);
		messageLabel->setMaxTextWidth(280);

		iWidgetGridLayoutPtr buttonGrid = new iWidgetGridLayout();
		buttonGrid->appendColumns(3);
		buttonGrid->setHorizontalAlignment(iHorizontalAlignment::Right);
		buttonGrid->setVerticalAlignment(iVerticalAlignment::Bottom);
		buttonGrid->setCellSpacing(4);

		iWidgetButtonPtr okButton = new iWidgetButton();
		okButton->setText("OK");
		okButton->registerOnClickEvent(iClickDelegate(this, &iDialogDecisionBox::onOK));

		iWidgetButtonPtr cancelButton = new iWidgetButton();
		cancelButton->setText("Cancel");
		cancelButton->registerOnClickEvent(iClickDelegate(this, &iDialogDecisionBox::onCancel));

		iWidgetGridLayoutPtr radioGrid = new iWidgetGridLayout();
		radioGrid->appendRows(static_cast<uint32>(radioButtonTexts.size()) - 1);
		radioGrid->setHorizontalAlignment(iHorizontalAlignment::Left);
		radioGrid->setVerticalAlignment(iVerticalAlignment::Top);
		radioGrid->setCellSpacing(4);

		iWidgetCheckBox::beginRadioButtonGroup();
		int i = 0;
		auto iter = radioButtonTexts.begin();
		while (iter != radioButtonTexts.end())
		{
			iWidgetCheckBox *checkBox = new iWidgetCheckBox();
			_radioButtons.push_back(checkBox);
			checkBox->setHorizontalAlignment(iHorizontalAlignment::Left);
			checkBox->setText((*iter));
			radioGrid->addWidget(checkBox, 0, i);

			if (preSelection == i)
			{
				checkBox->setChecked();
			}

			i++;
			iter++;
		}
		iWidgetCheckBox::endRadioButtonGroup();

		addWidget(grid);

		grid->addWidget(messageLabel, 0, 0);
		grid->addWidget(new iWidgetSpacer(280, 1), 0, 1);
		grid->addWidget(radioGrid, 0, 2);
		grid->addWidget(new iWidgetSpacer(280, 1), 0, 3);
		grid->addWidget(buttonGrid, 0, 4);

		buttonGrid->addWidget(okButton, 0, 0);
		buttonGrid->addWidget(cancelButton, 1, 0);
	}

	void iDialogDecisionBox::onOK(const iWidgetPtr source)
	{
		setReturnState(iDialogReturnState::Ok);
		close();
	}

	void iDialogDecisionBox::onCancel(const iWidgetPtr source)
	{
		setReturnState(iDialogReturnState::Cancel);
		close();
	}
} // namespace igor