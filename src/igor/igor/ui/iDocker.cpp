// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/iDocker.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/data/iIntersection.h>
#include <igor/renderer/iRenderer.h>

namespace igor
{
    static const iaColor4f s_areaColor(0.1, 0.1, 0.7, 0.1);
    static const iaColor4f s_areaBorderColor(0.6, 0.6, 0.8, 0.8);
    static const iaColor4f s_buttonColor(0.7, 0.7, 0.7, 1.0);
    static const int32 s_selectorSize = 32;
    static const int32 s_selectorSpacing = s_selectorSize >> 3;
    static const int32 s_minSizeRectSize = s_selectorSize * 3 + s_selectorSpacing * 2;

    iDocker::iDocker()
    {
    }

    void iDocker::updateTargets(iDockArea *area, const iaRectanglei &rect, const iaVector2i &pos)
    {
        if (area == nullptr)
        {
            return;
        }

        if (area->_areaA == nullptr ||
            area->_areaB == nullptr)
        {
            _targetRect = rect;
            _targetArea = area;

            if (rect._width > s_minSizeRectSize &&
                rect._height > s_minSizeRectSize)
            {
                _subdivide = true;
                _selectorCenter.set(rect.getCenter()._x - (s_selectorSize >> 1), rect.getCenter()._y - (s_selectorSize >> 1), s_selectorSize, s_selectorSize);
                _selectorRight = _selectorCenter;
                _selectorRight.adjust(s_selectorSize + s_selectorSpacing, 0, 0, 0);
                _selectorLeft = _selectorCenter;
                _selectorLeft.adjust(-s_selectorSize - s_selectorSpacing, 0, 0, 0);
                _selectorBottom = _selectorCenter;
                _selectorBottom.adjust(0, s_selectorSize + s_selectorSpacing, 0, 0);
                _selectorTop = _selectorCenter;
                _selectorTop.adjust(0, -s_selectorSize - s_selectorSpacing, 0, 0);

                _subdivideCenter = iIntersection::intersects(pos, _selectorCenter);
                _subdivideRightHalf = iIntersection::intersects(pos, _selectorRight);
                _subdivideLeftHalf = iIntersection::intersects(pos, _selectorLeft);
                _subdivideBottomHalf = iIntersection::intersects(pos, _selectorBottom);
                _subdivideTopHalf = iIntersection::intersects(pos, _selectorTop);

                if (_subdivideLeftHalf)
                {
                    _targetRect._width = _targetRect._width / 2;
                }

                if (_subdivideRightHalf)
                {
                    _targetRect._width = _targetRect._width / 2;
                    _targetRect._x += _targetRect._width;
                }

                if (_subdivideTopHalf)
                {
                    _targetRect._height = _targetRect._height / 2;
                }

                if (_subdivideBottomHalf)
                {
                    _targetRect._height = _targetRect._height / 2;
                    _targetRect._y += _targetRect._height;
                }
            }

            return;
        }

        // find match
        if (area->_verticalSplit)
        {
            iaRectanglei rectA(rect._x, rect._y, rect._width * area->_ratio, rect._height);
            if (iIntersection::intersects(pos, rectA))
            {
                updateTargets(area->_areaA.get(), rectA, pos);
                return;
            }

            iaRectanglei rectB(rect._x + rectA._width, rect._y, rect._width - rectA._width, rect._height);
            if (iIntersection::intersects(pos, rectB))
            {
                updateTargets(area->_areaB.get(), rectB, pos);
                return;
            }
        }
        else
        {
            iaRectanglei rectA(rect._x, rect._y, rect._width, rect._height * area->_ratio);
            if (iIntersection::intersects(pos, rectA))
            {
                updateTargets(area->_areaA.get(), rectA, pos);
                return;
            }

            iaRectanglei rectB(rect._x, rect._y + rectA._height, rect._width, rect._height - rectA._height);
            if (iIntersection::intersects(pos, rectB))
            {
                updateTargets(area->_areaB.get(), rectB, pos);
                return;
            }
        }
    }

