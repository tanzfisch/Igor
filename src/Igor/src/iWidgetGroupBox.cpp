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
		_height = 20;
		_width = 60;
	}

	void iWidgetGroupBox::setBorder(int32 border)
	{
		_border = border;
	}

	int32 iWidgetGroupBox::getBorder()
	{
		return _border;
	}

	void iWidgetGroupBox::update()
	{
        if (!_widgets.empty())
        {
            iWidget* widget = _widgets[0];
            _width = widget->getWidth() + _border * 2;
            if (_text.isEmpty())
            {
                _height = widget->getHeight() + _border * 2;
            }
            else
            {
                con_assert(iWidgetManager::getInstance().getTheme() != nullptr, "zero pointer");
                if (iWidgetManager::getInstance().getTheme() != nullptr)
                {
                    float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();
                    _height = static_cast<int32>(widget->getHeight() + _border * 2.0 + fontSize * 0.75);
                }
            }
        }

		updateParent();
	}

	void iWidgetGroupBox::setWidth(int32 width)
	{
		_width = width;
		update();
	}

	void iWidgetGroupBox::setHeight(int32 height)
	{
		_height = height;
		update();
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

	void iWidgetGroupBox::draw()
	{
		if (isVisible())
		{
			iWidgetManager::getInstance().getTheme()->drawGroupBox(_posx, _posy, _width, _height, _text, _widgetAppearanceState, isActive());

            if (!_widgets.empty())
			{
                iWidget* widget = _widgets[0];

				float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();

				int32 posx2 = _posx + _border;
				int32 posy2 = static_cast<int32>(_posy + _border + fontSize * 0.75);

				int32 width2 = _width - _border * 2;
				int32 height2 = static_cast<int32>(_height - _border - fontSize * 0.75);
				
				if (_text.isEmpty())
				{
					posy2 = _posy + _border;
					height2 = _height - _border * 2;
				}

                widget->calcPosition(posx2, posy2, width2, height2);
                widget->draw();
			}
		}
	}

}