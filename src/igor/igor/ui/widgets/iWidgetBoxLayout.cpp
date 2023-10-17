// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetBoxLayout.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>

namespace igor
{
    iWidgetBoxLayout::iWidgetBoxLayout(iWidgetBoxLayoutType layoutType, const iWidgetPtr parent)
        : iWidget(iWidgetType::iWidgetBoxLayout, iWidgetKind::Widget, parent), _layoutType(layoutType)
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
            _children.empty())
        {
            setMinSize(minWidth, minHeight);
            return;
        }

        if (_layoutType == iWidgetBoxLayoutType::Vertical)
        {
            for (const auto &child : getChildren())
            {
                if (child->getMinWidth() > minWidth)
                {
                    minWidth = child->getMinWidth();
                }

                minHeight += child->getMinHeight();
            }
        }
        else
        {
            for (const auto &child : getChildren())
            {
                if (child->getMinHeight() > minHeight)
                {
                    minHeight = child->getMinHeight();
                }

                minWidth += child->getMinWidth();
            }
        }

        if (getConfiguredWidth() > minWidth)
        {
            minWidth = getConfiguredWidth();
        }

        if (getConfiguredHeight() > minHeight)
        {
            minHeight = getConfiguredHeight();
        }

        setClientArea(0, 0, 0, 0);
        setMinSize(minWidth, minHeight);

        con_endl("iWidgetBoxLayout::calcMinSize " << getID() << " " << _layoutType << " " << minWidth << " " << minHeight);
    }

    void iWidgetBoxLayout::calcChildOffsets(std::vector<iaRectanglef> &offsets)
    {
        if (_children.empty())
        {
            return;
        }

        iaRectanglef clientRect;
        float32 offsetPos = 0;
        uint32 index = 0;

        con_endl("iWidgetBoxLayout::calcChildOffsets " << getID() << " " << _layoutType);

        for (const auto child : getChildren())
        {
            if (_layoutType == iWidgetBoxLayoutType::Vertical)
            {
                clientRect.setX(0);
                clientRect.setY(offsetPos);
                offsetPos += child->getMinHeight();
            }
            else
            {
                clientRect.setX(offsetPos);
                clientRect.setY(0);
                offsetPos += child->getMinWidth();
            }

            clientRect.setWidth(child->getMinWidth());
            clientRect.setHeight(child->getMinHeight());

            con_endl(clientRect);

            offsets.push_back(clientRect);
        }
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