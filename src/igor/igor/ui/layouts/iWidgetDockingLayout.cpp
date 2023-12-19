// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/layouts/iWidgetDockingLayout.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/ui/dialogs/iDialog.h>
#include <igor/resources/iResourceManager.h>
#include <igor/data/iIntersection.h>
#include <igor/renderer/iRenderer.h>
#include <igor/ui/widgets/iWidgetSplitter.h>

namespace igor
{
    iWidgetDockingLayout::iWidgetDockingLayout(const iWidgetPtr parent)
        : iWidget(iWidgetType::iWidgetDockingLayout, iWidgetKind::Layout, parent)
    {
        setVerticalAlignment(iVerticalAlignment::Stretch);
        setHorizontalAlignment(iHorizontalAlignment::Stretch);
        setAcceptDrop(true);

        iWidget::addWidget(new iWidgetSplitter(true));
    }

    void iWidgetDockingLayout::addWidget(iWidgetPtr widget)
    {
        con_warn("can't add or remove widgets from docking layout");
    }

    void iWidgetDockingLayout::removeWidget(iWidgetPtr widget)
    {
        con_warn("can't add or remove widgets from docking layout");
    }

} // namespace igor