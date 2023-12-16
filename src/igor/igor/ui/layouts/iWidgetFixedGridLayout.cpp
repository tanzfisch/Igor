// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/layouts/iWidgetFixedGridLayout.h>

namespace igor
{
    iWidgetFixedGridLayout::iWidgetFixedGridLayout(const iWidgetPtr parent)
        : iWidget(iWidgetType::iWidgetFixedGridLayout, iWidgetKind::Layout, parent)
    {
    }

    void iWidgetFixedGridLayout::setCellSize(const iaVector2f &cellSize)
    {
        _cellSize = cellSize;
    }

    const iaVector2f &iWidgetFixedGridLayout::getCellSize() const
    {
        return _cellSize;
    }

    void iWidgetFixedGridLayout::setMinCellSpacing(float32 minCellSpacing)
    {
        _minCellSpacing = minCellSpacing;
    }

    float32 iWidgetFixedGridLayout::getMinCellSpacing() const
    {
        return _minCellSpacing;
    }

    void iWidgetFixedGridLayout::calcMinSize()
    {
        if (getParent() != nullptr &&
            !getChildren().empty())
        {
            float32 width = getParent()->getActualWidth() - _minCellSpacing;
            float32 iconCountX = std::floor(width / (_cellSize._x + _minCellSpacing));
            float32 iconCountY = std::ceil((float32)getChildren().size() / iconCountX);

            updateMinSize((_cellSize._x + _minCellSpacing) * iconCountX + _minCellSpacing,
                          (_cellSize._y + _minCellSpacing) * iconCountY + _minCellSpacing);
        }
        else
        {
            updateMinSize(0, 0);
        }
    }

    void iWidgetFixedGridLayout::calcChildOffsets(std::vector<iaRectanglef> &offsets)
    {
        const auto &children = getChildren();
        offsets.reserve(children.size());

        float32 width = getActualWidth();
        if (getParent() != nullptr)
        {
            width = getParent()->getActualWidth();
        }

        const float32 iconCountX = std::floor(width / (_cellSize._x + _minCellSpacing));
        const float32 noneContentWidth = width - (iconCountX * _cellSize._x);
        const float32 cellSpacing = std::max(_minCellSpacing, noneContentWidth / (iconCountX + 1.0f));

        iaRectanglef clientRect(0, 0, _cellSize._x, _cellSize._y);
        float32 x = cellSpacing;
        float32 y = _minCellSpacing;

        for (const auto child : children)
        {
            clientRect.setX(x);
            clientRect.setY(y);

            offsets.push_back(clientRect);

            if (x + cellSpacing + _cellSize._x * 2.0 > width)
            {
                x = cellSpacing;
                y += _cellSize._y + _minCellSpacing;
            }
            else
            {
                x += _cellSize._x + cellSpacing;
            }
        }
    }

} // namespace igor