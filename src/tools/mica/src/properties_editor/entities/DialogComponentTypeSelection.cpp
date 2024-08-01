// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include "DialogComponentTypeSelection.h"

namespace igor
{
	DialogComponentTypeSelection::DialogComponentTypeSelection(const iWidgetPtr parent)
		: iDialog(iWidgetType::iDialog, parent)
	{
	}

	void DialogComponentTypeSelection::open(iDialogCloseDelegate dialogCloseDelegate)
	{
		setTitle("Select Component Type");
		setVerticalAlignment(iVerticalAlignment::Center);
		setHorizontalAlignment(iHorizontalAlignment::Center);
        setResizeable(false);

        iWidgetManager::getInstance().setModal(this);
        setMinWidth(20);
        setMinHeight(20);

		initGUI();

		iDialog::open(dialogCloseDelegate);
	}

	void DialogComponentTypeSelection::initGUI()
	{
		iWidgetBoxLayoutPtr mainLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);
    	mainLayout->setHorizontalAlignment(iHorizontalAlignment::Stretch);

		
	}

} // namespace igor
