
// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetSplitter.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/resources/iResourceManager.h>
#include <igor/ui/actions/iActionManager.h>
#include <igor/data/iIntersection.h>
#include <igor/renderer/iRenderer.h>
#include <igor/ui/dialogs/iDialog.h>

namespace igor
{
    // TODO move to theme
    static const iaColor4f s_areaColor(0.1, 0.1, 0.7, 0.1);
    static const iaColor4f s_areaBorderColor(0.6, 0.6, 0.8, 0.8);
    static const iaColor4f s_areaButtonColor(0.7, 0.7, 0.7, 0.7);
    static const iaColor4f s_areaButtonColorHighlight(1.0, 1.0, 1.0, 1.0);
    static const iaColor4f s_splitterColor(0.7, 0.7, 0.7, 1.0);
    static const int32 s_sectionSelectorSize = 32;
    static const int32 s_sectionSelectorSpacing = 4;
    static const float32 s_splitterAccessWidth = 10;
    static const float32 s_splitterWidth = 1;
    static const float32 s_splitterMoveWidth = 3;
    static const float32 s_halfSubdivideRatio = 0.5;
    static const float32 s_edgeSubdivideRatio = 0.2;

    iWidgetSplitter::iWidgetSplitter(bool dockingSplitter, const iWidgetPtr parent)
        : iWidget(iWidgetType::iWidgetSplitter, iWidgetKind::Widget, parent), _dockingSplitter(dockingSplitter)
    {
        setVerticalAlignment(iVerticalAlignment::Stretch);
        setHorizontalAlignment(iHorizontalAlignment::Stretch);
        setIgnoreChildEventConsumption(true);
        setOverlayEnabled(true);
    }

    void iWidgetSplitter::setRatio(float32 ratio)
    {
        _ratio = ratio;
    }

    float32 iWidgetSplitter::getRatio() const
    {
        return _ratio;
    }

    void iWidgetSplitter::setOrientation(iSplitterOrientation orientation)
    {
        _orientation = orientation;
    }

    iSplitterOrientation iWidgetSplitter::getOrientation() const
    {
        return _orientation;
    }

    void iWidgetSplitter::calcMinSize()
    {
        int32 minWidth = 0;
        int32 minHeight = 0;

        if (!isGrowingByContent() ||
            getChildren().empty())
        {
            updateMinSize(minWidth, minHeight);
            return;
        }

        con_assert(getChildren().size() <= 2, "internal error");

        for (const auto &child : getChildren())
        {
            if (_orientation == iSplitterOrientation::Vertical)
            {
                minHeight = std::max(minHeight, child->getMinHeight());
                minWidth += child->getMinWidth();
            }
            else
            {
                minWidth = std::max(minWidth, child->getMinWidth());
                minHeight += child->getMinHeight();
            }
        }

        if (_orientation == iSplitterOrientation::Vertical)
        {
            minWidth += s_splitterWidth;
        }
        else
        {
            minHeight += s_splitterWidth;
        }

        setClientArea(0, 0, 0, 0);
        updateMinSize(minWidth, minHeight);
    }

