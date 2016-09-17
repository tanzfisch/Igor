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
        setWidth(100);
        setHeight(100);
        setHorrizontalAlignment(iHorrizontalAlignment::Center);
        setVerticalAlignment(iVerticalAlignment::Center);
    }

    iWidgetDialog::~iWidgetDialog()
    {
        if (iWidgetManager::isModal(this))
        {
            iWidgetManager::resetModal();
        }
    }

    void iWidgetDialog::calcMinSize()
    {
        int32 minWidth = 0;
        int32 minHeight = 0;

        if (isGrowingByContent() &&
            !_children.empty())
        {
            iWidget* widget = _children[0];
            minWidth = widget->getMinWidth();
            minHeight = widget->getMinHeight();
        }

        setClientArea(_border, _border, _border, _border);
        setMinSize(minWidth, minHeight);
    }

    void iWidgetDialog::updateAlignment(int32 clientWidth, int32 clientHeight)
    {
        int32 width = getMinWidth();
        int32 height = getMinHeight();

        switch (iWidget::getHorrizontalAlignment())
        {
        case iHorrizontalAlignment::Left:
            _relativeX = 0;
            break;

        case iHorrizontalAlignment::Strech:
            _relativeX = 0;
            width = clientWidth;
            break;

        case iHorrizontalAlignment::Center:
            _relativeX = (clientWidth - width) / 2;
            break;

        case iHorrizontalAlignment::Right:
            _relativeX = clientWidth - width;
            break;

        case iHorrizontalAlignment::Absolut:
            _relativeX = _offsetX;
            break;

        default:;
        };

        switch (iWidget::getVerticalAlignment())
        {
        case iVerticalAlignment::Top:
            _relativeY = 0;
            break;

        case iVerticalAlignment::Strech:
            _relativeY = 0;
            height = clientHeight;
            break;

        case iVerticalAlignment::Center:
            _relativeY = (clientHeight - height) / 2;
            break;

        case iVerticalAlignment::Bottom:
            _relativeY = clientHeight - height;
            break;

        case iVerticalAlignment::Absolut:
            _relativeY = _offsetY;
            break;

        default:;
        };

        _actualWidth = width;
        _actualHeight = height;
    }

    void iWidgetDialog::setBorder(int32 border)
    {
        _border = border;
    }

    int32 iWidgetDialog::getBorder()
    {
        return _border;
    }

    void iWidgetDialog::draw(int32 parentPosX, int32 parentPosY)
    {
        if (isVisible())
        {
            iWidgetManager::getInstance().getTheme()->drawDialog(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight(), getAppearanceState(), isActive());

            for (auto widget : _children)
            {
                widget->draw(getActualPosX(), getActualPosY());
            }
        }
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