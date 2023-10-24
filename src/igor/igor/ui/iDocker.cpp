// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/iDocker.h>

#include <igor/ui/iWidgetManager.h>

namespace igor
{
    iDocker::iDocker()
    {
        _root._areaA = std::make_unique<iDockArea>();
        _root._areaA->_dialog = 1;

        _root._areaB = std::make_unique<iDockArea>();
        _root._areaB->_areaA = std::make_unique<iDockArea>();
        _root._areaB->_areaB = std::make_unique<iDockArea>();
        _root._areaB->_areaB->_dialog = 6;
        _root._areaB->_verticalSplit = false;
    }

    void iDocker::draw(iDockArea *area, const iaRectanglei &rect)
    {

    }

    void iDocker::draw(uint32 desktopWidth, uint32 desktopHeight)
    {

    }

    void iDocker::update(iDockArea *area, const iaRectanglei &rect)
    {
        if (area == nullptr)
        {
            return;
        }

        iDialogPtr dialog = iWidgetManager::getInstance().getDialog(area->_dialog);
        if (dialog != nullptr)
        {
            dialog->setX(rect._x);
            dialog->setY(rect._y);
            dialog->setWidth(rect._width);
            dialog->setHeight(rect._height);

            return;
        }

        if (area->_areaA == nullptr ||
            area->_areaB == nullptr)
        {
            return;
        }

        if (area->_verticalSplit)
        {
            iaRectanglei rectA(rect._x, rect._y, rect._width * area->_ratio, rect._height);
            iaRectanglei rectB(rect._x + rectA._width, rect._y, rect._width - rectA._width, rect._height);

            update(area->_areaA.get(), rectA);
            update(area->_areaB.get(), rectB);
        }
        else
        {
            iaRectanglei rectA(rect._x, rect._y, rect._width, rect._height * area->_ratio);
            iaRectanglei rectB(rect._x, rect._y + rectA._height, rect._width, rect._height - rectA._height);

            update(area->_areaA.get(), rectA);
            update(area->_areaB.get(), rectB);
        }
    }

    void iDocker::update(uint32 desktopWidth, uint32 desktopHeight)
    {
        iaRectanglei rect(0, 0, desktopWidth, desktopHeight);
        update(&_root, rect);
    }

}
