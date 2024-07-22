// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlTreeView.h>

#include <igor/ui/layouts/iWidgetBoxLayout.h>
#include <igor/ui/widgets/iWidgetScroll.h>
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
        iWidgetScrollPtr scroll = new iWidgetScroll(this);

        _vboxLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical);
        scroll->addWidget(_vboxLayout);
    }

    iClickTreeViewEvent &iUserControlTreeView::getClickEvent()
    {
        return _clickEvent;
    }

    void iUserControlTreeView::updateUI(iItem *item, const iaString &itemPath)
    {
        std::vector<iaString> tokens;
        itemPath.split('/', tokens);
        int indentation = tokens.size();

        const iaString displayName = item->getName();
        iaString path = itemPath;
        path += "/";
        path += displayName;

        iWidgetBoxLayoutPtr buttonLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal);

        iWidgetButtonPtr button = new iWidgetButton();
        _allInteractiveWidgets.push_back(button);
        button->setHorizontalAlignment(iHorizontalAlignment::Left);
        button->setBackground(iaColor4f::transparent);
        button->setText(displayName);
        button->setCheckable(true);
        button->registerOnClickEvent(iClickDelegate(this, &iUserControlTreeView::onClick));
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
            // TODO display enabled/disabled
        }

        buttonLayout->addWidget(new iWidgetSpacer(16 * indentation, button->getMinHeight()));
        buttonLayout->addWidget(button);

        _vboxLayout->addWidget(buttonLayout);

        for (const auto subItem : item->getItems())
        {
            updateUI(subItem, path);
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

        for (const auto item : itemData->getItems())
        {
            updateUI(item, "");
        }
    }

    const iaString &iUserControlTreeView::getSelectedItemPath() const
    {
        return _selectedItemPath;
    }

    iContextMenuTreeViewEvent& iUserControlTreeView::getContextMenuTreeViewEvent()
    {
        return _contextMenuTreeViewEvent;
    }

} // namespace igor