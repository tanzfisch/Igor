// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
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

    void iUserControlTreeView::updateUI(iItem *item, const iaString &itemPath, int indentation)
    {
        int indentationNext = indentation + 1;
        for (const auto child : item->getItems())
        {
            if (!child->hasValue("relativePath") ||
                !child->hasValue("isDirectory"))
            {
                continue;
            }

            bool isDirectory = child->getValue<bool>("isDirectory");
            if (!isDirectory)
            {
                continue;
            }

            const iaString displayName = child->getName();
            iaString path = itemPath;
            path += "/";
            path += displayName;

            const iaString relativePath = child->getValue<iaString>("relativePath");
            iWidgetBoxLayoutPtr buttonLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal);

            iWidgetButtonPtr button = new iWidgetButton();
            _allButtons.push_back(button);
            button->setHorizontalAlignment(iHorizontalAlignment::Left);
            button->setHorizontalTextAlignment(iHorizontalAlignment::Right);
            button->setText(displayName);
            button->setTooltip(relativePath);
            button->setCheckable(true);
            button->registerOnClickEvent(iClickDelegate(this, &iUserControlTreeView::onClick));

            if (path == _selectedItemPath)
            {
                button->setChecked(true);
            }

            button->setUserData(child);

            if (child->hasValue("icon"))
            {
                const iaString icon = child->getValue<iaString>("icon");
                button->setIcon(icon);
            }

            buttonLayout->addWidget(new iWidgetSpacer(16 * indentation, button->getMinHeight()));
            buttonLayout->addWidget(button);

            _vboxLayout->addWidget(buttonLayout);

            updateUI(child, path, indentationNext);
        }
    }

    void iUserControlTreeView::onClick(const iWidgetPtr source)
    {
        iItemPtr item = std::any_cast<iItemPtr>(source->getUserData());
        _selectedItemPath.clear();

        for (auto button : _allButtons)
        {
            button->setChecked(button == source);
        }

        while (item != nullptr && item != _root)
        {
            _selectedItemPath = iaString("/") + item->getName() + _selectedItemPath;
            item = item->getParent();
        } 

        // bundle all click events in one
        _clickEvent(source);
    }

    void iUserControlTreeView::setItems(iItem *items)
    {
        _vboxLayout->clear();
        _root = items;

        if (_root == nullptr)
        {
            return;
        }

        _allButtons.clear();
        updateUI(_root, "", 0);
    }

    const iaString &iUserControlTreeView::getSelectedItemPath() const
    {
        return _selectedItemPath;
    }

} // namespace igor