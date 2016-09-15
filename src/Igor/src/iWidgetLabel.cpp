// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetLabel.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iTextureFont.h>

namespace Igor
{

    iWidgetLabel::iWidgetLabel()
        : iWidget(iWidgetType::Label)
    {
        _configuredHeight = 20;
        _configuredWidth = 0;
        setHorrizontalAlignment(iHorrizontalAlignment::Center);
        setVerticalAlignment(iVerticalAlignment::Center);
    }

    void iWidgetLabel::calcMinSize()
    {
        int32 minWidth = 0;
        int32 minHeight = 0;

        if (isGrowingByContent())
        {
            float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();

            if (_maxTextWidth == 0)
            {
                int32 textWidth = (int32)iWidgetManager::getInstance().getTheme()->getFont()->measureWidth(_text, fontSize);
                minWidth = textWidth;
                minHeight = fontSize;
            }
            else
            {
                int32 textHeight = (int32)iWidgetManager::getInstance().getTheme()->getFont()->measureHeight(_text, fontSize, (float32)_maxTextWidth);
                minHeight = textHeight;
                minWidth = _maxTextWidth;
            }
        }

        setMinSize(minWidth, minHeight);
    }

    void iWidgetLabel::draw(int32 parentPosX, int32 parentPosY)
    {
        if (isVisible())
        {
            iWidgetManager::getInstance().getTheme()->drawText(getActualPosX(), getActualPosY(), _text, _maxTextWidth);
        }
    }

    const iaString& iWidgetLabel::getText() const
    {
        return _text;
    }

    void iWidgetLabel::setMaxTextWidth(int32 width)
    {
        _maxTextWidth = width;
    }

    int32 iWidgetLabel::getMaxTextWidth()
    {
        return _maxTextWidth;
    }

    void iWidgetLabel::setText(const iaString& text)
    {
        _text = text;
    }

}
