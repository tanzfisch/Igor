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
        if (iWidgetManager::getInstance().isModal(this))
        {
            iWidgetManager::getInstance().resetModal();
        }
    }

    void iWidgetDialog::updateContentSize()
    {
		int32 width = getConfiguredWidth();
		int32 height = getConfiguredHeight();

        if (isGrowingByContent() &&
			!_children.empty())
        {
            iWidget* widget = _children[0];
            if (widget->getActualWidth() + _border * 2 > width)
            {
                width = widget->getActualWidth() + _border * 2;
            }

            if (widget->getActualHeight() + _border * 2 > height)
            {
                height = widget->getActualHeight() + _border * 2;
            }
        }

       /* switch (iWidget::getHorrizontalAlignment())
        {
        case iHorrizontalAlignment::Left:
            _relativeX = 0;
            break;

        case iHorrizontalAlignment::Strech:
			_relativeX = 0;
            width = iWidgetManager::getInstance().getDesktopWidth();
            break;

        case iHorrizontalAlignment::Center:
			_relativeX = (iWidgetManager::getInstance().getDesktopWidth() - width) / 2;
            break;

        case iHorrizontalAlignment::Right:
			_relativeX = iWidgetManager::getInstance().getDesktopWidth() - width;
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
            height = iWidgetManager::getInstance().getDesktopHeight();
            break;

        case iVerticalAlignment::Center:
			_relativeY = (iWidgetManager::getInstance().getDesktopHeight() - height) / 2;
            break;

        case iVerticalAlignment::Bottom:
			_relativeY = iWidgetManager::getInstance().getDesktopHeight() - height;
            break;

        case iVerticalAlignment::Absolut:
			_relativeY = _offsetY;
            break;

        default:;
        };*/

        setContentSize(width, height);
    }

    void iWidgetDialog::setBorder(int32 border)
    {
        _border = border;
        //update();
    }

    int32 iWidgetDialog::getBorder()
    {
        return _border;
    }
    
    void iWidgetDialog::draw(int32 parentPosX, int32 parentPosY)
    {
		//updatePosition(parentPosX, parentPosY);

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