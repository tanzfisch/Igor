// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetDockingLayout.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/ui/dialogs/iDialog.h>
#include <igor/resources/iResourceManager.h>
#include <igor/data/iIntersection.h>
#include <igor/renderer/iRenderer.h>

namespace igor
{
    static const iaColor4f s_areaColor(0.1, 0.1, 0.7, 0.1);
    static const iaColor4f s_areaBorderColor(0.6, 0.6, 0.8, 0.8);
    static const iaColor4f s_areaButtonColor(0.7, 0.7, 0.7, 0.7);
    static const iaColor4f s_areaButtonColorHighlight(1.0, 1.0, 1.0, 1.0);
    static const int32 s_sectionSelectorSize = 32;
    static const float32 s_edgeSubdivideRatio = 0.2;

    iWidgetDockingLayout::iWidgetDockingLayout(const iWidgetPtr parent)
        : iWidget(iWidgetType::iWidgetDockingLayout, iWidgetKind::Layout, parent)
    {
        setVerticalAlignment(iVerticalAlignment::Stretch);
        setHorizontalAlignment(iHorizontalAlignment::Stretch);
    }

    void iWidgetDockingLayout::loadResources()
    {
        if (_sectionLeftTexture != nullptr)
        {
            return;
        }

        _sectionLeftTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_dock_left_quarter");
        _sectionRightTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_dock_right_quarter");
        _sectionTopTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_dock_top_quarter");
        _sectionBottomTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_dock_bottom_quarter");
    }

    void iWidgetDockingLayout::onUpdate()
    {
        const iaRectanglef rect = getActualRect();
        _leftSectionButton.set(rect._x + s_sectionSelectorSize, rect._y + rect._height * 0.5 - s_sectionSelectorSize * 0.5, s_sectionSelectorSize, s_sectionSelectorSize);
        _rightSectionButton.set(rect.getRight() - s_sectionSelectorSize * 2.0, rect._y + rect._height * 0.5 - s_sectionSelectorSize * 0.5, s_sectionSelectorSize, s_sectionSelectorSize);
        _topSectionButton.set(rect._x + rect._width * 0.5 - s_sectionSelectorSize * 0.5, rect._y + s_sectionSelectorSize, s_sectionSelectorSize, s_sectionSelectorSize);
        _bottomSectionButton.set(rect._x + rect._width * 0.5 - s_sectionSelectorSize * 0.5, rect.getBottom() - s_sectionSelectorSize * 2.0, s_sectionSelectorSize, s_sectionSelectorSize);
    }

    void iWidgetDockingLayout::handleMouseMove(const iaVector2f &pos)
    {
        iWidget::handleMouseMove(pos);

        if (!_isMouseOver)
        {
            return;
        }

        _subdivideLeftEdge = false;
        _subdivideRightEdge = false;
        _subdivideTopEdge = false;
        _subdivideBottomEdge = false;

        if (iIntersection::intersects(pos, _leftSectionButton))
        {
            _subdivideLeftEdge = true;
            _highlightSectionVisible = true;

            iaRectanglef leftSection = getActualRect();
            leftSection.setWidth(leftSection._width * s_edgeSubdivideRatio);
            _highlightSection = leftSection;
            return;
        }
        else if (iIntersection::intersects(pos, _rightSectionButton))
        {
            _subdivideRightEdge = true;
            _highlightSectionVisible = true;

            iaRectanglef rightSection = getActualRect();
            rightSection.setX(rightSection.getRight() - rightSection._width * s_edgeSubdivideRatio);
            rightSection.setWidth(rightSection._width * s_edgeSubdivideRatio);
            _highlightSection = rightSection;
            return;
        }
        else if (iIntersection::intersects(pos, _topSectionButton))
        {
            _subdivideTopEdge = true;
            _highlightSectionVisible = true;

            iaRectanglef topSection = getActualRect();
            topSection.setHeight(topSection._height * s_edgeSubdivideRatio);
            _highlightSection = topSection;
            return;
        }
        else if (iIntersection::intersects(pos, _bottomSectionButton))
        {
            _subdivideBottomEdge = true;
            _highlightSectionVisible = true;

            iaRectanglef bottomSection = getActualRect();
            bottomSection.setY(bottomSection.getBottom() - bottomSection._height * s_edgeSubdivideRatio);
            bottomSection.setHeight(bottomSection._height * s_edgeSubdivideRatio);
            _highlightSection = bottomSection;
            return;
        }

        _highlightSectionVisible = false;
    }

    bool iWidgetDockingLayout::dock(iWidgetID dialogID)
    {
        return false;
    }

    bool iWidgetDockingLayout::undock(iWidgetID dialogID)
    {
        return false;
    }

    void iWidgetDockingLayout::calcMinSize()
    {
        setClientArea(0, 0, 0, 0);
        setMinSize(0, 0);
    }

    void iWidgetDockingLayout::draw()
    {
        loadResources();

        if (!isVisible())
        {
            return;
        }

        // debug
        iRenderer::getInstance().drawRectangle(getActualRect(), iaColor4f::green);

        if (_isMouseOver)
        {
            iRenderer::getInstance().drawTexturedRectangle(_leftSectionButton, _sectionLeftTexture, _subdivideLeftEdge ? s_areaButtonColorHighlight : s_areaButtonColor, true);
            iRenderer::getInstance().drawTexturedRectangle(_rightSectionButton, _sectionRightTexture, _subdivideRightEdge ? s_areaButtonColorHighlight : s_areaButtonColor, true);
            iRenderer::getInstance().drawTexturedRectangle(_topSectionButton, _sectionTopTexture, _subdivideTopEdge ? s_areaButtonColorHighlight : s_areaButtonColor, true);
            iRenderer::getInstance().drawTexturedRectangle(_bottomSectionButton, _sectionBottomTexture, _subdivideBottomEdge ? s_areaButtonColorHighlight : s_areaButtonColor, true);
        }

        if (_highlightSectionVisible)
        {
            iRenderer::getInstance().drawFilledRectangle(_highlightSection, s_areaColor);
            iRenderer::getInstance().drawRectangle(_highlightSection, s_areaBorderColor);
        }

        for (const auto child : _children)
        {
            child->draw();
        }
    }

} // namespace igor