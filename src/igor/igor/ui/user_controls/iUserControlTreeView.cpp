// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlTreeView.h>

#include <igor/ui/layouts/iWidgetBoxLayout.h>
#include <igor/ui/widgets/iWidgetSpacer.h>
#include <igor/system/iKeyboard.h>

namespace igor
{
    iUserControlTreeView::iUserControlTreeView(const iWidgetPtr parent)
        : iUserControl(iWidgetType::iUserControlTreeView, parent)
    {
        setMinSize(30, 30);

        initUI();
    }

    iUserControlTreeView::~iUserControlTreeView()
    {
    }

    void iUserControlTreeView::initUI()
    {
        _scroll = new iWidgetScroll(this);
        _scroll->getClickEvent().add(iClickDelegate(this, &iUserControlTreeView::onClick));
        _scroll->getContextMenuEvent().add(iContextMenuDelegate(this, &iUserControlTreeView::onContextMenu));

        _vboxLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical);
        _scroll->addWidget(_vboxLayout);
    }

    iClickTreeViewEvent &iUserControlTreeView::getClickEvent()
    {
        return _clickEvent;
    }

    void iUserControlTreeView::updateUI(iItem *item, const iItemPath &itemPath, int indentation)
    {
        bool filter = true;

        iItemPath path = itemPath;
        path += item->getID();

        if (!_filters.empty())
        {
            filter = false;
            for (const auto &pair : _filters)
            {
                if (item->hasValue(pair.first))
                {
                    std::vector<iaString> matches;
                    const auto &value = item->getValue<iaString>(pair.first);
                    for (const auto &filterValue : pair.second)
                    {
                        iaString::searchRegex(value, filterValue, matches);
                        if (!matches.empty())
                        {
                            filter = true;
                            break;
                        }
                    }
                }

                if (filter)
                {
                    break;
                }
            }
        }

        if (filter)
        {
            iaString displayName = item->getID();
            if (item->hasValue(IGOR_ITEM_DATA_NAME))
            {
                displayName = item->getValue<iaString>(IGOR_ITEM_DATA_NAME);
            }

            _buttonLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal);

            iWidgetButtonPtr button = new iWidgetButton();
            _allInteractiveWidgets.push_back(button);
            button->setHorizontalAlignment(iHorizontalAlignment::Left);
            button->setBackground(iaColor4f::transparent);
            button->setText(displayName);
            button->setSelectable(true);
            button->getClickEvent().add(iClickDelegate(this, &iUserControlTreeView::onClick));
            button->getContextMenuEvent().add(iContextMenuDelegate(this, &iUserControlTreeView::onContextMenu));

            const auto iter = std::find_if(_selectedItemPaths.begin(), _selectedItemPaths.end(),
                                           [path](const iItemPath &selectedPath)
                                           { return selectedPath == path; });
            button->setSelect(iter != _selectedItemPaths.end());

            button->setUserData(path);

            if (item->hasValue(IGOR_ITEM_DATA_ICON))
            {
                const iaString icon = item->getValue<iaString>(IGOR_ITEM_DATA_ICON);
                button->setIcon(icon);
                button->setHorizontalTextAlignment(iHorizontalAlignment::Right);
            }
            else
            {
                button->setHorizontalTextAlignment(iHorizontalAlignment::Center);
            }

            if (item->hasValue(IGOR_ITEM_DATA_ENABLED))
            {
                const bool enabled = item->getValue<bool>(IGOR_ITEM_DATA_ENABLED);
                button->setEnabled(enabled);
            }

            _buttonLayout->addWidget(new iWidgetSpacer(16 * indentation, button->getMinHeight()));
            _buttonLayout->addWidget(button);

            _vboxLayout->addWidget(_buttonLayout);

            ++indentation;
        }

        for (const auto subItem : item->getItems())
        {
            updateUI(subItem, path, indentation);
        }
    }

    void iUserControlTreeView::onClick(const iWidgetPtr source)
    {
        const bool lshift = iKeyboard::getInstance().keyPressed(iKeyCode::LShift);
        const bool lctrl = iKeyboard::getInstance().keyPressed(iKeyCode::LControl);
        const bool isButton = source->getWidgetType() == iWidgetType::iWidgetButton;
        const bool multiSelect = isMultiSelectionEnabled();

        if (multiSelect && lshift && isButton) // shift overrides ctrl
        {
            bool select = false;
            bool sourceFirst = false;
            bool oldFirst = false;
            _selectedItemPaths.clear();

            for (auto button : _allInteractiveWidgets)
            {
                bool wasSelected = button->isSelected();
                if (wasSelected && !oldFirst)
                {
                    select = !sourceFirst;
                    oldFirst = true;
                }
                else if (button == source && !sourceFirst)
                {
                    button->setSelect(true);
                    select = !oldFirst;
                    sourceFirst = true;
                }
                else
                {
                    button->setSelect(select);
                }

                if (button->isSelected())
                {
                    _selectedItemPaths.push_back(std::any_cast<iItemPath>(button->getUserData()));
                }
            }

            iWidgetButtonPtr button = static_cast<iWidgetButtonPtr>(source);
        }
        else if (multiSelect && lctrl && isButton)
        {
            for (auto button : _allInteractiveWidgets)
            {
                if (button == source)
                {
                    button->setSelect(true);
                    _selectedItemPaths.push_back(std::any_cast<iItemPath>(button->getUserData()));
                    break;
                }
            }
        }
        else
        {
            _selectedItemPaths.clear();
            for (auto button : _allInteractiveWidgets)
            {
                bool select = button == source;
                button->setSelect(select);
                if (select)
                {
                    _selectedItemPaths.push_back(std::any_cast<iItemPath>(button->getUserData()));
                }
            }
        }
        
        _selectionChanged(this); // TODO for now just always trigger it
    }

    void iUserControlTreeView::onContextMenu(const iWidgetPtr source)
    {
        _contextMenuTreeViewEvent(source);
    }

    void iUserControlTreeView::clear()
    {
        _vboxLayout->clear();
        _allInteractiveWidgets.clear();
    }

    void iUserControlTreeView::setItems(iItemData *itemData)
    {
        con_assert(itemData != nullptr, "zero pointer");
        _vboxLayout->clear();
        _allInteractiveWidgets.clear();

        _scroll->setUserData(iaString(""));

        for (const auto item : itemData->getItems())
        {
            updateUI(item, iItemPath());
        }
    }

    const std::vector<iItemPath> &iUserControlTreeView::getSelectedItemPaths() const
    {
        return _selectedItemPaths;
    }

    iContextMenuTreeViewEvent &iUserControlTreeView::getContextMenuTreeViewEvent()
    {
        return _contextMenuTreeViewEvent;
    }

    void iUserControlTreeView::clearFilter()
    {
        _filters.clear();
    }

    void iUserControlTreeView::setFilter(const iaString &key, const iaString &value)
    {
        if (_filters.end() == _filters.find(key))
        {
            _filters[key] = std::vector<iaString>();
        }

        auto &values = _filters[key];
        auto iterValue = std::find(values.begin(), values.end(), value);

        if (iterValue == values.end())
        {
            values.push_back(value);
        }
    }

} // namespace igor