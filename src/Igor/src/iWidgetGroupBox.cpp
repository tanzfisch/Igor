// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetGroupBox.h>

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iUserControl.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iWidgetGroupBox::iWidgetGroupBox()
    {
        _configuredWidth = 60;
        _configuredHeight = 20;
    }

    iWidget* iWidgetGroupBox::createInstance()
    {
        return new iWidgetGroupBox();
    }

    void iWidgetGroupBox::setBorder(int32 border)
    {
        _border = border;
    }

    int32 iWidgetGroupBox::getBorder()
    {
        return _border;
    }

    void iWidgetGroupBox::setHeaderOnly(bool headerOnly)
    {
        _headerOnly = headerOnly;
    }

    bool iWidgetGroupBox::getHeaderOnly() const
    {
        return _headerOnly;
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

                    if (_headerOnly)
                    {
                        setClientArea(0, 0, static_cast<int32>(static_cast<float32>(_border) + fontSize * 0.75f), 0);
                    }
                    else
                    {
                        setClientArea(_border, _border, static_cast<int32>(static_cast<float32>(_border) + fontSize * 0.75), _border);
                    }
                }
            }
            else
            {
                if (_headerOnly)
                {
                    setClientArea(0, 0, _border, 0);
                }
                else
                {
                    setClientArea(_border, _border, _border, _border);
                }
            }
        }
        else
        {
            // TODO copy paste code
            if (_headerOnly)
            {
                setClientArea(0, 0, _border, 0);
            }
            else
            {
                setClientArea(_border, _border, _border, _border);
            }
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

    void iWidgetGroupBox::draw()
    {
        if (isVisible())
        {
            iWidgetManager::getInstance().getTheme()->drawGroupBox(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight(), _headerOnly, _text, getAppearanceState(), isActive());

            if (!_children.empty())
            {
                iWidget* widget = _children[0];

                if (widget != nullptr)
                {
                    widget->draw();
                }
            }
        }
    }

}