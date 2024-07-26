// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetSelectBox.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/ui/dialogs/iDialogIndexMenu.h>
#include <igor/data/iIntersection.h>
#include <igor/utils/iAny.h>

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

        updateMinSize(minWidth, minHeight);
    }

    bool iWidgetSelectBox::onMouseKeyDown(iEventMouseKeyDown &event)
    {
        if (!isEnabled() ||
            !isMouseOver())
        {
            return false;
        }

        _buttonAppearanceState = iWidgetState::Pressed;

        return iWidget::onMouseKeyDown(event);
    }

    void iWidgetSelectBox::onMouseMove(iEventMouseMove &event)
    {
        if (!isEnabled())
        {
            return;
        }

        iWidget::onMouseMove(event);

        if (!isMouseOver())
        {
            return;
        }

        if (!event.isConsumed())
        {
            _buttonAppearanceState = iWidgetState::Highlighted;
        }
        else
        {
            _buttonAppearanceState = iWidgetState::Standby;
        }
    }

    bool iWidgetSelectBox::onMouseKeyUp(iEventMouseKeyUp &event)
    {
        if (!isEnabled() ||
            !isMouseOver())
        {
            return false;
        }

        if (event.getKey() == iKeyCode::MouseLeft)
        {
            _buttonAppearanceState = iWidgetState::Standby;

            if (_selectBox == nullptr)
            {
                _selectBox = new iDialogIndexMenu();
            }

            _selectBox->setMinWidth(getActualWidth());
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

    void iWidgetSelectBox::setSelection(const std::any &userData)
    {
        uint32 index = 0;
        for(const auto &pair :_entries)   
        {
            if(iAny::getInstance().compare(userData, pair.second))
            {
                setSelection(index);
                return;
            }

            index++;
        }
    }

    void iWidgetSelectBox::setSelection(uint32 key)
    {
        con_assert(key < _entries.size() || key == -1, "out of range");

        if (key < _entries.size() || key == -1)
        {
            _currentSelection = key;
        }
    }

    uint32 iWidgetSelectBox::getItemCount() const
    {
        return static_cast<uint32>(_entries.size());
    }

    void iWidgetSelectBox::clear()
    {
        _entries.clear();
        _currentSelection = -1;
    }

    void iWidgetSelectBox::addItem(const iaString &entryText, const std::any &userData)
    {
        _entries.push_back({entryText, userData});
    }

    const std::any iWidgetSelectBox::getSelectedUserData() const
    {
        if (_currentSelection != -1)
        {
            return _entries[_currentSelection].second;
        }
        else
        {
            return "";
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
        if (!isVisible())
        {
            return;
        }

        iaString displayString;

        if (_currentSelection >= 0 && _currentSelection < _entries.size())
        {
            displayString = _entries[_currentSelection].first;
        }

        iWidgetManager::getInstance().getTheme()->drawSelectBox(getActualRect(), displayString, _buttonAppearanceState, isEnabled());
    }

} // namespace igor
