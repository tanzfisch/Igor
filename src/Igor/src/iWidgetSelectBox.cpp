// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetSelectBox.h>

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iTextureFont.h>
#include <iDialogMenu.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <sstream>
using namespace std;

namespace Igor
{

    iWidgetSelectBox::iWidgetSelectBox()
    {
        _reactOnMouseWheel = false;
    }

    iWidgetSelectBox::~iWidgetSelectBox()
    {
        if (_selectBox != nullptr)
        {
            iWidgetManager::getInstance().destroyDialog(_selectBox);
            _selectBox = nullptr;
        }
    }

    iWidget* iWidgetSelectBox::createInstance()
    {
        return new iWidgetSelectBox();
    }

    void iWidgetSelectBox::calcMinSize()
    {
        int32 minWidth = 0;
        int32 minHeight = 0;

        if (isGrowingByContent() &&
            !_entries.empty())
        {
            float32 fontSize = iWidgetManager::getInstance().getTheme()->getFontSize();
            int32 maxTextWidth = 0;
            for (auto text : _entries)
            {
                int32 textWidth = static_cast<int32>(iWidgetManager::getInstance().getTheme()->getFont()->measureWidth(text.first, fontSize));
                if (textWidth > maxTextWidth)
                {
                    maxTextWidth = textWidth;
                }
            }

            minHeight = static_cast<int32>(fontSize * 1.5f);
            maxTextWidth += static_cast<int32>(static_cast<float32>(minHeight) + fontSize);
            minWidth = maxTextWidth;
        }

        setMinSize(minWidth, minHeight);
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

    void iWidgetSelectBox::handleMouseMove(const iaVector2i& pos)
    {
        if (!isActive())
        {
            return;
        }

        iWidget::handleMouseMove(pos);

        int32 mx = pos._x - getActualPosX();
        int32 my = pos._y - getActualPosY();

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
                    _selectBox = static_cast<iDialogMenu*>(iWidgetManager::getInstance().createDialog("DialogMenu"));
                }

                _selectBox->setWidth(getActualWidth() - getActualHeight());
                _selectBox->setX(getActualPosX() + 2);
                _selectBox->setY(getActualPosY() + getActualHeight() + 2);

                vector<iaString> entries;
                for (auto entry : _entries)
                {
                    entries.push_back(entry.first);
                }

                _selectBox->show(entries, iDialogMenuCloseDelegate(this, &iWidgetSelectBox::onSelectionChanged));
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
        return static_cast<uint32>(_entries.size());
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

    void iWidgetSelectBox::draw()
    {
        _buttonRectangle.setX(static_cast<float32>(getActualWidth() - getActualHeight() - 1));
        _buttonRectangle.setY(0.0f);
        _buttonRectangle.setWidth(static_cast<float32>(getActualHeight()));
        _buttonRectangle.setHeight(static_cast<float32>(getActualHeight() - 1));

        if (isVisible())
        {
            iaString displayString;

            if (_currentSelection >= 0 && _currentSelection < _entries.size())
            {
                displayString = _entries[_currentSelection].first;
            }

            iWidgetManager::getInstance().getTheme()->drawSelectBox(getActualRect(), displayString, _buttonAppearanceState, isActive());
        }
    }

}
