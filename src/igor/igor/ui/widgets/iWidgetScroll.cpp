// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetScroll.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/ui/user_controls/iUserControl.h>
#include <igor/resources/shader_material/iShaderMaterial.h>
#include <igor/renderer/iRenderer.h>
#include <igor/data/iIntersection.h>
#include <igor/resources/iResourceManager.h>

#include <iaux/system/iaConsole.h>
#include <iaux/data/iaColor4.h>
#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{

    static const int32 BORDER_WIDTH = 2;
    static const int32 BORDER_WIDTH2 = 4;
    static const float32 SCROLL_STEPPING = 90.0f;

    iWidgetScroll::iWidgetScroll(const iWidgetPtr parent)
        : iWidget(iWidgetType::iWidgetScroll, iWidgetKind::Widget, parent)
    {
        setHorizontalAlignment(iHorizontalAlignment::Stretch);
        setVerticalAlignment(iVerticalAlignment::Stretch);
        setGrowingByContent(false);

        // get some resources
        _upTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_up");
        _downTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_down");
        _leftTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_left");
        _rightTexture = iResourceManager::getInstance().loadResource<iTexture>("igor_icon_right");
    }

    bool iWidgetScroll::onMouseKeyUp(iEventMouseKeyUp &event)
    {
        if (!isEnabled())
        {
            return false;
        }

        if (event.getKey() == iKeyCode::MouseLeft)
        {
            if (_hscrollButton._mouseOver)
            {
                _hscrollButton._appearanceState = iWidgetState::Clicked;
                _hscrollButton._mouseDown = false;
                return true;
            }

            if (_vscrollButton._mouseOver)
            {
                _vscrollButton._appearanceState = iWidgetState::Clicked;
                _vscrollButton._mouseDown = false;
                return true;
            }

            if (_leftButton._mouseOver)
            {
                _leftButton._appearanceState = iWidgetState::Clicked;
                _leftButton._mouseDown = false;
                return true;
            }

            if (_rightButton._mouseOver)
            {
                _rightButton._appearanceState = iWidgetState::Clicked;
                _rightButton._mouseDown = false;
                return true;
            }

            if (_upButton._mouseOver)
            {
                _upButton._appearanceState = iWidgetState::Clicked;
                _upButton._mouseDown = false;
                return true;
            }

            if (_downButton._mouseOver)
            {
                _downButton._appearanceState = iWidgetState::Clicked;
                _downButton._mouseDown = false;
                return true;
            }
        }

        return iWidget::onMouseKeyUp(event);
    }

    bool iWidgetScroll::onMouseDoubleClick(iEventMouseKeyDoubleClick &event)
    {
        if (!isEnabled() ||
            !isMouseOver())
        {
            return false;
        }

        if (event.getKey() == iKeyCode::MouseLeft)
        {
            if (handleButtonClicks())
            {
                return true;
            }
        }

        return iWidget::onMouseDoubleClick(event);
    }

    bool iWidgetScroll::onMouseKeyDown(iEventMouseKeyDown &event)
    {
        if (!isEnabled())
        {
            return false;
        }

        if (event.getKey() == iKeyCode::MouseLeft)
        {
            if (_hscrollButton._mouseOver)
            {
                _hscrollButton._appearanceState = iWidgetState::Pressed;
                _hscrollButton._mouseDown = true;
                return true;
            }

            if (_vscrollButton._mouseOver)
            {
                _vscrollButton._appearanceState = iWidgetState::Pressed;
                _vscrollButton._mouseDown = true;
                return true;
            }

            if (handleButtonClicks())
            {
                return true;
            }
        }

        return iWidget::onMouseKeyDown(event);
    }

    bool iWidgetScroll::handleButtonClicks()
    {
        if (_children.empty())
        {
            return false;
        }

        auto child = _children.front();
        if (child == nullptr)
        {
            return false;
        }

        if (_leftButton._mouseOver)
        {
            _leftButton._appearanceState = iWidgetState::Pressed;
            _leftButton._mouseDown = true;

            _hscroll -= 1.0f / (child->getActualWidth() / SCROLL_STEPPING);
            _hscroll = std::min(0.0f, _hscroll);

            return true;
        }

        if (_rightButton._mouseOver)
        {
            _rightButton._appearanceState = iWidgetState::Pressed;
            _rightButton._mouseDown = true;

            _hscroll += 1.0f / (child->getActualWidth() / SCROLL_STEPPING);
            _hscroll = std::max(1.0f, _hscroll);

            return true;
        }

        if (_upButton._mouseOver)
        {
            _upButton._appearanceState = iWidgetState::Pressed;
            _upButton._mouseDown = true;

            _vscroll -= 1.0f / (child->getActualHeight() / SCROLL_STEPPING);
            _vscroll = std::max(0.0f, _vscroll);

            return true;
        }

        if (_downButton._mouseOver)
        {
            _downButton._appearanceState = iWidgetState::Pressed;
            _downButton._mouseDown = true;

            _vscroll += 1.0f / (child->getActualHeight() / SCROLL_STEPPING);
            _vscroll = std::min(1.0f, _vscroll);

            return true;
        }

        return false;
    }

    void iWidgetScroll::onMouseMove(iEventMouseMove &event)
    {
        if (!isEnabled() || _children.empty())
        {
            return;
        }

        auto child = _children.front();
        if (child == nullptr)
        {
            return;
        }

        const auto &pos = event.getPosition();
        child->onMouseMove(event);

        if (_hscrollButton._mouseDown)
        {
            if (!_children.empty())
            {
                _hscroll += static_cast<float32>(iMouse::getInstance().getPosDelta()._x) / static_cast<float32>(calcHorizontalScrollSpace() - _hscrollButton._rectangle._width);
                _hscroll = std::max(0.0f, std::min(1.0f, _hscroll));
            }
        }

        if (_vscrollButton._mouseDown)
        {
            if (!_children.empty())
            {
                _vscroll += static_cast<float32>(iMouse::getInstance().getPosDelta()._y) / static_cast<float32>(calcVerticalScrollSpace() - _vscrollButton._rectangle._height);
                _vscroll = std::max(0.0f, std::min(1.0f, _vscroll));
            }
        }

        auto rect = getActualRect();
        if (iIntersection::intersects(pos, rect) &&
            !event.isConsumed())
        {
            if (!_isMouseOver)
            {
                _widgetState = iWidgetState::Highlighted;
                _mouseOver(this);
            }

            _isMouseOver = true;

            if (!_hscrollButton._mouseDown)
            {
                if (iIntersection::intersects(pos, _hscrollButton._rectangle))
                {
                    _hscrollButton._appearanceState = iWidgetState::Highlighted;
                    _hscrollButton._mouseOver = true;
                }
                else
                {
                    _hscrollButton._appearanceState = iWidgetState::Standby;
                    _hscrollButton._mouseOver = false;
                }
            }

            if (!_vscrollButton._mouseDown)
            {
                if (iIntersection::intersects(pos, _vscrollButton._rectangle))
                {
                    _vscrollButton._appearanceState = iWidgetState::Highlighted;
                    _vscrollButton._mouseOver = true;
                }
                else
                {
                    _vscrollButton._appearanceState = iWidgetState::Standby;
                    _vscrollButton._mouseOver = false;
                }
            }

            if (iIntersection::intersects(pos, _leftButton._rectangle))
            {
                _leftButton._appearanceState = iWidgetState::Highlighted;
                _leftButton._mouseOver = true;
            }
            else
            {
                _leftButton._appearanceState = iWidgetState::Standby;
                _leftButton._mouseOver = false;
            }

            if (iIntersection::intersects(pos, _rightButton._rectangle))
            {
                _rightButton._appearanceState = iWidgetState::Highlighted;
                _rightButton._mouseOver = true;
            }
            else
            {
                _rightButton._appearanceState = iWidgetState::Standby;
                _rightButton._mouseOver = false;
            }

            if (iIntersection::intersects(pos, _upButton._rectangle))
            {
                _upButton._appearanceState = iWidgetState::Highlighted;
                _upButton._mouseOver = true;
            }
            else
            {
                _upButton._appearanceState = iWidgetState::Standby;
                _upButton._mouseOver = false;
            }

            if (iIntersection::intersects(pos, _downButton._rectangle))
            {
                _downButton._appearanceState = iWidgetState::Highlighted;
                _downButton._mouseOver = true;
            }
            else
            {
                _downButton._appearanceState = iWidgetState::Standby;
                _downButton._mouseOver = false;
            }
        }
        else
        {
            if (_isMouseOver)
            {
                _widgetState = iWidgetState::Standby;

                _leftButton._appearanceState = iWidgetState::Standby;
                _leftButton._mouseOver = false;
                _rightButton._appearanceState = iWidgetState::Standby;
                _rightButton._mouseOver = false;
                _upButton._appearanceState = iWidgetState::Standby;
                _upButton._mouseOver = false;
                _downButton._appearanceState = iWidgetState::Standby;
                _downButton._mouseOver = false;

                _mouseOff(this);
            }

            _isMouseOver = false;
        }
    }

    void iWidgetScroll::setHorizontalScroll(float32 value)
    {
        _hscroll = std::max(0.0f, std::min(1.0f, value));
    }

    void iWidgetScroll::setVerticalScroll(float32 value)
    {
        _vscroll = std::max(0.0f, std::min(1.0f, value));
    }

    bool iWidgetScroll::onMouseWheel(iEventMouseWheel &event)
    {
        if (!isEnabled() ||
            !isMouseOver() ||
            getChildren().empty())
        {
            return false;
        }

        if (iWidget::onMouseWheel(event) &&
            !_ignoreChildEventConsumption)
        {
            return true;
        }

        auto child = getChildren().front();
        con_assert(child != nullptr, "internal error");

        if (_vscrollActive)
        {
            _vscroll -= event.getWheelDelta() * (1.0f / (child->getActualHeight() / SCROLL_STEPPING));
            _vscroll = std::max(0.0f, std::min(1.0f, _vscroll));

            return true;
        }
        else if (_hscrollActive)
        {
            _hscroll -= event.getWheelDelta() * (1.0f / (child->getActualWidth() / SCROLL_STEPPING));
            _hscroll = std::max(0.0f, std::min(1.0f, _hscroll));

            return true;
        }

        return false;
    }

    int32 iWidgetScroll::getScollbarWidth()
    {
        return _scrollbarWidth;
    }

    void iWidgetScroll::setScollbarWidth(int32 width)
    {
        _scrollbarWidth = width;
    }

    void iWidgetScroll::calcMinSize()
    {
        int32 minWidth = 0;
        int32 minHeight = 0;

        if (_vscrollActive && _hscrollActive) // hv scrollbars
        {
            minWidth = BORDER_WIDTH2 + _scrollbarWidth;
            minHeight = BORDER_WIDTH2 + _scrollbarWidth;
        }
        else if (_hscrollActive)
        {
            minWidth = BORDER_WIDTH2;
            minHeight = BORDER_WIDTH2 + _scrollbarWidth;
        }
        else if (_vscrollActive)
        {
            minWidth = BORDER_WIDTH2 + _scrollbarWidth;
            minHeight = BORDER_WIDTH2;
        }
        else // no scrollbars
        {
            minWidth = BORDER_WIDTH2;
            minHeight = BORDER_WIDTH2;
        }

        updateMinSize(minWidth, minHeight);
    }

    void iWidgetScroll::calcButtons()
    {
        if (_children.empty())
        {
            return;
        }

        const auto child = _children.front();
        if (child == nullptr)
        {
            return;
        }

        int32 childWidth = child->getActualWidth();
        int32 childHeight = child->getActualHeight();

        if (_vscrollActive) // v scrollbar
        {
            int32 scrollspace = calcVerticalScrollSpace();

            _vscrollButton._rectangle.setHeight(static_cast<int32>((static_cast<float32>(getActualHeight()) / static_cast<float32>(childHeight)) * static_cast<float32>(scrollspace)));
            _vscrollButton._rectangle.setX(getActualPosX() + getActualWidth() - _scrollbarWidth - 2);
            _vscrollButton._rectangle.setY(getActualPosY() + static_cast<int32>(static_cast<float32>(scrollspace - _vscrollButton._rectangle.getHeight()) * _vscroll + 3.0f + static_cast<float32>(_scrollbarWidth)));
            _vscrollButton._rectangle.setWidth(_scrollbarWidth);
        }

        if (_hscrollActive) // h scrollbar
        {
            int32 scrollspace = calcHorizontalScrollSpace();

            _hscrollButton._rectangle.setWidth(static_cast<int32>((static_cast<float32>(getActualWidth()) / static_cast<float32>(childWidth)) * static_cast<float32>(scrollspace)));
            _hscrollButton._rectangle.setX(getActualPosX() + static_cast<int32>(static_cast<float32>(scrollspace - _hscrollButton._rectangle.getWidth()) * _hscroll + 4.0f + static_cast<float32>(_scrollbarWidth)));
            _hscrollButton._rectangle.setY(getActualPosY() + getActualHeight() - _scrollbarWidth - 2);
            _hscrollButton._rectangle.setHeight(_scrollbarWidth);
        }

        _leftButton._rectangle.set(getActualPosX() + 2, getActualPosY() + getActualHeight() - _scrollbarWidth - 2, _scrollbarWidth, _scrollbarWidth);
        _upButton._rectangle.set(getActualPosX() + getActualWidth() - _scrollbarWidth - 2, getActualPosY() + 2, _scrollbarWidth, _scrollbarWidth);

        if (_vscrollActive && _hscrollActive)
        {
            _rightButton._rectangle.set(getActualPosX() + getActualWidth() - _scrollbarWidth * 2 - 2, getActualPosY() + getActualHeight() - _scrollbarWidth - 2, _scrollbarWidth, _scrollbarWidth);
            _downButton._rectangle.set(getActualPosX() + getActualWidth() - _scrollbarWidth - 2, getActualPosY() + getActualHeight() - 2 - _scrollbarWidth * 2, _scrollbarWidth, _scrollbarWidth);
        }
        else if (_hscrollActive)
        {
            _rightButton._rectangle.set(getActualPosX() + getActualWidth() - _scrollbarWidth - 2, getActualPosY() + getActualHeight() - _scrollbarWidth - 2, _scrollbarWidth, _scrollbarWidth);
        }
        else if (_vscrollActive)
        {
            _downButton._rectangle.set(getActualPosX() + getActualWidth() - _scrollbarWidth - 2, getActualPosY() + getActualHeight() - 2 - _scrollbarWidth, _scrollbarWidth, _scrollbarWidth);
        }
    }

    int32 iWidgetScroll::calcHorizontalScrollSpace() const
    {
        if (_vscrollActive)
        {
            return getActualWidth() - 6 - _scrollbarWidth * 3;
        }
        else
        {
            return getActualWidth() - 6 - _scrollbarWidth * 2;
        }
    }

    int32 iWidgetScroll::calcVerticalScrollSpace() const
    {
        if (_hscrollActive)
        {
            return getActualHeight() - 6 - _scrollbarWidth * 3;
        }
        else
        {
            return getActualHeight() - 6 - _scrollbarWidth * 2;
        }
    }

    void iWidgetScroll::calcChildOffsets(std::vector<iaRectanglef> &offsets)
    {
        offsets.clear();

        if (_children.empty())
        {
            return;
        }

        offsets.resize(_children.size());

        const auto child = _children.front();
        if (child == nullptr)
        {
            return;
        }

        iaRectanglef clientRect;

        float32 offsetX = 0;
        float32 offsetY = 0;

        if (_hscrollActive)
        {
            offsetX = _hscroll * (child->getConfiguredMinWidth() - getActualWidth() + 4);
        }

        if (_vscrollActive)
        {
            offsetY = _vscroll * (child->getConfiguredMinHeight() - getActualHeight() + 4);
        }

        clientRect.setX(offsetX);
        clientRect.setY(offsetY);
        clientRect.setWidth(getActualWidth());
        clientRect.setHeight(getActualHeight());

        offsets[0] = clientRect;
    }

    void iWidgetScroll::updateAlignment(int32 clientWidth, int32 clientHeight)
    {
        iWidget::updateAlignment(clientWidth, clientHeight);
    }

    void iWidgetScroll::calcChildFrame()
    {
        if (_vscrollActive && _hscrollActive) // hv scrollbars
        {
            _childFrame.setX(2);
            _childFrame.setY(2);
            _childFrame.setWidth(getActualWidth() - BORDER_WIDTH2 - _scrollbarWidth);
            _childFrame.setHeight(getActualHeight() - BORDER_WIDTH2 - _scrollbarWidth);
        }
        else if (_hscrollActive)
        {
            _childFrame.setX(2);
            _childFrame.setY(2);
            _childFrame.setWidth(getActualWidth() - BORDER_WIDTH2);
            _childFrame.setHeight(getActualHeight() - BORDER_WIDTH2 - _scrollbarWidth);
        }
        else if (_vscrollActive)
        {
            _childFrame.setX(2);
            _childFrame.setY(2);
            _childFrame.setWidth(getActualWidth() - BORDER_WIDTH2 - _scrollbarWidth);
            _childFrame.setHeight(getActualHeight() - BORDER_WIDTH2);
        }
        else // no scrollbars
        {
            _childFrame.setX(2);
            _childFrame.setY(2);
            _childFrame.setWidth(getActualWidth() - BORDER_WIDTH2);
            _childFrame.setHeight(getActualHeight() - BORDER_WIDTH2);
        }
    }

    void iWidgetScroll::addWidget(iWidgetPtr widget)
    {
        if (!getChildren().empty())
        {
            con_warn("undefined behaviour if adding more then one widget");
        }

        iWidget::addWidget(widget);

        if (widget->getVerticalAlignment() != iVerticalAlignment::Top ||
            widget->getHorizontalAlignment() != iHorizontalAlignment::Left)
        {
            // only top left alignment is supported for children of iWidgetScroll. Changing it for you
            widget->setVerticalAlignment(iVerticalAlignment::Top);
            widget->setHorizontalAlignment(iHorizontalAlignment::Left);
        }
    }

    void iWidgetScroll::draw()
    {
        if (!isVisible() ||
            _children.empty())
        {
            return;
        }

        const auto child = _children.front();
        if (child == nullptr)
        {
            return;
        }

        con_assert(child->getVerticalAlignment() == iVerticalAlignment::Top && child->getHorizontalAlignment() == iHorizontalAlignment::Left, "only top left alignment is supported for children of iWidgetScroll");

        int32 childWidth = child->getMinWidth();
        int32 childHeight = child->getMinHeight();

        if (getActualWidth() - BORDER_WIDTH2 < childWidth) // h scrollbar
        {
            _hscrollActive = true;
        }
        else
        {
            _hscrollActive = false;
        }

        if (getActualHeight() - BORDER_WIDTH2 < childHeight) // v scrollbar
        {
            _vscrollActive = true;
        }
        else
        {
            _vscrollActive = false;
        }

        calcChildFrame();
        calcButtons();

        // begin rendering
        iWidgetManager::getInstance().getTheme()->drawBackgroundFrame(getActualRect(), _widgetState, isEnabled());

        iaColor4f dark(0.3f, 0.3f, 0.3f, 1.0f);

        // render scroll bars
        if (_vscrollActive && _hscrollActive) // hv scroll bars
        {
            iRenderer::getInstance().drawFilledRectangle(static_cast<float32>(getActualPosX() + getActualWidth() - _scrollbarWidth - 2), static_cast<float32>(getActualPosY() + 2), static_cast<float32>(_scrollbarWidth), static_cast<float32>(getActualHeight() - 4), dark);
            iRenderer::getInstance().drawFilledRectangle(static_cast<float32>(getActualPosX() + 1), static_cast<float32>(getActualPosY() + getActualHeight() - _scrollbarWidth - 2), static_cast<float32>(getActualWidth() - 3), static_cast<float32>(_scrollbarWidth), dark);

            // left button
            iWidgetManager::getInstance().getTheme()->drawButton(_leftButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, _leftTexture, nullptr, _leftButton._appearanceState, isEnabled(), false);

            // right button
            iWidgetManager::getInstance().getTheme()->drawButton(_rightButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, _rightTexture, nullptr, _rightButton._appearanceState, isEnabled(), false);

            // h scroll button
            iWidgetManager::getInstance().getTheme()->drawButton(_hscrollButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, nullptr, _hscrollButton._appearanceState, isEnabled(), false);

            // up button
            iWidgetManager::getInstance().getTheme()->drawButton(_upButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, _upTexture, nullptr, _upButton._appearanceState, isEnabled(), false);

            // down button
            iWidgetManager::getInstance().getTheme()->drawButton(_downButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, _downTexture, nullptr, _downButton._appearanceState, isEnabled(), false);

            // v scroll button
            iWidgetManager::getInstance().getTheme()->drawButton(_vscrollButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, nullptr, _vscrollButton._appearanceState, isEnabled(), false);
        }
        else if (_hscrollActive) // h scrollbar
        {
            iRenderer::getInstance().drawFilledRectangle(static_cast<float32>(getActualPosX() + 1), static_cast<float32>(getActualPosY() + getActualHeight() - _scrollbarWidth - 2), static_cast<float32>(getActualWidth() - 3), static_cast<float32>(_scrollbarWidth), dark);

            // left button
            iWidgetManager::getInstance().getTheme()->drawButton(_leftButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, _leftTexture, nullptr, _leftButton._appearanceState, isEnabled(), false);

            // right button
            iWidgetManager::getInstance().getTheme()->drawButton(_rightButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, _rightTexture, nullptr, _rightButton._appearanceState, isEnabled(), false);

            // h scroll button
            iWidgetManager::getInstance().getTheme()->drawButton(_hscrollButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, nullptr, _hscrollButton._appearanceState, isEnabled(), false);
        }
        else if (_vscrollActive) // v scrollbar
        {
            iRenderer::getInstance().drawFilledRectangle(static_cast<float32>(getActualPosX() + getActualWidth() - _scrollbarWidth - 2), static_cast<float32>(getActualPosY() + 2), static_cast<float32>(_scrollbarWidth), static_cast<float32>(getActualHeight() - 4), dark);

            // up button
            iWidgetManager::getInstance().getTheme()->drawButton(_upButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, _upTexture, nullptr, _upButton._appearanceState, isEnabled(), false);

            // down button
            iWidgetManager::getInstance().getTheme()->drawButton(_downButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, _downTexture, nullptr, _downButton._appearanceState, isEnabled(), false);

            // v scroll button
            iWidgetManager::getInstance().getTheme()->drawButton(_vscrollButton._rectangle, "", iHorizontalAlignment::Center, iVerticalAlignment::Center, nullptr, nullptr, _vscrollButton._appearanceState, isEnabled(), false);
        }

        // render child
        iaRectanglei absoluteFramePos(getActualPosX() + _childFrame.getX(),
                                      getActualPosY() + _childFrame.getY(),
                                      _childFrame.getWidth(),
                                      _childFrame.getHeight());

        // do some clipping using the viewport

        // store current render states
        const iaRectanglei viewport = iRenderer::getInstance().getViewport();
        const iaMatrixd projectionMatrix = iRenderer::getInstance().getProjectionMatrix();
        const iaMatrixd modelMatrix = iRenderer::getInstance().getModelMatrix();

        iRenderer::getInstance().setViewport(absoluteFramePos.getX(), iWidgetManager::getInstance().getDesktopHeight() - absoluteFramePos.getY() - absoluteFramePos.getHeight(), absoluteFramePos.getWidth(), absoluteFramePos.getHeight());
        iRenderer::getInstance().setOrtho(static_cast<float64>(getActualPosX()),
                                          static_cast<float64>(getActualPosX() + absoluteFramePos.getWidth()),
                                          static_cast<float64>(getActualPosY() + absoluteFramePos.getHeight()),
                                          static_cast<float64>(getActualPosY()), 0.1f, 10.0f);

        iaMatrixd matrix;
        matrix.translate(0.0, 0.0, -1.0);
        iRenderer::getInstance().setModelMatrix(matrix);

        child->draw();

        // restore everything
        iRenderer::getInstance().setModelMatrix(modelMatrix);
        iRenderer::getInstance().setProjectionMatrix(projectionMatrix);
        iRenderer::getInstance().setViewport(viewport);
    }
} // namespace igor
