// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetScroll.h>

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iMaterialResourceFactory.h>
#include <iWidgetUserControl.h>
#include <iMaterial.h>
#include <iRenderer.h>

#include <iaConsole.h>
#include <iaColor4.h>
#include <iaMatrix.h>
using namespace IgorAux;

namespace Igor
{

    iWidgetScroll::iWidgetScroll()
        : iWidget(iWidgetType::Scroll)
    {
    }

    void iWidgetScroll::handleMouseMove(int32 x, int32 y)
    {
        if (!isActive())
        {
            return;
        }

        iWidget::handleMouseMove(x, y);

        if (!_widgets.empty())
        {
            iWidget* widget = _widgets[0];
            if (isMouseOver() ||
                widget->isMouseOver())
            {
                x -= _posx;
                y -= _posy;

                widget->handleMouseMove(x - static_cast<int32>(_translate._pos._x), y - static_cast<int32>(_translate._pos._y));
            }
        }
    }

    void iWidgetScroll::setHorrizontalScroll(float32 value)
    {
        con_assert(value >= 0.0f && value <= 1.0f, "out od range");
        _hscroll = value;
        update();
    }

    void iWidgetScroll::setVerticalScroll(float32 value)
    {
        con_assert(value >= 0.0f && value <= 1.0f, "out od range");
        _vscroll = value;
        update();
    }

    bool iWidgetScroll::handleMouseWheel(int32 d)
    {
        if (!isActive())
        {
            return false;
        }

        if (!_widgets.empty())
        {
            iWidget* widget = _widgets[0];
            if (widget->handleMouseWheel(d))
            {
                return true;
            }
        }

        if (_vscrollActive)
        {
            _vscroll -= d / 10.0f;
            if (_vscroll < 0.0f)
            {
                _vscroll = 0.0f;
            }

            if (_vscroll > 1.0f)
            {
                _vscroll = 1.0f;
            }

            update();
            return true;
        }
        else if (_hscrollActive)
        {
            _hscroll -= d / 10.0f;
            if (_hscroll < 0.0f)
            {
                _hscroll = 0.0f;
            }

            if (_hscroll > 1.0f)
            {
                _hscroll = 1.0f;
            }

            update();
            return true;
        }
        
        return false;
    }

    void iWidgetScroll::setWidth(int32 width)
    {
        _width = width;
    }

    void iWidgetScroll::setHeight(int32 height)
    {
        _height = height;
    }

    int32 iWidgetScroll::getScollbarWidth()
    {
        return _scrollbarWidth;
    }

    void iWidgetScroll::setScollbarWidth(int32 width)
    {
        _scrollbarWidth = width;
    }

    void iWidgetScroll::calcScrollButtons()
    {
        int32 scrollspace;

        iWidget* widget = _widgets[0];

        int32 childWidth = widget->getWidth();
        int32 childHeight = widget->getHeight();

        if (_vscrollActive) // v scrollbar
        {
            // v scroll button
            if (_hscrollActive)
            {
                scrollspace = _height - 6 - _scrollbarWidth * 3;
            }
            else
            {
                scrollspace = _height - 6 - _scrollbarWidth * 2;
            }

            _vscrollButton.setHeight(((float32)_height / childHeight) * scrollspace);
            _vscrollButton.setY((scrollspace - _vscrollButton.getHeight()) * _vscroll + 3 + _scrollbarWidth);
            _vscrollButton.setX(_width - _scrollbarWidth - 2);
            _vscrollButton.setWidth(_scrollbarWidth);
        }

        if (_hscrollActive) // h scrollbar
        {
            // h scroll button
            if (_vscrollActive)
            {
                scrollspace = _width - 6 - _scrollbarWidth * 3;
            }
            else
            {
                scrollspace = _width - 6 - _scrollbarWidth * 2;
            }

            _hscrollButton.setWidth(((float32)_width / childWidth) * scrollspace);
            _hscrollButton.setX((scrollspace - _hscrollButton.getWidth()) * _hscroll + 4 + _scrollbarWidth);
            _hscrollButton.setY(_height - _scrollbarWidth - 2);
            _hscrollButton.setHeight(_scrollbarWidth);
        }
    }

