// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/dialogs/iDialogMessageBox.h>

#include <igor/ui/dialogs/iDialog.h>
#include <igor/ui/iWidgetManager.h>
#include <igor/ui/widgets/iWidgetLabel.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/ui/widgets/iWidgetGrid.h>
#include <igor/ui/widgets/iWidgetSpacer.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{
	iDialogMessageBox::iDialogMessageBox(const iWidgetPtr parent)
		: iDialog(iWidgetType::iDialogMessageBox, parent)
	{
	}

	void iDialogMessageBox::open(iaString message, iMessageBoxButtons buttons)
	{
		iWidgetManager::getInstance().setModal(this);
		setEnabled();
		setVisible();
		setWidth(20);
		setHeight(20);

		iWidgetGridPtr grid = new iWidgetGrid(this);
		grid->appendRows(2);
		grid->setHorizontalAlignment(iHorizontalAlignment::Center);
		grid->setVerticalAlignment(iVerticalAlignment::Center);
		grid->setCellSpacing(4);
		grid->setBorder(4);

		iWidgetLabelPtr messageLabel = new iWidgetLabel();
		messageLabel->setText(message);
		messageLabel->setMaxTextWidth(280);
		grid->addWidget(messageLabel, 0, 0);

		grid->addWidget(new iWidgetSpacer(280, 1, true), 0, 1);

		iWidgetGridPtr buttonGrid = new iWidgetGrid();
		buttonGrid->appendColumns(3);
		buttonGrid->setHorizontalAlignment(iHorizontalAlignment::Right);
		buttonGrid->setVerticalAlignment(iVerticalAlignment::Bottom);
		buttonGrid->setCellSpacing(4);
		grid->addWidget(buttonGrid, 0, 2);

		int i = 3;

		if (buttons == iMessageBoxButtons::Ok || buttons == iMessageBoxButtons::CancelOk)
		{
			iWidgetButtonPtr okButton = new iWidgetButton();
			okButton->setText("OK");
			okButton->registerOnClickEvent(iClickDelegate(this, &iDialogMessageBox::onOK));
			buttonGrid->addWidget(okButton, i--, 0);
		}

		if (buttons == iMessageBoxButtons::CancelOk || buttons == iMessageBoxButtons::YesNoCancel)
		{
			iWidgetButtonPtr cancelButton = new iWidgetButton();
			cancelButton->setText("Cancel");
			cancelButton->registerOnClickEvent(iClickDelegate(this, &iDialogMessageBox::onCancel));
			buttonGrid->addWidget(cancelButton, i--, 0);
			buttonGrid->addWidget(new iWidgetSpacer(4, 1), i--, 0);
		}

		if (buttons == iMessageBoxButtons::YesNoCancel || buttons == iMessageBoxButtons::YesNo)
		{
			iWidgetButtonPtr noButton = new iWidgetButton();
			noButton->setText("No");
			noButton->registerOnClickEvent(iClickDelegate(this, &iDialogMessageBox::onNo));
			buttonGrid->addWidget(noButton, i--, 0);
		}

		if (buttons == iMessageBoxButtons::YesNoCancel || buttons == iMessageBoxButtons::YesNo)
		{
			iWidgetButtonPtr yesButton = new iWidgetButton();
			yesButton->setText("Yes");
			yesButton->registerOnClickEvent(iClickDelegate(this, &iDialogMessageBox::onYes));
			buttonGrid->addWidget(yesButton, i--, 0);
		}
	}

	void iDialogMessageBox::open(iDialogCloseDelegate dialogCloseDelegate, iaString message, iMessageBoxButtons buttons)
	{
		iDialog::open(dialogCloseDelegate);

		open(message, buttons);
	}

	__IGOR_DISABLE_WARNING__(4100)
	void iDialogMessageBox::onOK(const iWidgetPtr source)
	{
		setReturnState(iDialogReturnState::Ok);
		close();
	}

	void iDialogMessageBox::onCancel(const iWidgetPtr source)
	{
		setReturnState(iDialogReturnState::Cancel);
		close();
	}

	void iDialogMessageBox::onYes(const iWidgetPtr source)
	{
		setReturnState(iDialogReturnState::Yes);
		close();
	}

	void iDialogMessageBox::onNo(const iWidgetPtr source)
	{
		setReturnState(iDialogReturnState::No);
		close();
	}
	__IGOR_ENABLE_WARNING__(4100);

} // namespace igor