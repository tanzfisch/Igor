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
        // update();
    }

    int32 iWidgetGroupBox::getBorder()
    {
        return _border;
    }

    void iWidgetGroupBox::updateContentSize()
    {
        int32 width = _configuredWidth;
        int32 height = _configuredHeight;

        if (isGrowingByContent() &&
            !_children.empty())
        {
            iWidget* widget = _children[0];
            int32 tempWidth = widget->getContentWidth() + _border * 2;

            if (tempWidth > width)
            {
                width = tempWidth;
            }

            if (_text.isEmpty())
            {
                int32 tempHeight = widget->getContentHeight() + _border * 2;
                if (tempHeight > height)
                {
                    height = tempHeight;
                }
            }
            else
            {
                con_assert(iWidgetManager::getInstance().getTheme() != nullptr, "zero pointer");
                if (iWidgetManager::getInstance().getTheme() != nullptr)
                {
                    float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();
                    int32 tempHeight = static_cast<int32>(widget->getContentHeight() + _border * 2.0 + fontSize);

                    if (tempHeight > height)
                    {
                        height = tempHeight;
                    }
                }
            }
        }

        setContentSize(width, height);
    }

    void iWidgetGroupBox::setText(const iaString& text)
    {
        _text = text;
        //update();
    }

    const iaString& iWidgetGroupBox::getText() const
    {
        return _text;
    }

    void iWidgetGroupBox::draw(int32 parentPosX, int32 parentPosY)
    {
        //updatePosition(parentPosX, parentPosY);

        if (isVisible())
        {
            iWidgetManager::getInstance().getTheme()->drawGroupBox(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight(), _border, _text, getAppearanceState(), isActive());

            if (!_children.empty())
            {
                int32 realX = _absoluteX;
                int32 realY = _absoluteY;
                int32 realWidth = _actualWidth;
                int32 realHeight = _actualHeight;

                _absoluteX = realX + _border;
                _absoluteY = realY + _border;
                _actualWidth = realWidth - _border * 2;
                _actualHeight = realHeight - _border * 2;

                if (iWidgetManager::getInstance().getTheme() != nullptr)
                {
                    float32 fontHeight = iWidgetManager::getInstance().getTheme()->getFontSize();
                    _actualHeight -= fontHeight;
                    _absoluteY += fontHeight;
                }

                // updating childrens alignment once more but this time with fake parent boundaries
                iWidget* widget = _children[0];
               // TODO widget->updateAlignment();
                widget->draw(getActualPosX(), getActualPosY());
                // TODO widget->updatePosition(_absoluteX, _absoluteY);

                _absoluteX = realX;
                _absoluteY = realY;
                _actualWidth = realWidth;
                _actualHeight = realHeight;
            }
        }
    }

}