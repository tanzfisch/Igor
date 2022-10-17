// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetSelectBox.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/ui/dialogs/iDialogIndexMenu.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <sstream>

namespace igor
{

    iWidgetSelectBox::iWidgetSelectBox(const iWidgetPtr parent)
        : iWidget(iWidgetType::iWidgetSelectBox, iWidgetKind::Widget, parent)
    {
        _reactOnMouseWheel = false;
    }

    iWidgetSelectBox::~iWidgetSelectBox()
    {
        if (_selectBox != nullptr)
        {
            delete _selectBox;
            _selectBox = nullptr;
        }
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

        if (_mouseOver)
        {
            _buttonAppearanceState = iWidgetState::Pressed;
        }

        return iWidget::handleMouseKeyDown(key);
    }

    void iWidgetSelectBox::handleMouseMove(const iaVector2i &pos)
    {
        if (!isActive())
        {
            return;
        }

        iWidget::handleMouseMove(pos);

        int32 mx = pos._x - getActualPosX() - 2; // TODO where does that offset of 2 come from?
        int32 my = pos._y - getActualPosY() - 2;

        if (mx >= 0 && mx < getActualWidth() &&
            my >= 0 && my < getActualHeight())
        {
            _mouseOver = true;
            _buttonAppearanceState = iWidgetState::Highlighted;
        }
        else
        {
            _mouseOver = false;
            _buttonAppearanceState = iWidgetState::Standby;
        }
    }

    bool iWidgetSelectBox::handleMouseKeyUp(iKeyCode key)
    {
        if (!isActive())
        {
            return false;
        }

        if (_mouseOver)
        {
            if (key == iKeyCode::MouseLeft)
            {
                _buttonAppearanceState = iWidgetState::Standby;

                if (_selectBox == nullptr)
                {
                    _selectBox = new iDialogIndexMenu();
                }

                _selectBox->setWidth(getActualWidth());
                _selectBox->setX(getActualPosX() + 2);
                _selectBox->setY(getActualPosY() + getActualHeight() + 2);

                std::vector<iaString> entries;
                for (auto entry : _entries)
                {
                    entries.push_back(entry.first);
                }

                _selectBox->open(iDialogCloseDelegate(this, &iWidgetSelectBox::onSelectBoxClosed), entries);
            }

            setKeyboardFocus();
            return true;
        }

        return iWidget::handleMouseKeyUp(key);
    }

    void iWidgetSelectBox::onSelectBoxClosed(iDialogPtr dialog)
    {
        iDialogIndexMenuPtr dialogMenu = static_cast<iDialogIndexMenuPtr>(dialog);
        auto index = dialogMenu->getSelectionIndex();

        if (index != -1)
        {
            _currentSelection = index;
        }

        _change(this);

        delete _selectBox;
        _selectBox = nullptr;
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

    void iWidgetSelectBox::addSelectionEntry(const iaString &entryText, void *userData)
    {
        std::pair<iaString, void *> entry;
        entry.first = entryText;
        entry.second = userData;
        _entries.push_back(entry);
    }

    void *iWidgetSelectBox::getSelectedUserData() const
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

} // namespace igor