    void iWidgetScroll::update()
    {
        if (!_widgets.empty())
        {
            iWidget* widget = _widgets[0];

            int32 childWidth = widget->getWidth();
            int32 childHeight = widget->getHeight();

            if (_width - 4 < childWidth) // h scrollbar
            {
                _hscrollActive = true;
            }
            else
            {
                _hscrollActive = false;
            }

            if (_height - 4 < childHeight) // v scrollbar
            {
                _vscrollActive = true;
            }
            else
            {
                _vscrollActive = false;
            }

            calcScrollButtons();
            calcChildFrame();
            calcTranslation();

            /* TODO
            up_button;
                down_button;
                left_button;
                right_button;*/
        }

        updateParent();
    }

    void iWidgetScroll::calcTranslation()
    {
        _translate.identity();
        _translate.translate(0, 0, -30);

        iWidget* widget = _widgets[0];

        if (_hscrollActive)
        {
            _translate.translate(-_hscroll * (widget->getWidth() - _width + 4), 0, 0);
        }

        if (_vscrollActive)
        {
            _translate.translate(0, -_vscroll * (widget->getHeight() - _height + 4), 0);
        }
    }

    void iWidgetScroll::calcChildFrame()
    {
        if (_vscrollActive && _hscrollActive) // hv scrollbars
        {
            _childFrame.setX(2);
            _childFrame.setY(2);
            _childFrame.setWidth(_width - 4 - _scrollbarWidth);
            _childFrame.setHeight(_height - 4 - _scrollbarWidth);
        }
        else if (_hscrollActive)
        {
            _childFrame.setX(2);
            _childFrame.setY(2);
            _childFrame.setWidth(_width - 4);
            _childFrame.setHeight(_height - 4 - _scrollbarWidth);
        }
        else if (_vscrollActive)
        {
            _childFrame.setX(2);
            _childFrame.setY(2);
            _childFrame.setWidth(_width - 4 - _scrollbarWidth);
            _childFrame.setHeight(_height - 4);
        }
        else // no scrollbars
        {
            _childFrame.setX(2);
            _childFrame.setY(2);
            _childFrame.setWidth(_width - 4);
            _childFrame.setHeight(_height - 4);
        }
    }

