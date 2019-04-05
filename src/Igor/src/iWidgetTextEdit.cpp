// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetTextEdit.h>

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iKeyboard.h>

namespace Igor
{

	iWidgetTextEdit::iWidgetTextEdit()
	{
        _reactOnMouseWheel = false;
	}

    iWidget* iWidgetTextEdit::createInstance()
    {
        return new iWidgetTextEdit();
    }

	void iWidgetTextEdit::calcMinSize()
	{
        setMinSize(0, 0);
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
						_text.insert(iaString(static_cast<const char>(c)), _cursorPos);
						incCursorPos();
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

    bool iWidgetTextEdit::handleKeyUp(iKeyCode key)
    {
        return false;
    }

    bool iWidgetTextEdit::handleKeyDown(iKeyCode key)
    {
		switch (key)
		{
		case iKeyCode::Left:
			decCursorPos();
			return true;

		case iKeyCode::Right:
			incCursorPos();
			return true;
		}

        return false;
    }

    void iWidgetTextEdit::incCursorPos()
    {
        if (_cursorPos < _text.getSize())
        {
            _cursorPos++;
        }
    }

    void iWidgetTextEdit::decCursorPos()
    {
        if (_cursorPos > 0)
        {
            _cursorPos--;
        }
    }

    void iWidgetTextEdit::setCursorPos(uint64 cursorPos)
    {
        _cursorPos = min(_text.getSize(), cursorPos);
    }

	uint64 iWidgetTextEdit::getCursorPos() const
    {
        return _cursorPos;
    }

    iHorizontalAlignment iWidgetTextEdit::getHorizontalTextAlignment() const
    {
        return _horizontalTextAlignment;
    }
        
    iVerticalAlignment iWidgetTextEdit::getVerticalTextAlignment() const
    {
        return _verticalTextAlignment;
    }

	void iWidgetTextEdit::setHorizontalTextAlignment(iHorizontalAlignment align)
	{
		_horizontalTextAlignment = align;
	}

	void iWidgetTextEdit::setVerticalTextAlignment(iVerticalAlignment valign)
	{
		_verticalTextAlignment = valign;
	}

	void iWidgetTextEdit::draw()
	{
		if (isVisible())
		{
			iWidgetManager::getInstance().getTheme()->drawTextEdit(getActualRect(), _text, _cursorPos, _horizontalTextAlignment, _verticalTextAlignment, hasKeyboardFocus() && !isWriteProtected(), _widgetAppearanceState, isActive() && !_writeProtected);
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

