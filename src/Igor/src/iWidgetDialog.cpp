// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetDialog.h>

#include <iWidgetBaseTheme.h>
#include <iWidgetManager.h>
#include <iWidgetUserControl.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iWidgetDialog::iWidgetDialog()
        : iWidget(iWidgetType::Dialog)
    {
        setActive(false);
        setVisible(false);
        _width = 100;
        _height = 100;

        setHorrizontalAlignment(iHorrizontalAlignment::Center);
        setVerticalAlignment(iVerticalAlignment::Center);
    }

    iWidgetDialog::~iWidgetDialog()
    {

    }

    void iWidgetDialog::update()
    {
        if (!_widgets.empty())
        {
            iWidget* widget = _widgets[0];
            if (widget->getWidth() + _border * 2 > _width)
            {
                _width = widget->getWidth() + _border * 2;
            }

            if (widget->getHeight() + _border * 2 > _height)
            {
                _height = widget->getHeight() + _border * 2;
            }
        }

        switch (iWidget::getHorrizontalAlignment())
        {
        case iHorrizontalAlignment::Left:
            _posx = 0;
            break;

        case iHorrizontalAlignment::Strech:
            _posx = 0;
            if (iWidgetManager::getInstance().getDesktopWidth() > _width)
            {
                _width = iWidgetManager::getInstance().getDesktopWidth();
            }
            break;

        case iHorrizontalAlignment::Center:
            _posx = (iWidgetManager::getInstance().getDesktopWidth() - _width) / 2;
            break;

        case iHorrizontalAlignment::Right:
            _posx = iWidgetManager::getInstance().getDesktopWidth() - _width;
            break;

        case iHorrizontalAlignment::Absolut:
            _posx = _offsetX;
            break;

        default:;
        };

        switch (iWidget::getVerticalAlignment())
        {
        case iVerticalAlignment::Top:
            _posy = 0;
            break;

        case iVerticalAlignment::Strech:
            _posy = 0;
            if (iWidgetManager::getInstance().getDesktopHeight() > _height)
            {
                _height = iWidgetManager::getInstance().getDesktopHeight();
            }
            break;

        case iVerticalAlignment::Center:
            _posy = (iWidgetManager::getInstance().getDesktopHeight() - _height) / 2;
            break;

        case iVerticalAlignment::Bottom:
            _posy = iWidgetManager::getInstance().getDesktopHeight() - _height;
            break;

        case iVerticalAlignment::Absolut:
            _posy = _offsetY;
            break;

        default:;
        };
    }

    void iWidgetDialog::setHorrizontalAlignment(iHorrizontalAlignment horrizontalAlignment)
    {
        _horrizontalAlignment = horrizontalAlignment;
        
        update();
    }

    void iWidgetDialog::setVerticalAlignment(iVerticalAlignment verticalAlignment)
    {
        _verticalAlignment = verticalAlignment;

        update();
    }

    void iWidgetDialog::setBorder(int32 border)
    {
        _border = border;
        update();
    }

    int32 iWidgetDialog::getBorder()
    {
        return _border;
    }
    
    void iWidgetDialog::draw()
    {
        if (isVisible())
        {
            calcPosition(0, 0, iWidgetManager::getInstance().getDesktopWidth(), iWidgetManager::getInstance().getDesktopHeight());
            iWidgetManager::getInstance().getTheme()->drawDialog(_posx, _posy, _width, _height, _widgetAppearanceState, isActive());

            int32 posx2 = _posx + _border;
            int32 posy2 = _posy + _border;
            int32 width2 = _width - _border - _border;
            int32 height2 = _height - _border - _border;

            for (auto widget : _widgets)
            {
                widget->calcPosition(posx2, posy2, width2, height2);
                widget->draw();
            }
        }
    }

    void iWidgetDialog::setWidth(int32 width)
    {
        _width = width;
        update();
    }

    void iWidgetDialog::setHeight(int32 height)
    {
        _height = height;
        update();
    }

    void iWidgetDialog::setX(int32 x)
    {
        _offsetX = x;
        setHorrizontalAlignment(iHorrizontalAlignment::Absolut);
    }

    void iWidgetDialog::setY(int32 y)
    {
        _offsetY = y;
        setVerticalAlignment(iVerticalAlignment::Absolut);
    }
}