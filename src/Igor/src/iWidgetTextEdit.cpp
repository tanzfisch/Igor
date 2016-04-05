// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetTextEdit.h>

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iKeyboard.h>

namespace Igor
{

	iWidgetTextEdit::iWidgetTextEdit()
		: iWidget(iWidgetType::TextEdit)
	{
		_width = 80;
		_height = 20;
	}

	void iWidgetTextEdit::update()
	{
		updateParent();
	}

    void iWidgetTextEdit::setWriteProtected(bool writeProtected)
    {
        _writeProtected = writeProtected;
    }

    bool iWidgetTextEdit::isWriteProtected()
    {
        return _writeProtected;
    }

	bool iWidgetTextEdit::handleASCII(uint8 c)
	{
        if (isActive() &&
            !isWriteProtected())
        {
            if (hasKeyboardFocus())
            {
                if (c == 13) // handle enter
                {
                    if (!_triggerChangeAtOnce)
                    {
                        _change(this);
                    }
                }
                else if (c == '\b' && !_text.isEmpty()) // handle backspace
                {
                    _text = _text.getSubString(0, _text.getSize() - 1);
                }
                else
                {
                    if (c < 32 || c > 32 + 128 - 1) // filter all we don't understand
                    {
                        return false;
                    }

                    if (_text.getSize() < _maxTextLenght)
                    {
                        _text = _text + c;
                    }
                }

                if (_triggerChangeAtOnce)
                {
                    _change(this);
                }

                return true;
            }
        }

        return false;
	}

    iHorrizontalAlignment iWidgetTextEdit::getHorrizontalTextAlignment() const
    {
        return _horrizontalTextAlignment;
    }
        
    iVerticalAlignment iWidgetTextEdit::getVerticalTextAlignment() const
    {
        return _verticalTextAlignment;
    }

	void iWidgetTextEdit::setHorrizontalTextAlignment(iHorrizontalAlignment align)
	{
		_horrizontalTextAlignment = align;
	}

	void iWidgetTextEdit::setVerticalTextAlignment(iVerticalAlignment valign)
	{
		_verticalTextAlignment = valign;
	}

	void iWidgetTextEdit::setWidth(int32 width)
	{
		_width = width;
		update();
	}

	void iWidgetTextEdit::setHeight(int32 height)
	{
		_height = height;
		update();
	}

	void iWidgetTextEdit::draw()
	{
		if (isVisible())
		{
			iWidgetManager::getInstance().getTheme()->drawTextEdit(_posx, _posy, _width, _height, _text, _horrizontalTextAlignment, _verticalTextAlignment, hasKeyboardFocus() && !isWriteProtected(), _widgetAppearanceState, isActive() && !_writeProtected);
		}
	}

	const iaString& iWidgetTextEdit::getText() const
	{
		return _text;
	}

	void iWidgetTextEdit::setMaxTextLength(int32 chars)
	{
		_maxTextLenght = chars;
	}

	int32 iWidgetTextEdit::getMaxTextLength()
	{
		return _maxTextLenght;
	}

	void iWidgetTextEdit::setChangeEventAtOnce()
	{
        _triggerChangeAtOnce = true;
	}

	void iWidgetTextEdit::setChangeEventOnEnterAndLosFocus()
	{
        _triggerChangeAtOnce = false;
	}

	void iWidgetTextEdit::setText(const iaString& text)
	{
		_text = text;
	}

}

