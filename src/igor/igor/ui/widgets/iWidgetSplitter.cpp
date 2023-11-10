
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

namespace igor
{
    iWidgetSplitter::iWidgetSplitter(bool dockingSplitter, const iWidgetPtr parent)
        : iWidget(iWidgetType::iWidgetSplitter, iWidgetKind::Widget, parent)
    {
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

    void iWidgetSplitter::addWidget(iWidgetPtr widget, uint32 index)
    {
        con_assert(getChildren().size() <= 2, "can't add more then two");
        con_assert(index < 2, "index out of range");

        iWidget::addWidget(widget);

        if (index == 0)
        {
            con_assert(_childA == nullptr, "remove first before add a widget");

            _childA = widget;
            _childA->setHorizontalAlignment(iHorizontalAlignment::Stretch);
            _childA->setVerticalAlignment(iVerticalAlignment::Stretch);
        }
        else
        {
            con_assert(_childB == nullptr, "remove first before add a widget");
            _childB = widget;
            _childB->setHorizontalAlignment(iHorizontalAlignment::Stretch);
            _childB->setVerticalAlignment(iVerticalAlignment::Stretch);
        }
    }

    void iWidgetSplitter::addWidget(iWidgetPtr widget)
    {
        con_assert(getChildren().size() <= 2, "can't add more then two");

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

    static const float32 s_splitterWidth = 6;

    void iWidgetSplitter::handleMouseMove(const iaVector2f &pos)
    {
        if (!isEnabled())
        {
            return;
        }

        iaRectanglef clientRect = getActualRect();
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
        }

        if (iIntersection::intersects(pos, clientRect))
        {
            if (!_isMouseOver)
            {
                _widgetState = iWidgetState::Highlighted;
                _mouseOver(this);
            }

            _isMouseOver = true;

            if (!iIntersection::intersects(pos, splitterRect))
            {
                // get copy of children
                std::vector<iWidgetPtr> widgets = getChildren();
                for (auto widget : widgets)
                {
                    widget->handleMouseMove(pos);
                }
            }
        }
        else
        {
            if (_isMouseOver)
            {
                _widgetState = iWidgetState::Standby;
                _mouseOff(this);

                iMouse::getInstance().resetCursorType();
            }

            _isMouseOver = false;
        }

        _posLast = pos;
    }

    void iWidgetSplitter::draw()
    {
        if (isVisible())
        {
            //             iWidgetManager::getInstance().getTheme()->drawButton(getActualRect(), _text, _horizontalTextAlignment, _verticalTextAlignment, _texture, _iconTexture, getState(), isEnabled());
        }
    }

} // namespace igor