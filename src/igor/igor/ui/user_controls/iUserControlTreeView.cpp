// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlTreeView.h>

#include <igor/ui/widgets/iWidgetBoxLayout.h>
#include <igor/ui/widgets/iWidgetButton.h>
#include <igor/ui/widgets/iWidgetScroll.h>
#include <igor/ui/widgets/iWidgetSpacer.h>

namespace igor
{
    iUserControlTreeView::iUserControlTreeView(const iWidgetPtr parent)
        : iUserControl(iWidgetType::iUserControlTreeView, parent)
    {
        setMinSize(30,30);

        initUI();
    }

    iUserControlTreeView::~iUserControlTreeView()
    {
    }

    void iUserControlTreeView::initUI()
    {
        iWidgetScrollPtr scroll = new iWidgetScroll(this);
        scroll->setVerticalAlignment(iVerticalAlignment::Stretch);
        scroll->setHorizontalAlignment(iHorizontalAlignment::Stretch);

        _vboxLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical);
        scroll->addWidget(_vboxLayout);
    }

    iClickTreeViewEvent& iUserControlTreeView::getClickEvent()
    {
        return _clickEvent;
    }

    void iUserControlTreeView::updateUI(iItem* item, int indentation)
    {
        int indentationNext = indentation + 1;
        for(const auto child : item->getItems())
        {
            if(!child->hasValue("displayName") ||
            !child->hasValue("relativePath") ||
            !child->hasValue("isDirectory"))
            {
                continue;
            }

            bool isDirectory = child->getValue<bool>("isDirectory");
            if(!isDirectory)
            {
                continue;
            }

            const iaString displayName = child->getValue<iaString>("displayName");
            const iaString relativePath = child->getValue<iaString>("relativePath");
            iWidgetBoxLayoutPtr buttonLayout = new iWidgetBoxLayout(iWidgetBoxLayoutType::Horizontal);

            iWidgetButtonPtr button = new iWidgetButton();
            button->setHorizontalAlignment(iHorizontalAlignment::Left);
            button->setHorizontalTextAlignment(iHorizontalAlignment::Right);
            button->setText(displayName);
            button->setTooltip(relativePath);
            button->registerOnClickEvent(iClickDelegate(this, &iUserControlTreeView::onClick));

            button->setUserData(child);

            if(child->hasValue("icon"))
            {
                const iaString icon = child->getValue<iaString>("icon");
                button->setIcon(icon);
            }

            buttonLayout->addWidget(new iWidgetSpacer(16 * indentation, button->getMinHeight()));
            buttonLayout->addWidget(button);
            
            _vboxLayout->addWidget(buttonLayout);

            updateUI(child, indentationNext);
        }
    }

    void iUserControlTreeView::onClick(const iWidgetPtr source)
    {
        // bundle all click events in one
        _clickEvent(source);
    }

    void iUserControlTreeView::setItems(iItem* items)
    {
        _vboxLayout->clearChildren();
        _root = items;

        if(_root == nullptr)
        {
            return;
        }

        updateUI(_root, 0);
    }

} // namespace igor