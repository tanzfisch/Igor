// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/dialogs/iDialog.h>

#include <igor/ui/theme/iWidgetBaseTheme.h>
#include <igor/ui/iWidgetManager.h>
#include <igor/ui/user_controls/iUserControl.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iDialog::iDialog()
    {
        iWidgetManager::getInstance().registerDialog(this);

        setActive(false);
        setVisible(false);
        setWidth(100);
        setHeight(100);
        setHorizontalAlignment(iHorizontalAlignment::Center);
        setVerticalAlignment(iVerticalAlignment::Center);
    }

    iDialog::~iDialog()
    {
        if (iWidgetManager::getInstance().isModal(this))
        {
            iWidgetManager::getInstance().resetModal();
        }

        iWidgetManager::getInstance().unregisterDialog(this);
    }

    iWidgetType iDialog::getWidgetType() const
    {
        return iWidgetType::iDialog;
    }

    void iDialog::setReturnState(iDialogReturnState returnState)
    {
        _returnState = returnState;
    }

    iDialogReturnState iDialog::getReturnState() const
    {
        return _returnState;
    }

    void iDialog::open(iDialogCloseDelegate dialogCloseDelegate)
    {
        _dialogCloseDelegate = dialogCloseDelegate;
        setActive();
        setVisible();
    }

    void iDialog::close()
    {
        setActive(false);
        setVisible(false);
        iWidgetManager::getInstance().resetModal();

        iWidgetManager::getInstance().closeDialog(this);
    }

    void iDialog::calcMinSize()
    {
        int32 minWidth = 0;
        int32 minHeight = 0;

        if (isGrowingByContent() &&
            !_children.empty())
        {
            iWidgetPtr widget = _children.front();
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
            iWidgetManager &wm = iWidgetManager::getInstance();

            wm.getTheme()->drawDialog(getActualRect(), getState(), isActive());

            for (const auto child : _children)
            {
                child->draw();
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

} // namespace igor