    void iWidgetSplitter::calcChildOffsets(std::vector<iaRectanglef> &offsets)
    {
        con_assert(getChildren().size() <= 2, "internal error");

        const auto &children = getChildren();
        uint32 childCount = children.size();
        if (childCount == 0)
        {
            return;
        }

        const iaRectanglef rect(0, 0, getActualWidth(), getActualHeight());

        if (childCount == 1)
        {
            offsets.push_back(rect);
            return;
        }

        if (_orientation == iSplitterOrientation::Vertical)
        {
            bool updateRatio = false;

            int32 left = rect._width * _ratio;
            left -= s_splitterWidth / 2;

            int32 right = rect._width * (1.0f - _ratio);
            right -= s_splitterWidth / 2;

            if (left < children[0]->getMinWidth())
            {
                left = children[0]->getMinWidth();
                right = rect._width - left - s_splitterWidth;
                updateRatio = true;
            }
            else
            {
                if (right < children[1]->getMinWidth())
                {
                    right = children[1]->getMinWidth();
                    left = rect._width - right - s_splitterWidth;
                    updateRatio = true;
                }
                else
                {
                    right = rect._width - left - s_splitterWidth;
                }
            }

            iaRectanglef leftRect = rect;
            leftRect._width = left;
            offsets.push_back(leftRect);

            iaRectanglef rightRect = rect;
            rightRect._width = right;
            rightRect._x = rect._width - right;
            offsets.push_back(rightRect);

            if (updateRatio)
            {
                _ratio = (float32)left / (rect._width - s_splitterWidth);
            }
        }
        else
        {
            bool updateRatio = false;

            int32 top = rect._height * _ratio;
            top -= s_splitterWidth / 2;

            int32 bottom = rect._height * (1.0f - _ratio);
            bottom -= s_splitterWidth / 2;

            if (top < children[0]->getMinHeight())
            {
                top = children[0]->getMinHeight();
                bottom = rect._height - top - s_splitterWidth;
                updateRatio = true;
            }
            else
            {
                if (bottom < children[1]->getMinHeight())
                {
                    bottom = children[1]->getMinHeight();
                    top = rect._height - bottom - s_splitterWidth;
                    updateRatio = true;
                }
                else
                {
                    bottom = rect._height - top - s_splitterWidth;
                }
            }

            iaRectanglef topRect = rect;
            topRect._height = top;
            offsets.push_back(topRect);

            iaRectanglef bottomRect = rect;
            bottomRect._height = bottom;
            bottomRect._y = rect._height - bottom;
            offsets.push_back(bottomRect);

            if (updateRatio)
            {
                _ratio = (float32)top / (rect._height - s_splitterWidth);
            }
        }
    }

    void iWidgetSplitter::addWidget(iWidgetPtr widget)
    {
        if (getChildren().size() == 2)
        {
            con_err("can't add more then two");
            return;
        }

        iWidget::addWidget(widget);

        widget->setVerticalAlignment(iVerticalAlignment::Stretch);
        widget->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    }

    iSplitterState iWidgetSplitter::calcSplitterState(const iaVector2f &pos)
    {
        if (_activeOverlay ||
            getChildren().size() != 2)
        {
            return iSplitterState::Inactive;
        }

        iaRectanglef splitterRect = getActualRect();
        if (_orientation == iSplitterOrientation::Vertical)
        {
            splitterRect._x += (splitterRect._width * _ratio) - s_splitterAccessWidth * 0.5f;
            splitterRect._width = s_splitterAccessWidth;

            if (iIntersection::intersects(pos, splitterRect))
            {
                return iSplitterState::MouseOverVertical;
            }
        }
        else
        {
            splitterRect._y += (splitterRect._height * _ratio) - s_splitterAccessWidth * 0.5f;
            splitterRect._height = s_splitterAccessWidth;

            if (iIntersection::intersects(pos, splitterRect))
            {
                return iSplitterState::MouseOverHorizontal;
            }
        }

        return iSplitterState::Inactive;
    }

    bool iWidgetSplitter::onMouseKeyUp(iKeyCode key)
    {
        _splitterState = iSplitterState::Inactive;
        _activeOverlay = false;
        _dockSectionCenter = false;
        _dockSectionLeft = false;
        _dockSectionRight = false;
        _dockSectionTop = false;
        _dockSectionBottom = false;
        _dockSectionLeftEdge = false;
        _dockSectionRightEdge = false;
        _dockSectionTopEdge = false;
        _dockSectionBottomEdge = false;
        _validDockSection = false;

        return iWidget::onMouseKeyUp(key);
    }

    bool iWidgetSplitter::onMouseKeyDown(iKeyCode key)
    {
        if (!isEnabled() ||
            !_isMouseOver)
        {
            return false;
        }

        if (key == iKeyCode::MouseLeft ||
            key == iKeyCode::MouseRight)
        {
            _widgetState = iWidgetState::Pressed;
            _splitterState = calcSplitterState(_posLast);
            _lastMousePos.set(iMouse::getInstance().getPos()._x, iMouse::getInstance().getPos()._y);

            if (_splitterState != iSplitterState::Inactive)
            {
                return true;
            }
        }

        // get copy of children
        std::vector<iWidgetPtr> widgets = getChildren();
        bool result = false;

        for (auto widget : widgets)
        {
            if (widget->onMouseKeyDown(key))
            {
                result = true;
            }
        }

        if (!_ignoreChildEventConsumption && result)
        {
            return true;
        }

        return false;
    }

