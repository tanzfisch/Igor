
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
        if (_children.empty())
        {
            return;
        }

        con_assert(getChildren().size() <= 2, "internal error");

        iaRectanglef clientRect;

        if (_orientation == iSplitterOrientation::Vertical)
        {
            if (_childA != nullptr)
            {
                clientRect = getActualRect();
                clientRect._width *= _ratio;
                offsets.push_back(clientRect);
            }

            if (_childB != nullptr)
            {
                clientRect = getActualRect();
                clientRect._x += clientRect._width * _ratio;
                clientRect._width *= (1.0f - _ratio);
                offsets.push_back(clientRect);
            }
        }
        else
        {
            if (_childA != nullptr)
            {
                clientRect = getActualRect();
                clientRect._height *= _ratio;
                offsets.push_back(clientRect);
            }

            if (_childB != nullptr)
            {
                clientRect = getActualRect();
                clientRect._y += clientRect._height * _ratio;
                clientRect._height *= (1.0f - _ratio);
                offsets.push_back(clientRect);
            }
        }
    }

    void iWidgetSplitter::addWidget(iWidgetPtr widget)
    {
        con_assert(getChildren().size() <= 2, "can't add more then two");

        // TODO _dockingSplitter

        iWidget::addWidget(widget);

        if (_childA == nullptr)
        {
            _childA = widget;
            _childA->setHorizontalAlignment(iHorizontalAlignment::Stretch);
            _childA->setVerticalAlignment(iVerticalAlignment::Stretch);
        }
        else if (_childB == nullptr)
        {
            _childB = widget;
            _childB->setHorizontalAlignment(iHorizontalAlignment::Stretch);
            _childB->setVerticalAlignment(iVerticalAlignment::Stretch);
        }
    }

    void iWidgetSplitter::removeWidget(iWidgetPtr widget)
    {
        iWidget::removeWidget(widget);

        // TODO _dockingSplitter ???

        if (_childA == widget)
        {
            _childA = nullptr;
        }

        if (_childB == widget)
        {
            _childB = nullptr;
        }
    }

    bool iWidgetSplitter::handleMouseKeyDown(iKeyCode key)
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

        return iWidget::handleMouseKeyDown(key);
    }

    bool iWidgetSplitter::handleMouseKeyUp(iKeyCode key)
    {
        if (!isEnabled())
        {
            return false;
        }

        return iWidget::handleMouseKeyUp(key);
    }

    void iWidgetSplitter::handleMouseMove(const iaVector2f &pos)
    {
        if (!isEnabled())
        {
            return;
        }

        iWidget::handleMouseMove(pos);

        if (!_isMouseOver)
        {
            return;
        }

        /*iaRectanglef clientRect = getActualRect();
        iaRectanglef splitterRect = clientRect;

        if (_orientation == iSplitterOrientation::Vertical)
        {
            splitterRect._x = (splitterRect._width * _ratio) - s_splitterWidth * 0.5f;
            splitterRect._width = s_splitterWidth;
        }
        else
        {
            splitterRect._y = (splitterRect._height * _ratio) - s_splitterWidth * 0.5f;
            splitterRect._height = s_splitterWidth;
        }*/

        if (!_dockingSplitter)
        {
            return;
        }

        _dockSectionCenter = false;
        _dockSectionLeft = false;
        _dockSectionRight = false;
        _dockSectionTop = false;
        _dockSectionBottom = false;

        if (iIntersection::intersects(pos, _centerSectionButton))
        {
            _dockSectionCenter = true;
            _highlightSectionVisible = true;

            _highlightSection = getActualRect();
            return;
        }
        else if (iIntersection::intersects(pos, _leftSectionButton))
        {
            _dockSectionLeft = true;
            _highlightSectionVisible = true;

            iaRectanglef leftSection = getActualRect();
            leftSection.setWidth(leftSection._width * s_halfSubdivideRatio);
            _highlightSection = leftSection;
            return;
        }
        else if (iIntersection::intersects(pos, _rightSectionButton))
        {
            _dockSectionRight = true;
            _highlightSectionVisible = true;

            iaRectanglef rightSection = getActualRect();
            rightSection.setX(rightSection.getRight() - rightSection._width * s_halfSubdivideRatio);
            rightSection.setWidth(rightSection._width * s_halfSubdivideRatio);
            _highlightSection = rightSection;
            return;
        }
        else if (iIntersection::intersects(pos, _topSectionButton))
        {
            _dockSectionTop = true;
            _highlightSectionVisible = true;

            iaRectanglef topSection = getActualRect();
            topSection.setHeight(topSection._height * s_halfSubdivideRatio);
            _highlightSection = topSection;
            return;
        }
        else if (iIntersection::intersects(pos, _bottomSectionButton))
        {
            _dockSectionBottom = true;
            _highlightSectionVisible = true;

            iaRectanglef bottomSection = getActualRect();
            bottomSection.setY(bottomSection.getBottom() - bottomSection._height * s_halfSubdivideRatio);
            bottomSection.setHeight(bottomSection._height * s_halfSubdivideRatio);
            _highlightSection = bottomSection;
            return;
        }

        _highlightSectionVisible = false;
    }

    void iWidgetSplitter::drawOverlay()
    {
        if (_isMouseOver) // TODO only on drag
        {
            iRenderer::getInstance().drawTexturedRectangle(_centerSectionButton, _selectorCenterTexture, _dockSectionCenter ? s_areaButtonColorHighlight : s_areaButtonColor, true);
            iRenderer::getInstance().drawTexturedRectangle(_leftSectionButton, _selectorLeftTexture, _dockSectionLeft ? s_areaButtonColorHighlight : s_areaButtonColor, true);
            iRenderer::getInstance().drawTexturedRectangle(_rightSectionButton, _selectorRightTexture, _dockSectionRight ? s_areaButtonColorHighlight : s_areaButtonColor, true);
            iRenderer::getInstance().drawTexturedRectangle(_topSectionButton, _selectorTopTexture, _dockSectionTop ? s_areaButtonColorHighlight : s_areaButtonColor, true);
            iRenderer::getInstance().drawTexturedRectangle(_bottomSectionButton, _selectorBottomTexture, _dockSectionBottom ? s_areaButtonColorHighlight : s_areaButtonColor, true);
        }

        if (_highlightSectionVisible)
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
        loadResources();

        if (!isVisible())
        {
            return;
        }

        // TODO ?
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