// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/user_controls/iUserControlTreeView.h>

#include <igor/ui/widgets/iWidgetBoxLayout.h>

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
        auto vboxLayout = iWidgetBoxLayout(iWidgetBoxLayoutType::Vertical, this);
    }

    void iUserControlTreeView::draw()
    {
        if (!isVisible())
        {
            return;
        }

        // TODO
    }

} // namespace igor