    void iWidgetSplitter::simulateDrop(iWidgetID widgetID)
    {
        iWidgetPtr widget = iWidgetManager::getInstance().getWidget(widgetID);
        if (widget == nullptr)
        {
            return;
        }

        auto children = getChildren(); // make copy
        const uint32 childCount = children.size();

        // subdivide
        if (childCount == 2)
        {
            if (!_validDockSection)
            {
                return;
            }

            iWidgetSplitterPtr splitter = new iWidgetSplitter();
            for (auto child : children)
            {
                iWidget::removeWidget(child);
                splitter->addWidget(child);
                splitter->setOrientation(getOrientation());
                splitter->setRatio(getRatio());
            }

            if (_dockSectionLeftEdge)
            {
                addWidget(widget);
                addWidget(splitter);
                setOrientation(iSplitterOrientation::Vertical);
                setRatio(s_edgeSubdivideRatio);
            }
            else if (_dockSectionRightEdge)
            {
                addWidget(splitter);
                addWidget(widget);
                setOrientation(iSplitterOrientation::Vertical);
                setRatio(1.0f - s_edgeSubdivideRatio);
            }

            else if (_dockSectionTopEdge)
            {
                addWidget(widget);
                addWidget(splitter);
                setOrientation(iSplitterOrientation::Horizontal);
                setRatio(s_edgeSubdivideRatio);
            }

            else if (_dockSectionBottomEdge)
            {
                addWidget(splitter);
                addWidget(widget);
                setOrientation(iSplitterOrientation::Horizontal);
                setRatio(1.0f - s_edgeSubdivideRatio);
            }

            _dockSectionLeftEdge = false;
            _dockSectionRightEdge = false;
            _dockSectionTopEdge = false;
            _dockSectionBottomEdge = false;
            _validDockSection = false;

            return;
        }

        addWidget(widget);

        // added second
        if (childCount == 1)
        {
            if (_dockSectionLeft ||
                _dockSectionTop)
            {
                std::reverse(_children.begin(), _children.end());
            }

            if (_dockSectionLeft || _dockSectionRight)
            {
                setOrientation(iSplitterOrientation::Vertical);
            }

            if (_dockSectionTop || _dockSectionBottom)
            {
                setOrientation(iSplitterOrientation::Horizontal);
            }
        }

        _activeOverlay = false;
        _dockSectionCenter = false;
        _dockSectionLeft = false;
        _dockSectionRight = false;
        _dockSectionTop = false;
        _dockSectionBottom = false;

        iWidgetManager::getInstance().endDrag();
    }

    static void updateCursor(iSplitterState splitterState)
    {
        iMouseCursorType cursorType;

        switch (splitterState)
        {
        case iSplitterState::MouseOverVertical:
            cursorType = iMouseCursorType::HorizontalSplit;
            break;
        case iSplitterState::MouseOverHorizontal:
            cursorType = iMouseCursorType::VeticalSplit;
            break;
        case iSplitterState::Moving:
        case iSplitterState::Inactive:
        default:
            cursorType = iMouseCursorType::Arrow;
        }

        iMouse::getInstance().setCursorType(cursorType);
    }

