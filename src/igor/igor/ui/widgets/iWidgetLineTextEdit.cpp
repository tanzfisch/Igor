// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetLineTextEdit.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/system/iKeyboard.h>
#include <igor/resources/texture/iTextureFont.h>

#include <algorithm>

namespace igor
{

	iWidgetLineTextEdit::iWidgetLineTextEdit(const iWidgetPtr parent)
		: iWidget(iWidgetType::iWidgetLineTextEdit, iWidgetKind::Widget, parent)
	{
		_reactOnMouseWheel = false;
		_configuredMinWidth = 20;		
		_configuredMinHeight = 20;			
	}

	void iWidgetLineTextEdit::calcMinSize()
	{
		updateMinSize(0, 0);
	}

	void iWidgetLineTextEdit::setWriteProtected(bool writeProtected)
	{
		_writeProtected = writeProtected;
	}

	bool iWidgetLineTextEdit::isWriteProtected()
	{
		return _writeProtected;
	}

	bool iWidgetLineTextEdit::onASCII(iEventKeyASCII &event)
	{
		if (!isEnabled() ||
			isWriteProtected() ||
			!hasKeyboardFocus())
		{
			return false;
		}

		const char character = event.getChar();

		// skip all we don't care about
		if (character < 32 ||
			character > 126)
		{
			return true;
		}

		if (_text.getLength() < _maxTextLenght)
		{
			_text.insert(iaString(character), getCursorPos());
			incCursorPos();
		}

		if (_triggerChangeAtOnce)
		{
			handleChanges();
		}

		return true;
	}

	bool iWidgetLineTextEdit::onKeyUp(iEventKeyUp &event)
	{
		if (!isEnabled() ||
			isWriteProtected() ||
			!hasKeyboardFocus())
		{
			return false;
		}

		// always consume the event when widget has keyboard focus
		return true;
	}

	bool iWidgetLineTextEdit::onKeyDown(iEventKeyDown &event)
	{
		if (!isEnabled() ||
			isWriteProtected() ||
			!hasKeyboardFocus())
		{
			return false;
		}

		switch (event.getKey())
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

	void iWidgetLineTextEdit::updateMetrics()
	{
		float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();
		_cursorPosPix = iWidgetManager::getInstance().getTheme()->getFont()->measureWidth(_text.getSubString(0, _cursorPos), fontSize);
	}

	void iWidgetLineTextEdit::incCursorPos()
	{
		if (_cursorPos < _text.getLength())
		{
			_cursorPos++;
			updateMetrics();
		}
	}

	void iWidgetLineTextEdit::decCursorPos()
	{
		if (_cursorPos > 0)
		{
			_cursorPos--;
			updateMetrics();
		}
	}

	void iWidgetLineTextEdit::setCursorPos(uint64 cursorPos)
	{
		_cursorPos = std::min((uint64)_text.getLength(), cursorPos);
		updateMetrics();
	}

	uint64 iWidgetLineTextEdit::getCursorPos() const
	{
		return _cursorPos;
	}

	iHorizontalAlignment iWidgetLineTextEdit::getHorizontalTextAlignment() const
	{
		return _horizontalTextAlignment;
	}

	iVerticalAlignment iWidgetLineTextEdit::getVerticalTextAlignment() const
	{
		return _verticalTextAlignment;
	}

	void iWidgetLineTextEdit::setHorizontalTextAlignment(iHorizontalAlignment align)
	{
		_horizontalTextAlignment = align;
	}

	void iWidgetLineTextEdit::setVerticalTextAlignment(iVerticalAlignment valign)
	{
		_verticalTextAlignment = valign;
	}

	void iWidgetLineTextEdit::draw()
	{
		if (!isVisible())
		{
			return;
		}

		iWidgetManager::getInstance().getTheme()->drawLineTextEdit(getActualRect(), _text, _cursorPosPix, _horizontalTextAlignment, _verticalTextAlignment, hasKeyboardFocus() && !isWriteProtected(), _widgetState, isEnabled() && !_writeProtected);
	}

	const iaString &iWidgetLineTextEdit::getText() const
	{
		return _text;
	}

	void iWidgetLineTextEdit::setMaxTextLength(int32 chars)
	{
		_maxTextLenght = chars;
	}

	int32 iWidgetLineTextEdit::getMaxTextLength()
	{
		return _maxTextLenght;
	}

	void iWidgetLineTextEdit::setChangeEventAtOnce()
	{
		_triggerChangeAtOnce = true;
	}

	void iWidgetLineTextEdit::setChangeEventOnEnterAndLostFocus()
	{
		_triggerChangeAtOnce = false;
	}

	void iWidgetLineTextEdit::onGainedKeyboardFocus()
	{
		setCursorPos(_text.getLength());
		_textBackup = _text;
	}

	void iWidgetLineTextEdit::handleChanges()
	{
		if (_textBackup != _text)
		{
			_change(this);
		}
	}

	void iWidgetLineTextEdit::onLostKeyboardFocus()
	{
		handleChanges();
	}

	void iWidgetLineTextEdit::setText(const iaString &text)
	{
		_text = text;
	}

} // namespace igor
