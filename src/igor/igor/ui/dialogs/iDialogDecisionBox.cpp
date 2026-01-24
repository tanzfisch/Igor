// Igor game engine
// (c) Copyright 2012-2026 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/dialogs/iDialogDecisionBox.h>

#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/ui/layouts/iWidgetGridLayout.h>
#include <igor/ui/layouts/iWidgetBoxLayout.h>
#include <igor/ui/widgets/iWidgetSpacer.h>
#include <igor/ui/widgets/iWidgetCheckBox.h>
#include <igor/ui/widgets/iWidgetSelectBox.h>
#include <igor/ui/iWidgetManager.h>

namespace igor
{

	iDialogDecisionBox::iDialogDecisionBox(const iWidgetPtr parent)
		: iDialog(iWidgetType::iDialogDecisionBox, parent)
	{
	}

	void iDialogDecisionBox::open(iDialogCloseDelegate dialogCloseDelegate, const iaString &title, const iaString &message, const std::vector<iaString> &selectionTexts, int32 preSelection, iDialogDecisionBoxStyle style)
	{
		iDialog::open(dialogCloseDelegate, true);

		onInitUI(title, message, selectionTexts, preSelection, style);
	}

	int32 iDialogDecisionBox::getSelection() const
	{
		if(_selectBox != nullptr)
		{
			return _selectBox->getSelectedIndex();
		}

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

	void iDialogDecisionBox::onInitUI(const iaString &title, const iaString &message, const std::vector<iaString> &selectionTexts, int32 preSelection, iDialogDecisionBoxStyle style)
	{
		con_assert_sticky(preSelection == -1 || preSelection < selectionTexts.size(), "index out range");

		setTitle(title);
		setVerticalAlignment(iVerticalAlignment::Center);
		setHorizontalAlignment(iHorizontalAlignment::Center);
		setResizeable(false);

		setEnabled();
		setVisible();
		setMinWidth(20);
		setMinHeight(20);

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
		okButton->getClickEvent().add(iClickDelegate(this, &iDialogDecisionBox::onOK));

		iWidgetButtonPtr cancelButton = new iWidgetButton();
		cancelButton->setText("Cancel");
		cancelButton->getClickEvent().add(iClickDelegate(this, &iDialogDecisionBox::onCancel));

		iWidgetBoxLayoutPtr selectionLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical);
		selectionLayout->setHorizontalAlignment(iHorizontalAlignment::Left);
		selectionLayout->setVerticalAlignment(iVerticalAlignment::Top);

		if (selectionTexts.size() < 6)
		{
			iWidgetCheckBox::beginRadioButtonGroup();
			for (const auto &text : selectionTexts)
			{
				iWidgetCheckBox *checkBox = new iWidgetCheckBox(selectionLayout);
				_radioButtons.push_back(checkBox);
				checkBox->setHorizontalAlignment(iHorizontalAlignment::Left);
				checkBox->setText(text);

				if (preSelection == _radioButtons.size() - 1)
				{
					checkBox->setChecked();
				}
			}
			iWidgetCheckBox::endRadioButtonGroup();
		}
		else
		{
			_selectBox = new iWidgetSelectBox(selectionLayout);
			for (const auto &text : selectionTexts)
			{
				_selectBox->addItem(text);
			}			
			_selectBox->setSelection(preSelection);
		}

		addWidget(grid);

		grid->addWidget(messageLabel, 0, 0);
		grid->addWidget(new iWidgetSpacer(280, 1), 0, 1);
		grid->addWidget(selectionLayout, 0, 2);
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