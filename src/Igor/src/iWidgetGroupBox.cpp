// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetGroupBox.h>

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iWidgetUserControl.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

	iWidgetGroupBox::iWidgetGroupBox()
		: iWidget(iWidgetType::GroupBox)
	{
		_configuredWidth = 60;
		_configuredHeight = 20;
	}

	void iWidgetGroupBox::setBorder(int32 border)
	{
		_border = border;
		update();
	}

	int32 iWidgetGroupBox::getBorder()
	{
		return _border;
	}

	void iWidgetGroupBox::update()
	{
		int32 width = _configuredWidth;
		int32 height = _configuredHeight;

		if (isGrowingByContent() &&
			!_children.empty())
        {
            iWidget* widget = _children[0];
            int32 childWidth = widget->getActualWidth() + _border * 2;

			if (childWidth > width)
			{
				width = childWidth;
			}

            if (_text.isEmpty())
            {
                int32 childHeight = widget->getActualHeight() + _border * 2;
				if (childHeight > height)
				{
					height = childHeight;
				}
            }
            else
            {
                con_assert(iWidgetManager::getInstance().getTheme() != nullptr, "zero pointer");
                if (iWidgetManager::getInstance().getTheme() != nullptr)
                {
                    float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();
					int32 childHeight = static_cast<int32>(widget->getActualHeight() + _border * 2.0 + fontSize * 0.75);

					if (childHeight > height)
					{
						height = childHeight;
					}
                }
            }
        }

		iWidget::update(width, height);
	}

	void iWidgetGroupBox::setText(const iaString& text)
	{
		_text = text;
		update();
	}

	const iaString& iWidgetGroupBox::getText() const
	{
		return _text;
	}

	void iWidgetGroupBox::draw(int32 parentPosX, int32 parentPosY)
	{
		updatePosition(parentPosX, parentPosY);

		if (isVisible())
		{
			iWidgetManager::getInstance().getTheme()->drawGroupBox(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight(), _text, getAppearanceState(), isActive());

            if (!_children.empty())
			{
				iWidget* widget = _children[0];
                widget->draw(getActualPosX(), getActualPosY());
			}
		}
	}

}