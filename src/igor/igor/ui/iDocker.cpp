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

    void iDocker::updateTargets(std::shared_ptr<iDockArea> area, const iaRectanglef &rect, const iaVector2f &relaticeMousePos)
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

                _subdivideCenter = iIntersection::intersects(relaticeMousePos, _selectorCenter);
                _subdivideRightHalf = iIntersection::intersects(relaticeMousePos, _selectorRight);
                _subdivideLeftHalf = iIntersection::intersects(relaticeMousePos, _selectorLeft);
                _subdivideBottomHalf = iIntersection::intersects(relaticeMousePos, _selectorBottom);
                _subdivideTopHalf = iIntersection::intersects(relaticeMousePos, _selectorTop);

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
            iaRectanglef rectA(rect._x, rect._y, rect._width * area->_ratio, rect._height);
            if (iIntersection::intersects(relaticeMousePos, rectA))
            {
                updateTargets(area->_areaA, rectA, relaticeMousePos);
                return;
            }

            iaRectanglef rectB(rect._x + rectA._width, rect._y, rect._width - rectA._width, rect._height);
            if (iIntersection::intersects(relaticeMousePos, rectB))
            {
                updateTargets(area->_areaB, rectB, relaticeMousePos);
                return;
            }
        }
        else
        {
            iaRectanglef rectA(rect._x, rect._y, rect._width, rect._height * area->_ratio);
            if (iIntersection::intersects(relaticeMousePos, rectA))
            {
                updateTargets(area->_areaA, rectA, relaticeMousePos);
                return;
            }

            iaRectanglef rectB(rect._x, rect._y + rectA._height, rect._width, rect._height - rectA._height);
            if (iIntersection::intersects(relaticeMousePos, rectB))
            {
                updateTargets(area->_areaB, rectB, relaticeMousePos);
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

    void iDocker::drawDebug(std::shared_ptr<iDockArea> area, const iaRectanglef &rect, int nesting)
    {
        static const int nestingStep = 10;

        if (area == nullptr)
        {
            return;
        }

        iDialogPtr dialog = iWidgetManager::getInstance().getDialog(area->_dialog);
        if(dialog)
        {
            iaRectanglef drawRect = rect;
            drawRect.adjust(nesting * nestingStep, nesting * nestingStep, -(nesting * 2 * nestingStep), -(nesting * 2 * nestingStep));
            iRenderer::getInstance().drawRectangle(drawRect, iaColor4f::green);
            iRenderer::getInstance().drawString(drawRect._x, drawRect._y, dialog->getTitle() + "_" + iaString::toString(dialog->getID()));
        }

        if (area->_areaA == nullptr ||
            area->_areaB == nullptr)
        {
            return;
        }        

        iaRectanglef drawRect = rect;
        drawRect.adjust(nesting * nestingStep, nesting * nestingStep, -(nesting * 2 * nestingStep), -(nesting * 2 * nestingStep));
        iRenderer::getInstance().drawRectangle(drawRect, iaColor4f::magenta);

        nesting++;

        if (area->_verticalSplit)
        {
            iRenderer::getInstance().drawLine(drawRect._x + drawRect._width * area->_ratio, (float32)drawRect._y, drawRect._x + drawRect._width * area->_ratio, (float32)drawRect._y + (float32)drawRect._height);

            iaRectanglef rectA(rect._x, rect._y, rect._width * area->_ratio, rect._height);
            iaRectanglef rectB(rect._x + rectA._width, rect._y, rect._width - rectA._width, rect._height);
            drawDebug(area->_areaA, rectA, nesting);
            drawDebug(area->_areaB, rectB, nesting);
        }
        else
        {
            iRenderer::getInstance().drawLine((float32)drawRect._x, drawRect._y + drawRect._height * area->_ratio, (float32)drawRect._x + drawRect._width, drawRect._y + drawRect._height * area->_ratio);

            iaRectanglef rectA(rect._x, rect._y, rect._width, rect._height * area->_ratio);
            iaRectanglef rectB(rect._x, rect._y + rectA._height, rect._width, rect._height - rectA._height);
            drawDebug(area->_areaA, rectA, nesting);
            drawDebug(area->_areaB, rectB, nesting);
        }
    }

    void iDocker::draw()
    {
        loadResources();

        iaMatrixd modelMatrix = iRenderer::getInstance().getModelMatrix();

        iaMatrixd matrix;
        matrix.translate(_parentRect._x, _parentRect._y, -1);
        iRenderer::getInstance().setModelMatrix(matrix);

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

        iRenderer::getInstance().setModelMatrix(modelMatrix);
    }

    void iDocker::drawDebug()
    { 
        const iaRectanglef relativeRect(0, 0, _parentRect._width, _parentRect._height);
        drawDebug(_root, relativeRect, 0);
    }

    void iDocker::update(std::shared_ptr<iDockArea> area, const iaRectanglef &rect)
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
            iaRectanglef rectA(rect._x, rect._y, rect._width * area->_ratio, rect._height);
            iaRectanglef rectB(rect._x + rectA._width, rect._y, rect._width - rectA._width, rect._height);

            update(area->_areaA, rectA);
            update(area->_areaB, rectB);
        }
        else
        {
            iaRectanglef rectA(rect._x, rect._y, rect._width, rect._height * area->_ratio);
            iaRectanglef rectB(rect._x, rect._y + rectA._height, rect._width, rect._height - rectA._height);

            update(area->_areaA, rectA);
            update(area->_areaB, rectB);
        }
    }

    void iDocker::update(const iaRectanglef &parentRect, const iaVector2f &mousePos)
    {
        _parentRect = parentRect;
        const iaRectanglef relativeRect(0, 0, _parentRect._width, _parentRect._height);
        const iaVector2f relativeMousePos(mousePos._x - _parentRect._x, mousePos._y - _parentRect._y);

        update(_root, relativeRect);

        _subdivide = false;
        _targetArea = nullptr;
        updateTargets(_root, relativeRect, relativeMousePos);

        _selectorLeftEdge.set(relativeRect._x + s_selectorSize, relativeRect._y + relativeRect._height * 0.5 - s_selectorSize *0.5, s_selectorSize, s_selectorSize);
        _selectorRightEdge.set(relativeRect.getRight() - s_selectorSize * 2.0, relativeRect._y + relativeRect._height * 0.5 - s_selectorSize * 0.5, s_selectorSize, s_selectorSize);
        _selectorTopEdge.set(relativeRect._x + relativeRect._width * 0.5 - s_selectorSize * 0.5, relativeRect._y + s_selectorSize, s_selectorSize, s_selectorSize);
        _selectorBottomEdge.set(relativeRect._x + relativeRect._width * 0.5 - s_selectorSize * 0.5, relativeRect.getBottom() - s_selectorSize * 2.0, s_selectorSize, s_selectorSize);

        _subdivideLeftEdge = iIntersection::intersects(relativeMousePos, _selectorLeftEdge);
        _subdivideRightEdge = iIntersection::intersects(relativeMousePos, _selectorRightEdge);
        _subdivideTopEdge = iIntersection::intersects(relativeMousePos, _selectorTopEdge);
        _subdivideBottomEdge = iIntersection::intersects(relativeMousePos, _selectorBottomEdge);

        if (_subdivideLeftEdge)
        {
            _targetRect = relativeRect;
            _targetRect._width *= s_edgeSubdivideRatio;
        }

        if (_subdivideRightEdge)
        {
            _targetRect = relativeRect;
            int32 width = _targetRect._width * s_edgeSubdivideRatio;
            _targetRect._x = _targetRect._width - width;
            _targetRect._width = width;
        }

        if (_subdivideTopEdge)
        {
            _targetRect = relativeRect;
            _targetRect._height *= s_edgeSubdivideRatio;
        }

        if (_subdivideBottomEdge)
        {
            _targetRect = relativeRect;
            int32 height = _targetRect._height * s_edgeSubdivideRatio;
            _targetRect._y = _targetRect._height - height;
            _targetRect._height = height;
        }
    }

    bool iDocker::dock(iWidgetID dialogID)
    {
        iDialogPtr dialog = iWidgetManager::getInstance().getDialog(dialogID);
        con_assert(dialog != nullptr, "invalid id");
        
        if(_targetArea == nullptr)
        {
            return false;
        }

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
                    std::shared_ptr<iDockArea> childA = _root->_areaA;
                    std::shared_ptr<iDockArea> childB = _root->_areaB;
                    bool verticalSplit = _root->_verticalSplit;
                    float32 ratio = _root->_ratio;

                    _root->_areaA = std::make_shared<iDockArea>(_root);
                    _root->_areaA->_dialog = dialogID;
                    _root->_verticalSplit = true;
                    _root->_ratio = s_edgeSubdivideRatio;

                    _root->_areaB = std::make_shared<iDockArea>(_root);                    
                    _root->_areaB->_areaA = childA;
                    childA->_parent = _root->_areaB;
                    _root->_areaB->_areaB = childB;
                    childB->_parent = _root->_areaB;

                    _root->_areaB->_verticalSplit = verticalSplit;
                    _root->_areaB->_ratio = ratio;
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
            else if (_subdivideTopEdge)
            {
                if (_root->_areaA != nullptr &&
                    _root->_areaB != nullptr)
                {
                    std::shared_ptr<iDockArea> childA = _root->_areaA;
                    std::shared_ptr<iDockArea> childB = _root->_areaB;
                    bool verticalSplit = _root->_verticalSplit;
                    float32 ratio = _root->_ratio;

                    _root->_areaA = std::make_shared<iDockArea>(_root);
                    _root->_areaA->_dialog = dialogID;
                    _root->_verticalSplit = false;
                    _root->_ratio = s_edgeSubdivideRatio;

                    _root->_areaB = std::make_shared<iDockArea>(_root);                    
                    _root->_areaB->_areaA = childA;
                    childA->_parent = _root->_areaB;
                    _root->_areaB->_areaB = childB;
                    childB->_parent = _root->_areaB;

                    _root->_areaB->_verticalSplit = verticalSplit;
                    _root->_areaB->_ratio = ratio;
                }
                else
                {
                    _root->_areaA = std::make_shared<iDockArea>(_root);
                    _root->_areaB = std::make_shared<iDockArea>(_root);
                    _root->_ratio = s_edgeSubdivideRatio;
                    _root->_areaA->_dialog = dialogID;
                    _root->_verticalSplit = false;
                }
            }
            else
            {
                return false;
            }
        }

        dialog->setX(_targetRect._x);
        dialog->setY(_targetRect._y);
        dialog->setWidth(_targetRect._width);
        dialog->setHeight(_targetRect._height);

        _targetArea = nullptr;

        update();
        return true;
    }

    void iDocker::update()
    {
        const iaRectanglef relativeRect(0, 0, _parentRect._width, _parentRect._height);
        update(_root, relativeRect);
    }

    bool iDocker::isEmpty(std::shared_ptr<iDockArea> area)
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

    bool iDocker::undock(std::shared_ptr<iDockArea> area, iWidgetID dialogID)
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

        if (undock(area->_areaA, dialogID) || undock(area->_areaB, dialogID))
        {
            if (isEmpty(area) && area != _root)
            {
                area->_areaA->_parent.reset();
                area->_areaA = nullptr;
                area->_areaB->_parent.reset();
                area->_areaB = nullptr;
            }

            return true;
        }

        return false;
    }

    bool iDocker::undock(iWidgetID dialogID)
    {
        bool result = undock(_root, dialogID);
        update();

        return result;
    }

}
