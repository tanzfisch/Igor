// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlGridView.h>

#include <igor/ui/widgets/iWidgetScroll.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/resources/iResourceManager.h>

namespace igor
{
    iUserControlGridView::iUserControlGridView(const iWidgetPtr parent)
        : iUserControl(iWidgetType::iUserControlGridView, parent)
    {
        setMinSize(30, 30);

        initUI();
    }

    void iUserControlGridView::initUI()
    {
        iWidgetScrollPtr scroll = new iWidgetScroll(this);
        scroll->setVerticalAlignment(iVerticalAlignment::Stretch);
        scroll->setHorizontalAlignment(iHorizontalAlignment::Stretch);

        _vboxLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical);
        scroll->addWidget(_vboxLayout);
    }

    iClickGridViewEvent &iUserControlGridView::getClickEvent()
    {
        return _clickEvent;
    }

    void iUserControlGridView::onClick(const iWidgetPtr source)
    {
        // bundle all click events in one
        _clickEvent(source);
    }

    void iUserControlGridView::setItems(iItem *items)
    {
        _vboxLayout->clearChildren();
        _items = items;

        if (_items == nullptr)
        {
            return;
        }

        for (auto item : _items->getItems())
        {
            if (!item->hasValue("isDirectory") ||
                !item->hasValue("displayName") ||
                !item->hasValue("relativePath") ||
                !item->hasValue("uuid"))
            {
                continue;
            }

            iaUUID uuid(item->getValue<uint64>("uuid"));
            bool isDirectory = item->getValue<bool>("isDirectory");
            if (isDirectory)
            {
                continue;
            }

            const iaString displayName = item->getValue<iaString>("displayName");
            const iaString relativePath = item->getValue<iaString>("relativePath");

            auto button = new iWidgetButton();
            button->setHorizontalAlignment(iHorizontalAlignment::Left);
            button->setMinWidth(200);
            button->setText(displayName);
            button->setTooltip(relativePath);

            _vboxLayout->addWidget(button);
        }
    }

} // namespace igor