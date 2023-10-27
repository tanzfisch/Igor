// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/iDocker.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/data/iIntersection.h>
#include <igor/renderer/iRenderer.h>
#include <igor/resources/iResourceManager.h>

namespace igor
{
    static const iaColor4f s_areaColor(0.1, 0.1, 0.7, 0.1);
    static const iaColor4f s_areaBorderColor(0.6, 0.6, 0.8, 0.8);
    static const iaColor4f s_areaButtonColor(0.7, 0.7, 0.7, 0.7);
    static const iaColor4f s_areaButtonColorHighlight(1.0, 1.0, 1.0, 1.0);
    static const int32 s_selectorSize = 32;
    static const int32 s_selectorSpacing = s_selectorSize >> 3;
    static const int32 s_minSizeRectSize = s_selectorSize * 3 + s_selectorSpacing * 2;

    static const float32 s_edgeSubdivideRatio = 0.2;
    static const float32 s_halfSubdivideRatio = 0.5;

    iDocker::iDocker()
    {
        _root = std::make_shared<iDockArea>(nullptr);
    }

    void iDocker::updateTargets(iDockAreaPtr area, const iaRectanglei &rect, const iaVector2i &pos)
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
                updateTargets(area->_areaA, rectA, pos);
                return;
            }

            iaRectanglei rectB(rect._x + rectA._width, rect._y, rect._width - rectA._width, rect._height);
            if (iIntersection::intersects(pos, rectB))
            {
                updateTargets(area->_areaB, rectB, pos);
                return;
            }
        }
        else
        {
            iaRectanglei rectA(rect._x, rect._y, rect._width, rect._height * area->_ratio);
            if (iIntersection::intersects(pos, rectA))
            {
                updateTargets(area->_areaA, rectA, pos);
                return;
            }

            iaRectanglei rectB(rect._x, rect._y + rectA._height, rect._width, rect._height - rectA._height);
            if (iIntersection::intersects(pos, rectB))
            {
                updateTargets(area->_areaB, rectB, pos);
                return;
            }
        }
    }

    void iDocker::loadResources()
    {
        if (_selectorCenterTexture != nullptr)
        {
            return;
        }

        _selectorCenterTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_dock_center");
        _selectorLeftTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_dock_left_half");
        _selectorRightTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_dock_right_half");
        _selectorTopTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_dock_top_half");
        _selectorBottomTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_dock_bottom_half");
        _selectorQuarterLeftTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_dock_left_quarter");
        _selectorQuarterRightTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_dock_right_quarter");
        _selectorQuarterTopTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_dock_top_quarter");
        _selectorQuarterBottomTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_dock_bottom_quarter");
    }

    void iDocker::drawDebug(iDockAreaPtr area, const iaRectanglei &rect, int nesting)
    {
        if (area == nullptr)
        {
            return;
        }

        static const int nestingStep = 10;

        iaRectanglei drawRect = rect;
        drawRect.adjust(nesting * nestingStep, nesting * nestingStep, -(nesting * 2 * nestingStep), -(nesting * 2 * nestingStep));
        iRenderer::getInstance().drawRectangle(drawRect, iaColor4f::magenta);

        nesting++;

        if (area->_verticalSplit)
        {
            iaRectanglei rectA(rect._x, rect._y, rect._width * area->_ratio, rect._height);
            iaRectanglei rectB(rect._x + rectA._width, rect._y, rect._width - rectA._width, rect._height);
            drawDebug(area->_areaA, rectA, nesting);
            drawDebug(area->_areaB, rectB, nesting);
        }
        else
        {
            iaRectanglei rectA(rect._x, rect._y, rect._width, rect._height * area->_ratio);
            iaRectanglei rectB(rect._x, rect._y + rectA._height, rect._width, rect._height - rectA._height);
            drawDebug(area->_areaA, rectA, nesting);
            drawDebug(area->_areaB, rectB, nesting);
        }
    }

    void iDocker::draw()
    {
        loadResources();

        iRenderer::getInstance().drawFilledRectangle(_targetRect, s_areaColor);
        iRenderer::getInstance().drawRectangle(_targetRect, s_areaBorderColor);

        if (_subdivide)
        {
            iRenderer::getInstance().drawTexturedRectangle(_selectorCenter, _selectorCenterTexture, _subdivideCenter ? s_areaButtonColorHighlight : s_areaButtonColor, true);
            iRenderer::getInstance().drawTexturedRectangle(_selectorRight, _selectorRightTexture, _subdivideRightHalf ? s_areaButtonColorHighlight : s_areaButtonColor, true);
            iRenderer::getInstance().drawTexturedRectangle(_selectorLeft, _selectorLeftTexture, _subdivideLeftHalf ? s_areaButtonColorHighlight : s_areaButtonColor, true);
            iRenderer::getInstance().drawTexturedRectangle(_selectorBottom, _selectorBottomTexture, _subdivideBottomHalf ? s_areaButtonColorHighlight : s_areaButtonColor, true);
            iRenderer::getInstance().drawTexturedRectangle(_selectorTop, _selectorTopTexture, _subdivideTopHalf ? s_areaButtonColorHighlight : s_areaButtonColor, true);
        }

        iRenderer::getInstance().drawTexturedRectangle(_selectorLeftEdge, _selectorQuarterLeftTexture, _subdivideLeftEdge ? s_areaButtonColorHighlight : s_areaButtonColor, true);
        iRenderer::getInstance().drawTexturedRectangle(_selectorRightEdge, _selectorQuarterRightTexture, _subdivideRightEdge ? s_areaButtonColorHighlight : s_areaButtonColor, true);
        iRenderer::getInstance().drawTexturedRectangle(_selectorTopEdge, _selectorQuarterTopTexture, _subdivideTopEdge ? s_areaButtonColorHighlight : s_areaButtonColor, true);
        iRenderer::getInstance().drawTexturedRectangle(_selectorBottomEdge, _selectorQuarterBottomTexture, _subdivideBottomEdge ? s_areaButtonColorHighlight : s_areaButtonColor, true);

        iaRectanglei rect(0, 0, _desktopSize._x, _desktopSize._y);
        drawDebug(_root, rect, 0);
    }

    void iDocker::update(iDockAreaPtr area, const iaRectanglei &rect)
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

            update(area->_areaA, rectA);
            update(area->_areaB, rectB);
        }
        else
        {
            iaRectanglei rectA(rect._x, rect._y, rect._width, rect._height * area->_ratio);
            iaRectanglei rectB(rect._x, rect._y + rectA._height, rect._width, rect._height - rectA._height);

            update(area->_areaA, rectA);
            update(area->_areaB, rectB);
        }
    }

    void iDocker::update(const iaVector2i &desktopSize, const iaVector2i &mousePos)
    {
        _desktopSize = desktopSize;
        iaRectanglei rect(0, 0, _desktopSize._x, _desktopSize._y);
        update(_root, rect);

        _subdivide = false;
        _targetArea = nullptr;
        updateTargets(_root, rect, mousePos);

        _selectorLeftEdge.set(s_selectorSize, (_desktopSize._y >> 1) - (s_selectorSize >> 1), s_selectorSize, s_selectorSize);
        _selectorRightEdge.set(_desktopSize._x - (s_selectorSize << 1), (_desktopSize._y >> 1) - (s_selectorSize >> 1), s_selectorSize, s_selectorSize);
        _selectorTopEdge.set((_desktopSize._x >> 1) - (s_selectorSize >> 1), s_selectorSize, s_selectorSize, s_selectorSize);
        _selectorBottomEdge.set((_desktopSize._x >> 1) - (s_selectorSize >> 1), _desktopSize._y - (s_selectorSize << 1), s_selectorSize, s_selectorSize);

        _subdivideLeftEdge = iIntersection::intersects(mousePos, _selectorLeftEdge);
        _subdivideRightEdge = iIntersection::intersects(mousePos, _selectorRightEdge);
        _subdivideTopEdge = iIntersection::intersects(mousePos, _selectorTopEdge);
        _subdivideBottomEdge = iIntersection::intersects(mousePos, _selectorBottomEdge);

        if (_subdivideLeftEdge)
        {
            _targetRect = rect;
            _targetRect._width *= s_edgeSubdivideRatio;
        }

        if (_subdivideRightEdge)
        {
            _targetRect = rect;
            int32 width = _targetRect._width * s_edgeSubdivideRatio;
            _targetRect._x = _targetRect._width - width;
            _targetRect._width = width;
        }

        if (_subdivideTopEdge)
        {
            _targetRect = rect;
            _targetRect._height *= s_edgeSubdivideRatio;
        }

        if (_subdivideBottomEdge)
        {
            _targetRect = rect;
            int32 height = _targetRect._height * s_edgeSubdivideRatio;
            _targetRect._y = _targetRect._height - height;
            _targetRect._height = height;
        }
    }

    void iDocker::dock(iWidgetID dialogID)
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
            if (_subdivideLeftHalf ||
                _subdivideRightHalf ||
                _subdivideTopHalf ||
                _subdivideBottomHalf)
            {
                _targetArea->_areaA = std::make_shared<iDockArea>(_targetArea);
                _targetArea->_areaB = std::make_shared<iDockArea>(_targetArea);
                _targetArea->_ratio = s_halfSubdivideRatio;

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
            else if (_subdivideLeftEdge)
            {
                if (_root->_areaA != nullptr &&
                    _root->_areaB != nullptr)
                {
                    iDockAreaPtr tempA = _root->_areaA;
                    iDockAreaPtr tempB = _root->_areaB;

                    _root->_areaA = std::make_shared<iDockArea>(_root);
                    _root->_areaB = std::make_shared<iDockArea>(_root);
                    tempA->_parent = _root->_areaB;
                    _root->_areaB->_areaA = tempA;
                    tempB->_parent = _root->_areaB;
                    _root->_areaB->_areaB = tempB;
                    _root->_ratio = s_edgeSubdivideRatio;
                    _root->_areaA->_dialog = dialogID;
                    _root->_verticalSplit = true;
                }
                else
                {
                    _root->_areaA = std::make_shared<iDockArea>(_root);
                    _root->_areaB = std::make_shared<iDockArea>(_root);
                    _root->_ratio = s_edgeSubdivideRatio;
                    _root->_areaA->_dialog = dialogID;
                    _root->_verticalSplit = true;
                }
            }
            else
            {
                return;
            }
        }

        dialog->setX(_targetRect._x);
        dialog->setY(_targetRect._y);
        dialog->setWidth(_targetRect._width);
        dialog->setHeight(_targetRect._height);

        _targetArea = nullptr;

        update();
    }

    void iDocker::update()
    {
        iaRectanglei rect(0, 0, _desktopSize._x, _desktopSize._y);
        update(_root, rect);
    }

    bool iDocker::isEmpty(iDockAreaPtr area)
    {
        if (area == nullptr)
        {
            return true;
        }

        if (area->_dialog != iWidget::INVALID_WIDGET_ID)
        {
            return false;
        }

        if (!isEmpty(area->_areaA))
        {
            return false;
        }

        if (!isEmpty(area->_areaB))
        {
            return false;
        }

        return true;
    }

    bool iDocker::undock(iDockAreaPtr area, iWidgetID dialogID)
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

        if (undock(area->_areaA, dialogID))
        {
            if (isEmpty(area->_areaA))
            {
                // move areaB in place of area
                iDockAreaPtr areaB = area->_areaB;

                area->_areaA->_parent = nullptr;
                area->_areaA = nullptr;
                area->_areaB->_parent = nullptr;
                area->_areaB = nullptr;

                // area == root
                if (area->_parent == nullptr)
                {
                    _root = areaB;
                }
                /*else
                {
                    iDockAreaPtr parent = area->_parent;

                    if (parent->_areaA == area)
                    {
                        parent->_areaA = areaB;
                    }
                    else if (parent->_areaB == area)
                    {
                        parent->_areaB = areaB;
                    }
                }*/
            }

            return true;
        }

        if (undock(area->_areaB, dialogID))
        {
            if (isEmpty(area->_areaB))
            {
                // move areaB in place of area
                iDockAreaPtr areaA = area->_areaA;

                area->_areaA->_parent = nullptr;
                area->_areaA = nullptr;
                area->_areaB->_parent = nullptr;
                area->_areaB = nullptr;

                // area == root
                if (area->_parent == nullptr)
                {
                    _root = areaA;
                }
                /*else
                {
                    iDockAreaPtr parent = area->_parent;

                    if (parent->_areaA == area)
                    {
                        parent->_areaA = areaA;
                    }
                    else if (parent->_areaB == area)
                    {
                        parent->_areaB = areaA;
                    }
                }*/
            }

            return true;
        }

        return false;
    }

    void iDocker::undock(iWidgetID dialogID)
    {
        undock(_root, dialogID);

        update();
    }

}
