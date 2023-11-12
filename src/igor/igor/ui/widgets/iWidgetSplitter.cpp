
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
    static const int32 s_sectionSelectorSize = 32;
    static const int32 s_sectionSelectorSpacing = 4;
    static const float32 s_splitterWidth = 6;
    static const float32 s_halfSubdivideRatio = 0.5;

    iWidgetSplitter::iWidgetSplitter(bool dockingSplitter, const iWidgetPtr parent)
        : iWidget(iWidgetType::iWidgetSplitter, iWidgetKind::Widget, parent), _dockingSplitter(dockingSplitter)
    {
        setVerticalAlignment(iVerticalAlignment::Stretch);
        setHorizontalAlignment(iHorizontalAlignment::Stretch);
        setAcceptDrop(dockingSplitter);
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
            setMinSize(minWidth, minHeight);
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

        setMinSize(minWidth, minHeight);
    }

    void iWidgetSplitter::calcChildOffsets(std::vector<iaRectanglef> &offsets)
    {
        con_assert(getChildren().size() <= 2, "internal error");

        uint32 childCount = getChildren().size();
        if (childCount == 0)
        {
            return;
        }

        iaRectanglef clientRect;
        const float32 parentWidth = getActualWidth();
        const float32 parentHeight = getActualHeight();        

        if (childCount == 1)
        {
            clientRect._width = parentWidth;
            clientRect._height = parentHeight;
            offsets.push_back(clientRect);
            return;
        }

        if (_orientation == iSplitterOrientation::Vertical)
        {
            clientRect._width = parentWidth;
            clientRect._height = parentHeight;
            clientRect._width *= _ratio;
            offsets.push_back(clientRect);

            clientRect._width = parentWidth;
            clientRect._height = parentHeight;
            clientRect._x += clientRect._width * _ratio;
            clientRect._width *= (1.0f - _ratio);
            offsets.push_back(clientRect);
        }
        else
        {
            clientRect._width = parentWidth;
            clientRect._height = parentHeight;
            clientRect._height *= _ratio;
            offsets.push_back(clientRect);

            clientRect._width = parentWidth;
            clientRect._height = parentHeight;
            clientRect._y += clientRect._height * _ratio;
            clientRect._height *= (1.0f - _ratio);
            offsets.push_back(clientRect);
        }
    }

    void iWidgetSplitter::addWidget(iWidgetPtr widget)
    {
        con_assert(getChildren().size() <= 2, "can't add more then two");
        // TODO _dockingSplitter ?

        iWidget::addWidget(widget);

        widget->setVerticalAlignment(iVerticalAlignment::Stretch);
        widget->setHorizontalAlignment(iHorizontalAlignment::Stretch);
    }

    void iWidgetSplitter::removeWidget(iWidgetPtr widget)
    {
        iWidget::removeWidget(widget);

        // TODO _dockingSplitter ???
    }

    bool iWidgetSplitter::onMouseKeyDown(iKeyCode key)
    {
        if (!isEnabled())
        {
            return false;
        }

        if (!_isMouseOver)
        {
            return false;
        }

        // TODO?

        return iWidget::onMouseKeyDown(key);
    }

    void iWidgetSplitter::onDrop(const iDrag &drag)
    {
        // drop is not over valid area
        if (!_validDockSection)
        {
            return;
        }

        iMimeData mimeData = drag.getMimeData();
        if (!mimeData.hasWidgetID())
        {
            return;
        }

        iWidgetPtr widget = iWidgetManager::getInstance().getWidget(mimeData.getWidgetID());
        if (widget == nullptr)
        {
            return;
        }

        if (_dockSectionCenter)
        {
            addWidget(widget);
        }
        else if (_dockSectionLeft)
        {
            // TODO
        }
        else if (_dockSectionRight)
        {
            // TODO
        }
        else if (_dockSectionTop)
        {
            // TODO
        }
        else if (_dockSectionBottom)
        {
            // TODO
        }
    }

    void iWidgetSplitter::onMouseMove(const iaVector2f &pos)
    {
        if (!isEnabled())
        {
            return;
        }

        iWidget::onMouseMove(pos);

        if (!_isMouseOver ||
            !_dockingSplitter)
        {
            return;
        }

        _dockSectionCenter = false;
        _dockSectionLeft = false;
        _dockSectionRight = false;
        _dockSectionTop = false;
        _dockSectionBottom = false;

        // TODO would be nice to be able to stack dialogs
        int32 childCount = getChildren().size();
        if (childCount == 0)
        {
            if (iIntersection::intersects(pos, _centerSectionButton))
            {
                _dockSectionCenter = true;
                _validDockSection = true;

                _highlightSection = getActualRect();
                return;
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
                return;
            }
            else if (iIntersection::intersects(pos, _rightSectionButton))
            {
                _dockSectionRight = true;
                _validDockSection = true;

                iaRectanglef rightSection = getActualRect();
                rightSection.setX(rightSection.getRight() - rightSection._width * s_halfSubdivideRatio);
                rightSection.setWidth(rightSection._width * s_halfSubdivideRatio);
                _highlightSection = rightSection;
                return;
            }
            else if (iIntersection::intersects(pos, _topSectionButton))
            {
                _dockSectionTop = true;
                _validDockSection = true;

                iaRectanglef topSection = getActualRect();
                topSection.setHeight(topSection._height * s_halfSubdivideRatio);
                _highlightSection = topSection;
                return;
            }
            else if (iIntersection::intersects(pos, _bottomSectionButton))
            {
                _dockSectionBottom = true;
                _validDockSection = true;

                iaRectanglef bottomSection = getActualRect();
                bottomSection.setY(bottomSection.getBottom() - bottomSection._height * s_halfSubdivideRatio);
                bottomSection.setHeight(bottomSection._height * s_halfSubdivideRatio);
                _highlightSection = bottomSection;
                return;
            }
        }

        _validDockSection = false;
    }

    void iWidgetSplitter::drawOverlay()
    {
        loadResources();

        if (_isMouseOver) // TODO only on drag
        {
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
        }

        if (_validDockSection)
        {
            iRenderer::getInstance().drawFilledRectangle(_highlightSection, s_areaColor);
            iRenderer::getInstance().drawRectangle(_highlightSection, s_areaBorderColor);
        }

        for (const auto child : getChildren())
        {
            if (child->getWidgetType() != iWidgetType::iWidgetSplitter)
            {
                continue;
            }

            iWidgetSplitterPtr splitter = static_cast<iWidgetSplitterPtr>(child);
            splitter->drawOverlay();
        }
    }

    void iWidgetSplitter::draw()
    {
        iWidget::draw();

        // TODO
    }

    void iWidgetSplitter::onUpdate()
    {
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
    }

} // namespace igor