    void iWidgetScroll::draw()
    {
        if (isVisible())
        {
            // begin rendering
            iWidgetManager::getInstance().getTheme()->drawBackgroundFrame(_posx, _posy, _width, _height, _widgetAppearanceState, isActive());

            iaColor4f dark(0.3f, 0.3f, 0.3f, 1.0f);

            // render scrollbars
            if (_vscrollActive && _hscrollActive) // hv scrollbars
            {
                iRenderer::getInstance().setColor(dark);
                iRenderer::getInstance().drawRectangle(_posx + _width - _scrollbarWidth - 2, _posy + 2, _scrollbarWidth, _height - 4);
                iRenderer::getInstance().drawRectangle(_posx + 1, _posy + _height - _scrollbarWidth - 2, _width - 3, _scrollbarWidth); // don't ask about the coordinates -.-

                // left button
                iWidgetManager::getInstance().getTheme()->drawButton(_posx + 2, _posy + _height - _scrollbarWidth - 2, _scrollbarWidth, _scrollbarWidth, "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
                // TODO pic

                // right button
                iWidgetManager::getInstance().getTheme()->drawButton(_posx + _width - _scrollbarWidth * 2 - 2, _posy + _height - _scrollbarWidth - 2, _scrollbarWidth, _scrollbarWidth, "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
                // TODO pic

                // h scroll button
                iWidgetManager::getInstance().getTheme()->drawButton(_posx + _hscrollButton.getX(), _posy + _hscrollButton.getY(),
                    _hscrollButton.getWidth(), _hscrollButton.getHeight(), "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());

                // top button
                iWidgetManager::getInstance().getTheme()->drawButton(_posx + _width - _scrollbarWidth - 2, _posy + 2, _scrollbarWidth, _scrollbarWidth, "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
                // TODO pic

                // bottom button
                iWidgetManager::getInstance().getTheme()->drawButton(_posx + _width - _scrollbarWidth - 2, _posy + _height - 2 - _scrollbarWidth * 2, _scrollbarWidth,
                    _scrollbarWidth, "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
                // TODO pic

                // v scroll button
                iWidgetManager::getInstance().getTheme()->drawButton(_posx + _vscrollButton.getX(), _posy + _vscrollButton.getY(), _vscrollButton.getWidth(),
                    _vscrollButton.getHeight(), "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
            }
            else if (_hscrollActive) // h scrollbar
            {
                iRenderer::getInstance().setColor(dark);
                iRenderer::getInstance().drawRectangle(_posx + 1, _posy + _height - _scrollbarWidth - 2, _width - 3, _scrollbarWidth); // don't ask about the coordinates -.-

                // left button
                iWidgetManager::getInstance().getTheme()->drawButton(_posx + 2, _posy + _height - _scrollbarWidth - 2, _scrollbarWidth, _scrollbarWidth, "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
                // TODO pic

                // right button
                iWidgetManager::getInstance().getTheme()->drawButton(_posx + _width - _scrollbarWidth - 2, _posy + _height - _scrollbarWidth - 2, _scrollbarWidth, _scrollbarWidth, "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
                // TODO pic

                // h scroll button
                iWidgetManager::getInstance().getTheme()->drawButton(_posx + _hscrollButton.getX(), _posy + _hscrollButton.getY(),
                    _hscrollButton.getWidth(), _hscrollButton.getHeight(), "", iHorrizontalAlignment::Center, iVerticalAlignment::Center,
                    0, iWidgetAppearanceState::Standby, isActive());
            }
            else if (_vscrollActive) // v scrollbar
            {
                iRenderer::getInstance().setColor(dark);
                iRenderer::getInstance().drawRectangle(_posx + _width - _scrollbarWidth - 2, _posy + 2, _scrollbarWidth, _height - 4);

                // top button
                iWidgetManager::getInstance().getTheme()->drawButton(_posx + _width - _scrollbarWidth - 2, _posy + 2, _scrollbarWidth, _scrollbarWidth, "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
                // TODO pic

                // bottom button
                iWidgetManager::getInstance().getTheme()->drawButton(_posx + _width - _scrollbarWidth - 2, _posy + _height - 2 - _scrollbarWidth, _scrollbarWidth,
                    _scrollbarWidth, "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
                // TODO pic

                // v scroll button
                iWidgetManager::getInstance().getTheme()->drawButton(_posx + _vscrollButton.getX(), _posy + _vscrollButton.getY(), _vscrollButton.getWidth(),
                    _vscrollButton.getHeight(), "", iHorrizontalAlignment::Center, iVerticalAlignment::Center, 0, iWidgetAppearanceState::Standby, isActive());
            }

            //render child
            iRectanglei absoluteFramePos(_posx + _childFrame.getX(), _posy + _childFrame.getY(), _childFrame.getWidth(), _childFrame.getHeight());

            // fake the parent position [0,0] because of different coordsystem
            if (!_widgets.empty())
            {
                iWidget* widget = _widgets[0];
                
                widget->calcPosition(0, 0, absoluteFramePos.getWidth(), absoluteFramePos.getHeight());

                iRectanglei viewport;
                iaMatrixf projectionMatrix;
                iaMatrixf modelMatrix;

                iRenderer::getInstance().getViewport(viewport);
                iRenderer::getInstance().getProjectionMatrix(projectionMatrix);
                iRenderer::getInstance().getModelMatrix(modelMatrix);

                iRenderer::getInstance().setViewport(absoluteFramePos.getX(), iWidgetManager::getInstance().getDesktopHeight() - absoluteFramePos.getY() - absoluteFramePos.getHeight(), absoluteFramePos.getWidth(), absoluteFramePos.getHeight());
                iRenderer::getInstance().setOrtho(0, absoluteFramePos.getWidth(), absoluteFramePos.getHeight(), 0, 1, 40);
                iRenderer::getInstance().setModelMatrix(_translate);

                widget->draw();

                iRenderer::getInstance().setModelMatrix(modelMatrix);
                iRenderer::getInstance().setProjectionMatrix(projectionMatrix);
                iRenderer::getInstance().setViewport(viewport.getX(), viewport.getY(), viewport.getWidth(), viewport.getHeight());
            }
        }
    }

}