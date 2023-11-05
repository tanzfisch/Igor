// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetBoxLayout.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>

namespace igor
{
    iWidgetBoxLayout::iWidgetBoxLayout(iWidgetBoxLayoutType layoutType, const iWidgetPtr parent)
        : iWidget(iWidgetType::iWidgetBoxLayout, iWidgetKind::Layout, parent), _layoutType(layoutType)
    {
        setVerticalAlignment(iVerticalAlignment::Top);
        setHorizontalAlignment(iHorizontalAlignment::Left);
    }

    void iWidgetBoxLayout::addWidget(iWidgetPtr widget)
    {
        iWidget::addWidget(widget);
    }

    void iWidgetBoxLayout::removeWidget(iWidgetPtr widget)
    {
        iWidget::removeWidget(widget);
    }

    void iWidgetBoxLayout::calcMinSize()
    {
        int32 minWidth = 0;
        int32 minHeight = 0;

        if (!isGrowingByContent() ||
            getChildren().empty())
        {
            setMinSize(minWidth, minHeight);
            return;
        }

        for (const auto child : getChildren())
        {
            if (_layoutType == iWidgetBoxLayoutType::Vertical)
            {
                minWidth = std::max(minWidth, child->getMinWidth());
                minHeight += child->getMinHeight();
            }
            else
            {
                minHeight = std::max(minHeight, child->getMinHeight());
                minWidth += child->getMinWidth();
            }
        }

        minWidth = std::max(minWidth, getConfiguredWidth());
        minHeight = std::max(minHeight, getConfiguredHeight());

        setClientArea(0, 0, 0, 0);
        setMinSize(minWidth, minHeight);
    }

    void iWidgetBoxLayout::calcChildOffsets(std::vector<iaRectanglef> &offsets)
    {
        if (getID() == 2)
        {
            int x = 0;
        }

        if (_children.empty())
        {
            return;
        }

        iaRectanglef clientRect;
        float32 offsetPos = 0;
        uint32 index = 0;

        auto &children = getChildren();

        for (auto child : children)
        {
            if (_layoutType == iWidgetBoxLayoutType::Vertical)
            {
                clientRect.setX(0);
                clientRect.setY(offsetPos);
                clientRect.setWidth(getActualWidth());
                clientRect.setHeight(child->getMinHeight());

                offsetPos += child->getMinHeight();
            }
            else
            {
                clientRect.setX(offsetPos);
                clientRect.setY(0);
                clientRect.setWidth(child->getMinWidth());
                clientRect.setHeight(getActualHeight());

                offsetPos += child->getMinWidth();
            }

            offsets.push_back(clientRect);
        }

        if (getVerticalAlignment() == iVerticalAlignment::Stretch &&
            _layoutType == iWidgetBoxLayoutType::Vertical &&
            _stretchIndex >= 0 && _stretchIndex < children.size())
        {
            offsets[_stretchIndex].adjust(0, 0, 0, getActualHeight() - offsetPos);
        }

        if (getHorizontalAlignment() == iHorizontalAlignment::Stretch &&
            _layoutType == iWidgetBoxLayoutType::Horizontal &&
            _stretchIndex >= 0 && _stretchIndex < children.size())
        {
            offsets[_stretchIndex].adjust(0, 0, getActualWidth() - offsetPos, 0);
        }
    }

    void iWidgetBoxLayout::setStretchIndex(int32 index)
    {
        _stretchIndex = index;
    }

    int32 iWidgetBoxLayout::getStretchIndex() const
    {
        return _stretchIndex;
    }

    void iWidgetBoxLayout::draw()
    {
        if (!isVisible())
        {
            return;
        }

        iWidgetManager::getInstance().getTheme()->drawGridCell(getActualRect(), getState());

        // TODO?

        for (const auto child : _children)
        {
            child->draw();
        }
    }

    std::wostream &operator<<(std::wostream &stream, const iWidgetBoxLayoutType &type)
    {
        const static std::wstring text[] = {
            L"Vertical",
            L"Horizontal"};

        stream << text[static_cast<int>(type)];

        return stream;
    }

} // namespace igor