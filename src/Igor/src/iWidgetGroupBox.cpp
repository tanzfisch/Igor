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
        _border = 4;
    }

    void iWidgetGroupBox::setBorder(int32 border)
    {
        _border = border;
    }

    int32 iWidgetGroupBox::getBorder()
    {
        return _border;
    }

    void iWidgetGroupBox::calcMinSize()
    {
        int32 minWidth = 0;
        int32 minHeight = 0;

        if (isGrowingByContent() &&
            !_children.empty())
        {
            iWidget* widget = _children[0];

            if (widget->getMinWidth() > minWidth)
            {
                minWidth = widget->getMinWidth();
            }

            if (widget->getMinHeight() > minHeight)
            {
                minHeight = widget->getMinHeight();
            }

            if (!_text.isEmpty())
            {
                con_assert(iWidgetManager::getInstance().getTheme() != nullptr, "zero pointer");
                if (iWidgetManager::getInstance().getTheme() != nullptr)
                {
                    float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();
                    setClientArea(_border, _border, _border + fontSize * 0.5, _border);
                }
            }
            else
            {
                setClientArea(_border, _border, _border, _border);
            }
        }
        else
        {
            setClientArea(_border, _border, _border, _border);
        }

        setMinSize(minWidth, minHeight);
    }

    void iWidgetGroupBox::setText(const iaString& text)
    {
        _text = text;
    }

    const iaString& iWidgetGroupBox::getText() const
    {
        return _text;
    }

    void iWidgetGroupBox::draw(int32 parentPosX, int32 parentPosY)
    {
        if (isVisible())
        {
            iWidgetManager::getInstance().getTheme()->drawGroupBox(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight(), _text, getAppearanceState(), isActive());

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

                iWidget* widget = _children[0];

                if (widget != nullptr)
                {
                    widget->draw(getActualPosX(), getActualPosY());
                }

                _absoluteX = realX;
                _absoluteY = realY;
                _actualWidth = realWidth;
                _actualHeight = realHeight;
            }
        }
    }

}