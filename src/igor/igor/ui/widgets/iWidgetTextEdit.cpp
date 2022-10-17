// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetTextEdit.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/system/iKeyboard.h>
#include <igor/resources/texture/iTextureFont.h>

#include <algorithm>

namespace igor
{

	iWidgetTextEdit::iWidgetTextEdit(const iWidgetPtr parent)
		: iWidget(iWidgetType::iWidgetTextEdit, iWidgetKind::Widget, parent)
	{
		_reactOnMouseWheel = false;
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
		if (!isActive() || isWriteProtected() || !hasKeyboardFocus())
		{
			return false;
		}

		// skipp all we don't care about
		if (c < 32 || c > 32 + 128 - 1 || c == 13)
		{
			return true;
		}

		if (_text.getLength() < _maxTextLenght)
		{
			_text.insert(iaString(static_cast<const char>(c)), getCursorPos());
			incCursorPos();
		}

		if (_triggerChangeAtOnce)
		{
			handleChanges();
		}

		return true;
	}

	bool iWidgetTextEdit::handleKeyUp(iKeyCode key)
	{
		if (!isActive() || isWriteProtected() || !hasKeyboardFocus())
		{
			return false;
		}

		// always consume the event when widget has keyboard focus
		return true;
	}

	bool iWidgetTextEdit::handleKeyDown(iKeyCode key)
	{
		if (!isActive() || isWriteProtected() || !hasKeyboardFocus())
		{
			return false;
		}

		switch (key)
		{
		case iKeyCode::Delete:
			_text.remove(getCursorPos(), 1);
			break;

		case iKeyCode::Home:
			setCursorPos(0);
			break;

		case iKeyCode::End:
			setCursorPos(_text.getLength());
			break;

		case iKeyCode::ESC:
			_text = _textBackup;
			resetKeyboardFocus();
			return true;

		case iKeyCode::Enter:
		case iKeyCode::Return:
			resetKeyboardFocus();
			return true;

		case iKeyCode::Backspace:
			decCursorPos();
			_text.remove(getCursorPos(), 1);
			break;

		case iKeyCode::Left:
			decCursorPos();
			break;

		case iKeyCode::Right:
			incCursorPos();
			break;
		}

		if (_triggerChangeAtOnce)
		{
			handleChanges();
		}

		// always consume the event when widget has keyboard fo
		return true;
	}

	void iWidgetTextEdit::updateMetrics()
	{
		float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();
		_cursorPosPix = iWidgetManager::getInstance().getTheme()->getFont()->measureWidth(_text.getSubString(0, _cursorPos), fontSize);
	}

	void iWidgetTextEdit::incCursorPos()
	{
		if (_cursorPos < _text.getLength())
		{
			_cursorPos++;
			updateMetrics();
		}
	}

	void iWidgetTextEdit::decCursorPos()
	{
		if (_cursorPos > 0)
		{
			_cursorPos--;
			updateMetrics();
		}
	}

	void iWidgetTextEdit::setCursorPos(uint64 cursorPos)
	{
		_cursorPos = std::min((uint64)_text.getLength(), cursorPos);
		updateMetrics();
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
			iWidgetManager::getInstance().getTheme()->drawTextEdit(getActualRect(), _text, _cursorPosPix, _horizontalTextAlignment, _verticalTextAlignment, hasKeyboardFocus() && !isWriteProtected(), _widgetState, isActive() && !_writeProtected);
		}
	}

	const iaString &iWidgetTextEdit::getText() const
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

	void iWidgetTextEdit::setChangeEventOnEnterAndLostFocus()
	{
		_triggerChangeAtOnce = false;
	}

	void iWidgetTextEdit::handleGainedKeyboardFocus()
	{
		setCursorPos(_text.getLength());
		_textBackup = _text;
	}

	void iWidgetTextEdit::handleChanges()
	{
		if (_textBackup != _text)
		{
			_change(this);
		}
	}

	void iWidgetTextEdit::handleLostKeyboardFocus()
	{
		handleChanges();
	}

	void iWidgetTextEdit::setText(const iaString &text)
	{
		_text = text;
	}

} // namespace igor