    void iWidgetSplitter::onMouseMove(const iaVector2f &pos, bool consumed)
    {
        if (!isEnabled())
        {
            return;
        }

        iaVector2f lastPos = _posLast;
        iWidget::onMouseMove(pos, consumed);

        if (!_isMouseOver || consumed)
        {
            return;
        }

        const auto &children = getChildren();

        if (children.size() == 2)
        {
            updateCursor(calcSplitterState(pos));
        }

        if (_splitterState != iSplitterState::Inactive &&
            _splitterState != iSplitterState::Moving &&
            _lastMousePos.distance(pos) > 3.0)
        {
            _splitterState = iSplitterState::Moving;
        }

        if (_splitterState == iSplitterState::Moving)
        {
            iaVector2f diff = pos - lastPos;

            float32 minRatio;
            float32 maxRatio;
            float32 newRatio = _ratio;

            if (getOrientation() == iSplitterOrientation::Vertical)
            {
                newRatio += diff._x / (float32)getActualWidth();
                minRatio = (float32)children[0]->getMinWidth() / (float32)getActualWidth();
                maxRatio = 1.0f - ((float32)children[1]->getMinWidth() / (float32)getActualWidth());
            }
            else
            {
                newRatio += diff._y / (float32)getActualHeight();
                minRatio = (float32)children[0]->getMinHeight() / (float32)getActualHeight();
                maxRatio = 1.0f - ((float32)children[1]->getMinHeight() / (float32)getActualHeight());
            }
            _ratio = std::clamp(newRatio, minRatio, maxRatio);
        }
    }

    void iWidgetSplitter::drawOverlay()
    {
        loadResources();

        // TODO const iMimeData &mimeData = getDrag().getMimeData(); maybe not here

        if (!_activeOverlay)
        {
            return;
        }

        int32 childCount = getChildren().size();

        if (childCount == 0)
        {
            iRenderer::getInstance().drawTexturedRectangle(_centerSectionButton, _selectorCenterTexture, _dockSectionCenter ? s_areaButtonColorHighlight : s_areaButtonColor, true);
        }
        else if (childCount == 1)
        {
            iRenderer::getInstance().drawTexturedRectangle(_leftSectionButton, _selectorLeftTexture, _dockSectionLeft ? s_areaButtonColorHighlight : s_areaButtonColor, true);
            iRenderer::getInstance().drawTexturedRectangle(_rightSectionButton, _selectorRightTexture, _dockSectionRight ? s_areaButtonColorHighlight : s_areaButtonColor, true);
            iRenderer::getInstance().drawTexturedRectangle(_topSectionButton, _selectorTopTexture, _dockSectionTop ? s_areaButtonColorHighlight : s_areaButtonColor, true);
            iRenderer::getInstance().drawTexturedRectangle(_bottomSectionButton, _selectorBottomTexture, _dockSectionBottom ? s_areaButtonColorHighlight : s_areaButtonColor, true);
        }
        else if (childCount == 2)
        {
            iRenderer::getInstance().drawTexturedRectangle(_leftEdgeSectionButton, _selectorLeftEdgeTexture, _dockSectionLeftEdge ? s_areaButtonColorHighlight : s_areaButtonColor, true);
            iRenderer::getInstance().drawTexturedRectangle(_rightEdgeSectionButton, _selectorRightEdgeTexture, _dockSectionRightEdge ? s_areaButtonColorHighlight : s_areaButtonColor, true);
            iRenderer::getInstance().drawTexturedRectangle(_topEdgeSectionButton, _selectorTopEdgeTexture, _dockSectionTopEdge ? s_areaButtonColorHighlight : s_areaButtonColor, true);
            iRenderer::getInstance().drawTexturedRectangle(_bottomEdgeSectionButton, _selectorBottomEdgeTexture, _dockSectionBottomEdge ? s_areaButtonColorHighlight : s_areaButtonColor, true);
        }

        if (_validDockSection)
        {
            iRenderer::getInstance().drawFilledRectangle(_highlightSection, s_areaColor);
            iRenderer::getInstance().drawRectangle(_highlightSection, s_areaBorderColor);
        }
    }

