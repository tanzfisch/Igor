// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetSelectBox.h>

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iTextureFont.h>
#include <iSelectBox.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <sstream>
using namespace std;

namespace Igor
{

	iWidgetSelectBox::iWidgetSelectBox()
		: iWidget(iWidgetType::NumberChooser)
	{

	}

    iWidgetSelectBox::~iWidgetSelectBox()
    {
        if (_selectBox != nullptr)
        {
            delete _selectBox;
            _selectBox = nullptr;
        }
    }

	void iWidgetSelectBox::update()
	{
        int32 width = _configuredWidth;
        int32 height = _configuredHeight;

        if (isGrowingByContent() &&
            !_entries.empty())
        {
            float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();
            int32 maxTextWidth = 0;
            for (auto text : _entries)
            {
                int32 textWidth = iWidgetManager::getInstance().getTheme()->getFont()->measureWidth(text.first, fontSize);
                if (textWidth > maxTextWidth)
                {
                    maxTextWidth = textWidth;
                }
            }

            if (fontSize * 1.5 > height)
            {
                height = fontSize * 1.5;
            }

            maxTextWidth += height + fontSize;

            if (maxTextWidth > width)
            {
                width = maxTextWidth;
            }
        }

        iWidget::update(width, height);
	}

	bool iWidgetSelectBox::handleMouseKeyDown(iKeyCode key)
	{
		if (!isActive())
		{
			return false;
		}

        if (_mouseOverButton)
        {
            _buttonAppearanceState = iWidgetAppearanceState::Pressed;
        }

		return iWidget::handleMouseKeyDown(key);
	}

	void iWidgetSelectBox::handleMouseMove(int32 x, int32 y)
	{
		if (!isActive())
		{
			return;
		}

        iWidget::handleMouseMove(x, y);

		int32 mx = x - getActualPosX();
		int32 my = y - getActualPosY();

		if (mx >= _buttonRectangle.getX() &&
            mx < _buttonRectangle.getX() + _buttonRectangle.getWidth() &&
			my >= _buttonRectangle.getY() &&
            my < _buttonRectangle.getY() + _buttonRectangle.getHeight())
		{
            _mouseOverButton = true;
            _buttonAppearanceState = iWidgetAppearanceState::Highlighted;
		}
		else
		{
            _mouseOverButton = false;
            _buttonAppearanceState = iWidgetAppearanceState::Standby;
		}
	}

	bool iWidgetSelectBox::handleMouseKeyUp(iKeyCode key)
	{
		if (!isActive())
		{
			return false;
		}

		if (_mouseOverButton)
		{
            if (key == iKeyCode::MouseLeft)
            {
                _buttonAppearanceState = iWidgetAppearanceState::Standby;

                if (_selectBox == nullptr)
                {
                    _selectBox = new iSelectBox();
                }

                // TODO insuficcient if select box is within a iWidgetScroll. maybe the widget system needs an other big redesign :-(
				_selectBox->setWidth(getActualWidth() - getActualHeight());
                _selectBox->setX(getActualPosX() + 2);
                _selectBox->setY(getActualPosY() + getActualHeight() + 2);

                vector<iaString> entries;
                for (auto entry : _entries)
                {
                    entries.push_back(entry.first);
                }

                _selectBox->show(entries, iSelectBoxCloseDelegate(this, &iWidgetSelectBox::onSelectionChanged));
            }

            setKeyboardFocus();
            return true;
		}

        return iWidget::handleMouseKeyUp(key);
	}

    void iWidgetSelectBox::onSelectionChanged(int32 value)
    {
        if (value > -1)
        {
            _currentSelection = value;
        }

        _change(this);
    }

	bool iWidgetSelectBox::handleMouseWheel(int32 d)
	{
		if (!isActive())
		{
			return false;
		}

        iWidget::handleMouseWheel(d);

		if (isMouseOver())
		{
			if (d < 0)
			{
				// TODO go to next lower entry
			}
			else
			{
				// TODO go to next higher entry
			}

            return true;
		}

        return false;
	}

	void iWidgetSelectBox::setSelection(uint32 key)
	{
        con_assert(key < _entries.size() || key == -1, "out of range");

        if (key < _entries.size() || key == -1)
        {
            _currentSelection = key;
        }
	}

    uint32 iWidgetSelectBox::getSelectionEntryCount() const
    {
        return _entries.size();
    }

    void iWidgetSelectBox::clear()
    {
        _entries.clear();
        _currentSelection = -1;
    }

    void iWidgetSelectBox::addSelectionEntry(const iaString& entryText, void* userData)
    {
        pair<iaString, void*> entry;
        entry.first = entryText;
        entry.second = userData;
        _entries.push_back(entry);
    }

    void* iWidgetSelectBox::getSelectedUserData() const
    {
        if (_currentSelection != -1)
        {
            return _entries[_currentSelection].second;
        }
        else
        {
            return nullptr;
        }
    }

	uint32 iWidgetSelectBox::getSelectedIndex() const
	{
        return _currentSelection;
	}

	iaString iWidgetSelectBox::getSelectedValue() const
	{
        return _entries[_currentSelection].first;
	}

	void iWidgetSelectBox::draw(int32 parentPosX, int32 parentPosY)
    {
        updatePosition(parentPosX, parentPosY);

        _buttonRectangle.setX(getActualWidth() - getActualHeight() - 1);
        _buttonRectangle.setY(0);
        _buttonRectangle.setWidth(getActualHeight());
        _buttonRectangle.setHeight(getActualHeight() - 1);

		if (isVisible())
		{
            iaString displayString;

            if(_currentSelection >= 0 && _currentSelection < _entries.size())
            { 
                displayString = _entries[_currentSelection].first;
            }

			iWidgetManager::getInstance().getTheme()->drawSelectBox(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight(), displayString, _buttonAppearanceState, isActive());
		}
	}

}
