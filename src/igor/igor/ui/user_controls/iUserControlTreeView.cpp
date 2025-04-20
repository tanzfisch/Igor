// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlTreeView.h>

#include <igor/ui/layouts/iWidgetBoxLayout.h>
#include <igor/ui/widgets/iWidgetSpacer.h>

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

        _vboxLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical);
        _scroll->addWidget(_vboxLayout);

        _scroll->registerOnContextMenuEvent(iContextMenuDelegate(this, &iUserControlTreeView::onContextMenu));
    }

    iClickTreeViewEvent &iUserControlTreeView::getClickEvent()
    {
        return _clickEvent;
    }

    void iUserControlTreeView::updateUI(iItem *item, const iaString &itemPath, int indentation)
    {
        bool filter = true;

        std::vector<iaString> tokens;
        itemPath.split('/', tokens);

        iaString path = itemPath;
        path += "/";
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

                if(filter)
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

            iWidgetBoxLayoutPtr buttonLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal);

            iWidgetButtonPtr button = new iWidgetButton();
            _allInteractiveWidgets.push_back(button);
            button->setHorizontalAlignment(iHorizontalAlignment::Left);
            button->setBackground(iaColor4f::transparent);
            button->setText(displayName);
            button->setCheckable(true);
            button->getClickEvent().add(iClickDelegate(this, &iUserControlTreeView::onClick));
            button->registerOnContextMenuEvent(iContextMenuDelegate(this, &iUserControlTreeView::onContextMenu));

            if (path == _selectedItemPath)
            {
                button->setChecked(true);
            }

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

            buttonLayout->addWidget(new iWidgetSpacer(16 * indentation, button->getMinHeight()));
            buttonLayout->addWidget(button);

            _vboxLayout->addWidget(buttonLayout);

            ++indentation;
        }

        for (const auto subItem : item->getItems())
        {
            updateUI(subItem, path, indentation);
        }
    }

    void iUserControlTreeView::onClick(const iWidgetPtr source)
    {
        // change selection
        for (auto button : _allInteractiveWidgets)
        {
            button->setChecked(button == source);
        }
        _selectedItemPath = std::any_cast<iaString>(source->getUserData());

        // pass on event
        _clickEvent(source);
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
            updateUI(item, "");
        }
    }

    const iaString &iUserControlTreeView::getSelectedItemPath() const
    {
        return _selectedItemPath;
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