    void iDocker::draw()
    {
        iRenderer::getInstance().drawFilledRectangle(_targetRect, s_areaColor);
        iRenderer::getInstance().drawRectangle(_targetRect, s_areaBorderColor);

        if (_subdivide)
        {
            iRenderer::getInstance().drawTexturedRectangle(_selectorCenter, nullptr, _subdivideCenter ? iaColor4f::white : s_buttonColor);
            iRenderer::getInstance().drawTexturedRectangle(_selectorRight, nullptr, _subdivideRightHalf ? iaColor4f::white : s_buttonColor);
            iRenderer::getInstance().drawTexturedRectangle(_selectorLeft, nullptr, _subdivideLeftHalf ? iaColor4f::white : s_buttonColor);
            iRenderer::getInstance().drawTexturedRectangle(_selectorBottom, nullptr, _subdivideBottomHalf ? iaColor4f::white : s_buttonColor);
            iRenderer::getInstance().drawTexturedRectangle(_selectorTop, nullptr, _subdivideTopHalf ? iaColor4f::white : s_buttonColor);
        }
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

    void iDocker::update(const iaVector2i &desktopSize, const iaVector2i &mousePos)
    {
        iaRectanglei rect(0, 0, desktopSize._x, desktopSize._y);
        update(&_root, rect);

        _subdivide = false;
        _targetArea = nullptr;
        updateTargets(&_root, rect, mousePos);
    }

    void iDocker::dock(iWidgetID dialogID, const iaVector2i &desktopSize, const iaVector2i &pos)
    {
        iDialogPtr dialog = iWidgetManager::getInstance().getDialog(dialogID);
        con_assert(dialog != nullptr, "invalid id");
        con_assert(_targetArea != nullptr, "internal error");

        if (!_subdivide || _subdivideCenter)
        {
            _targetArea->_dialog = dialogID;
        }
        else
        {
            if (!_subdivideLeftHalf &&
                !_subdivideRightHalf &&
                !_subdivideTopHalf &&
                !_subdivideBottomHalf)
            {
                return;
            }

            _targetArea->_areaA = std::make_unique<iDockArea>();
            _targetArea->_areaB = std::make_unique<iDockArea>();
            _targetArea->_ratio = 0.5;

            if (_subdivideLeftHalf)
            {
                _targetArea->_areaA->_dialog = dialogID;
                _targetArea->_verticalSplit = true;
            }

            if (_subdivideRightHalf)
            {
                _targetArea->_areaB->_dialog = dialogID;
                _targetArea->_verticalSplit = true;
            }

            if (_subdivideTopHalf)
            {
                _targetArea->_areaA->_dialog = dialogID;
                _targetArea->_verticalSplit = false;
            }

            if (_subdivideBottomHalf)
            {
                _targetArea->_areaB->_dialog = dialogID;
                _targetArea->_verticalSplit = false;
            }
        }

        dialog->setX(_targetRect._x);
        dialog->setY(_targetRect._y);
        dialog->setWidth(_targetRect._width);
        dialog->setHeight(_targetRect._height);

        _targetArea = nullptr;
    }

    bool iDocker::isEmpty(iDockArea *area)
    {
        if (area == nullptr)
        {
            return true;
        }

        if (area->_dialog != iWidget::INVALID_WIDGET_ID)
        {
            return false;
        }

        if (!isEmpty(area->_areaA.get()))
        {
            return false;
        }

        if (!isEmpty(area->_areaB.get()))
        {
            return false;
        }

        return true;
    }

    bool iDocker::undock(iDockArea *area, iWidgetID dialogID)
    {
        if (area == nullptr)
        {
            return false;
        }

        if (area->_dialog == dialogID)
        {
            area->_dialog = iWidget::INVALID_WIDGET_ID;
            return true;
        }

        if (undock(area->_areaA.get(), dialogID) || undock(area->_areaB.get(), dialogID))
        {
            if (isEmpty(area))
            {
                area->_areaA = nullptr;
                area->_areaB = nullptr;
            }
        }

        return false;
    }

    void iDocker::undock(iWidgetID dialogID)
    {
        undock(&_root, dialogID);
    }

}
