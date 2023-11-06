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
            removeWidget(iWidgetManager::getInstance().getWidget(dialogID));
        }

        return result;
    }

    void iWidgetDockingLayout::onUpdate()
    {
        /*        if (dialogs.back()->_motionState == iDialogMotionState::Moving &&
                    dialogs.back()->isDockable())
                {
                    iaRectanglei rect(0, 0, _desktopWidth, _desktopHeight);
                    _docker.update(rect, iMouse::getInstance().getPos());
                }*/

        const iaVector2i &mousePos = iMouse::getInstance().getPos();
        _docker.update(getActualRect(), iaVector2f(mousePos._x, mousePos._y));
    }

    void iWidgetDockingLayout::calcMinSize()
    {
        int32 minWidth = 0;
        int32 minHeight = 0;

        // TODO docker

        setClientArea(0, 0, 0, 0);
        setMinSize(minWidth, minHeight);
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

        // if (dialogs.back()->_motionState == iDialogMotionState::Moving &&
        //   dialogs.back()->isDockable())
        {
            _docker.draw();
        }

        // _docker.drawDebug();
    }

} // namespace igor