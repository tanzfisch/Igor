// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetDockingLayout.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>

namespace igor
{
    iWidgetDockingLayout::iWidgetDockingLayout(const iWidgetPtr parent)
        : iWidget(iWidgetType::iWidgetDockingLayout, iWidgetKind::Layout, parent)
    {
        setVerticalAlignment(iVerticalAlignment::Stretch);
        setHorizontalAlignment(iHorizontalAlignment::Stretch);
    }

    bool iWidgetDockingLayout::dock(iWidgetID dialogID)
    {
        bool result = _docker.dock(dialogID);
        if (result)
        {
            addWidget(iWidgetManager::getInstance().getWidget(dialogID));
        }

        return result;
    }

    bool iWidgetDockingLayout::undock(iWidgetID dialogID)
    {
        bool result = _docker.undock(dialogID);
        if (result)
        {
            iDialogPtr dialog = iWidgetManager::getInstance().getDialog(dialogID);
            removeWidget(dialog);
            dialog->setPos(dialog->getActualPos());
        }

        return result;
    }

    void iWidgetDockingLayout::onUpdate()
    {
        const iaVector2i &mousePos = iMouse::getInstance().getPos();
        _docker.update(getActualRect(), iaVector2f(mousePos._x, mousePos._y));
    }

    void iWidgetDockingLayout::calcMinSize()
    {
        iaVector2f size = _docker.calcMinSize();

        setClientArea(0, 0, 0, 0);
        setMinSize(size._x, size._y);
    }

    void iWidgetDockingLayout::draw()
    {
        if (!isVisible())
        {
            return;
        }

        iWidgetManager::getInstance().getTheme()->drawGridCell(getActualRect(), getState());

        for (const auto child : _children)
        {
            child->draw();
        }
    }

    void iWidgetDockingLayout::drawOverlay()
    {
        _docker.draw(true);
    }

} // namespace igor