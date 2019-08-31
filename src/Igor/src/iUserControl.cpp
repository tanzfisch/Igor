#include <iUserControl.h>

#include <iWidgetManager.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
	iUserControl::iUserControl()
	{
		setHorizontalAlignment(iHorizontalAlignment::Strech);
		setVerticalAlignment(iVerticalAlignment::Strech);
	}

	void iUserControl::calcMinSize()
	{
		int32 minWidth = 0;
		int32 minHeight = 0;

		if (isGrowingByContent() &&
			!_children.empty())
		{
			iWidget* widget = _children[0];
			minWidth = widget->getMinWidth();
			minHeight = widget->getMinHeight();
		}

		setClientArea(0, 0, 0, 0);
		setMinSize(minWidth, minHeight);
	}

}