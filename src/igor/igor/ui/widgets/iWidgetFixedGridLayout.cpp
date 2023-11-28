// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetFixedGridLayout.h>

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

    void iWidgetFixedGridLayout::calcChildOffsets(std::vector<iaRectanglef> &offsets)
    {
        const auto &children = getChildren();
        offsets.reserve(children.size());

        iaRectanglef clientRect(0, 0, _cellSize._x, _cellSize._y);
        float32 x = 0.0f;
        float32 y = 0.0f;
        
        for (const auto child : children)
        {
            clientRect.setX(x);
            clientRect.setY(y);

            offsets.push_back(clientRect);

            y += _cellSize._y;
        }
    }

} // namespace igor