    void iWidgetSplitter::draw()
    {
        if (!isVisible())
        {
            return;
        }

        for (const auto child : _children)
        {
            child->draw();
        }

        if (getChildren().size() == 2)
        {
            const auto &rect = getActualRect();

            iRenderer::getInstance().setLineWidth(s_splitterWidth);

            if (getOrientation() == iSplitterOrientation::Vertical)
            {
                iRenderer::getInstance().drawLine(rect._x + rect._width * _ratio, rect._y, rect._x + rect._width * _ratio, rect._y + rect._height, iaColor4f::gray);
            }
            else
            {
                iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height * _ratio, rect._x + rect._width, rect._y + rect._height * _ratio, iaColor4f::gray);
            }

            if (_splitterState == iSplitterState::Moving)
            {
                iRenderer::getInstance().setLineWidth(s_splitterMoveWidth);

                if (getOrientation() == iSplitterOrientation::Vertical)
                {
                    iRenderer::getInstance().drawLine(rect._x + rect._width * _ratio, rect._y, rect._x + rect._width * _ratio, rect._y + rect._height, s_splitterColor);
                }
                else
                {
                    iRenderer::getInstance().drawLine(rect._x, rect._y + rect._height * _ratio, rect._x + rect._width, rect._y + rect._height * _ratio, s_splitterColor);
                }
            }

            iRenderer::getInstance().setLineWidth(1);
        }
    }

    void iWidgetSplitter::onUpdate()
    {
        iWidget::onUpdate();

        _dockSectionCenter = false;
        _dockSectionLeft = false;
        _dockSectionRight = false;
        _dockSectionTop = false;
        _dockSectionBottom = false;
        _dockSectionLeftEdge = false;
        _dockSectionRightEdge = false;
        _dockSectionTopEdge = false;
        _dockSectionBottomEdge = false;
        _validDockSection = false;

        iaVector2f pos(iMouse::getInstance().getPos()._x, iMouse::getInstance().getPos()._y);

        if (!iIntersection::intersects(pos, getActualRect()))
        {
            _activeOverlay = false;
            return;
        }

        if (iWidgetManager::getInstance().inDrag())
        {
            const iDrag &drag = iWidgetManager::getInstance().getDrag();
            iMimeData mimeData = drag.getMimeData();
            if (mimeData.hasWidgetID())
            {
                _draggedWidgetID = mimeData.getWidgetID();
            }
        }

        _activeOverlay = _draggedWidgetID != iWidget::INVALID_WIDGET_ID;

        if (_activeOverlay)
        {
            // update geometries
            const iaRectanglef rect = getActualRect();
            _centerSectionButton.set(rect.getCenter()._x - (s_sectionSelectorSize >> 1),
                                     rect.getCenter()._y - (s_sectionSelectorSize >> 1),
                                     s_sectionSelectorSize, s_sectionSelectorSize);
            _leftSectionButton = _centerSectionButton;
            _leftSectionButton.adjust(-s_sectionSelectorSize - s_sectionSelectorSpacing, 0, 0, 0);
            _rightSectionButton = _centerSectionButton;
            _rightSectionButton.adjust(s_sectionSelectorSize + s_sectionSelectorSpacing, 0, 0, 0);
            _topSectionButton = _centerSectionButton;
            _topSectionButton.adjust(0, -s_sectionSelectorSize - s_sectionSelectorSpacing, 0, 0);
            _bottomSectionButton = _centerSectionButton;
            _bottomSectionButton.adjust(0, s_sectionSelectorSize + s_sectionSelectorSpacing, 0, 0);
            _leftEdgeSectionButton.set(rect._x + s_sectionSelectorSize, rect._y + rect._height * 0.5 - s_sectionSelectorSize * 0.5, s_sectionSelectorSize, s_sectionSelectorSize);
            _rightEdgeSectionButton.set(rect.getRight() - s_sectionSelectorSize * 2.0, rect._y + rect._height * 0.5 - s_sectionSelectorSize * 0.5, s_sectionSelectorSize, s_sectionSelectorSize);
            _topEdgeSectionButton.set(rect._x + rect._width * 0.5 - s_sectionSelectorSize * 0.5, rect._y + s_sectionSelectorSize, s_sectionSelectorSize, s_sectionSelectorSize);
            _bottomEdgeSectionButton.set(rect._x + rect._width * 0.5 - s_sectionSelectorSize * 0.5, rect.getBottom() - s_sectionSelectorSize * 2.0, s_sectionSelectorSize, s_sectionSelectorSize);

            // TODO would be nice to be able to stack dialogs
            int32 childCount = getChildren().size();
            if (childCount == 0)
            {
                if (iIntersection::intersects(pos, _centerSectionButton))
                {
                    _dockSectionCenter = true;
                    _validDockSection = true;

                    _highlightSection = getActualRect();
                }
            }
            else if (childCount == 1)
            {
                if (iIntersection::intersects(pos, _leftSectionButton))
                {
                    _dockSectionLeft = true;
                    _validDockSection = true;

                    iaRectanglef leftSection = getActualRect();
                    leftSection.setWidth(leftSection._width * s_halfSubdivideRatio);
                    _highlightSection = leftSection;
                }
                else if (iIntersection::intersects(pos, _rightSectionButton))
                {
                    _dockSectionRight = true;
                    _validDockSection = true;

                    iaRectanglef rightSection = getActualRect();
                    rightSection.setX(rightSection.getRight() - rightSection._width * s_halfSubdivideRatio);
                    rightSection.setWidth(rightSection._width * s_halfSubdivideRatio);
                    _highlightSection = rightSection;
                }
                else if (iIntersection::intersects(pos, _topSectionButton))
                {
                    _dockSectionTop = true;
                    _validDockSection = true;

                    iaRectanglef topSection = getActualRect();
                    topSection.setHeight(topSection._height * s_halfSubdivideRatio);
                    _highlightSection = topSection;
                }
                else if (iIntersection::intersects(pos, _bottomSectionButton))
                {
                    _dockSectionBottom = true;
                    _validDockSection = true;

                    iaRectanglef bottomSection = getActualRect();
                    bottomSection.setY(bottomSection.getBottom() - bottomSection._height * s_halfSubdivideRatio);
                    bottomSection.setHeight(bottomSection._height * s_halfSubdivideRatio);
                    _highlightSection = bottomSection;
                }
            }

            if (iIntersection::intersects(pos, _leftEdgeSectionButton))
            {
                _dockSectionLeftEdge = true;
                _validDockSection = true;

                iaRectanglef leftSection = getActualRect();
                leftSection.setWidth(leftSection._width * s_edgeSubdivideRatio);
                _highlightSection = leftSection;
            }
            else if (iIntersection::intersects(pos, _rightEdgeSectionButton))
            {
                _dockSectionRightEdge = true;
                _validDockSection = true;

                iaRectanglef rightSection = getActualRect();
                rightSection.setX(rightSection.getRight() - rightSection._width * s_edgeSubdivideRatio);
                rightSection.setWidth(rightSection._width * s_edgeSubdivideRatio);
                _highlightSection = rightSection;
            }
            else if (iIntersection::intersects(pos, _topEdgeSectionButton))
            {
                _dockSectionTopEdge = true;
                _validDockSection = true;

                iaRectanglef topSection = getActualRect();
                topSection.setHeight(topSection._height * s_edgeSubdivideRatio);
                _highlightSection = topSection;
            }
            else if (iIntersection::intersects(pos, _bottomEdgeSectionButton))
            {
                _dockSectionBottomEdge = true;
                _validDockSection = true;

                iaRectanglef bottomSection = getActualRect();
                bottomSection.setY(bottomSection.getBottom() - bottomSection._height * s_edgeSubdivideRatio);
                bottomSection.setHeight(bottomSection._height * s_edgeSubdivideRatio);
                _highlightSection = bottomSection;
            }
        }

        // detect mouse release
        if (_activeOverlay &&
            _validDockSection &&
            !iMouse::getInstance().getLeftButton())
        {
            // simulate drop
            simulateDrop(_draggedWidgetID);

            _validDockSection = false;
            _draggedWidgetID = iWidget::INVALID_WIDGET_ID;
        }
    }

    void iWidgetSplitter::loadResources()
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

        _selectorLeftEdgeTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_dock_left_edge");
        _selectorRightEdgeTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_dock_right_edge");
        _selectorTopEdgeTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_dock_top_edge");
        _selectorBottomEdgeTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_dock_bottom_edge");
    }

} // namespace igor