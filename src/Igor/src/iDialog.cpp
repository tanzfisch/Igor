// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
// see copyright notice in corresponding header file

#include <iDialog.h>

#include <iWidgetBaseTheme.h>
#include <iWidgetManager.h>
#include <iUserControl.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iDialog::iDialog()
    {
        setActive(false);
        setVisible(false);
        setWidth(100);
        setHeight(100);
        setHorizontalAlignment(iHorizontalAlignment::Center);
        setVerticalAlignment(iVerticalAlignment::Center);
    }

    iDialog::~iDialog()
    {
        if (iWidgetManager::isModal(this))
        {
            iWidgetManager::resetModal();
        }
    }

    iDialog* iDialog::createInstance()
    {
        return new iDialog();
    }

    void iDialog::calcMinSize()
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

    void iDialog::updateAlignment(int32 clientWidth, int32 clientHeight)
    {
        int32 width = getMinWidth();
        int32 height = getMinHeight();

        switch (iWidget::getHorizontalAlignment())
        {
        case iHorizontalAlignment::Left:
            _relativeX = 0;
            break;

        case iHorizontalAlignment::Strech:
            _relativeX = 0;
            width = clientWidth;
            break;

        case iHorizontalAlignment::Center:
            _relativeX = (clientWidth - width) / 2;
            break;

        case iHorizontalAlignment::Right:
            _relativeX = clientWidth - width;
            break;

        case iHorizontalAlignment::Absolut:
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

    void iDialog::setBorder(int32 border)
    {
        _border = border;
    }

    int32 iDialog::getBorder()
    {
        return _border;
    }

    void iDialog::draw()
    {
        if (isVisible())
        {
            iWidgetManager::getInstance().getTheme()->drawDialog(getActualRect(), getAppearanceState(), isActive());

            for (auto widget : _children)
            {
                widget->draw();
            }
        }
    }

    void iDialog::setX(int32 x)
    {
        _offsetX = x;
        setHorizontalAlignment(iHorizontalAlignment::Absolut);
    }

    void iDialog::setY(int32 y)
    {
        _offsetY = y;
        setVerticalAlignment(iVerticalAlignment::Absolut);
    }
}