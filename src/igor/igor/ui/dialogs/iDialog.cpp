// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/dialogs/iDialog.h>

#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/ui/iWidgetManager.h>
#include <igor/ui/user_controls/iUserControl.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iDialog::iDialog(iWidgetType type, const iWidgetPtr parent)
        : iWidget(type, iWidgetKind::Dialog, nullptr)
    {
        iWidgetManager::getInstance().registerDialog(this);

        setEnabled(false);
        setVisible(false);
        setWidth(100);
        setHeight(100);
        setHorizontalAlignment(iHorizontalAlignment::Center);
        setVerticalAlignment(iVerticalAlignment::Center);

        if (parent != nullptr)
        {
            iWidgetPtr parentDialog = nullptr;
            iWidgetPtr iterator = parent;

            while (iterator->_parent != nullptr)
            {
                iterator = iterator->_parent;
                if (iterator->getWidgetKind() == iWidgetKind::Dialog)
                {
                    parentDialog = iterator;
                    break;
                }
            }

            if (parentDialog != nullptr)
            {
                setZValue(parentDialog->getZValue() + 1);
            }
        }
    }

    iDialog::~iDialog()
    {
        if (iWidgetManager::getInstance().isModal(this))
        {
            iWidgetManager::getInstance().resetModal();
        }

        iWidgetManager::getInstance().unregisterDialog(this);
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
        setEnabled();
        setVisible();

        _isOpen = true;
    }

    void iDialog::close()
    {
        setEnabled(false);
        setVisible(false);
        iWidgetManager::getInstance().resetModal();
        iWidgetManager::getInstance().closeDialog(this);

        _isOpen = false;
    }

    bool iDialog::isOpen() const
    {
        return _isOpen;
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

        case iHorizontalAlignment::Stretch:
            _relativeX = 0;
            width = clientWidth;
            break;

        case iHorizontalAlignment::Center:
            _relativeX = (clientWidth - width) / 2;
            break;

        case iHorizontalAlignment::Right:
            _relativeX = clientWidth - width;
            break;

        case iHorizontalAlignment::Absolute:
            _relativeX = _offsetX;
            break;

        default:;
        };

        switch (iWidget::getVerticalAlignment())
        {
        case iVerticalAlignment::Top:
            _relativeY = 0;
            break;

        case iVerticalAlignment::Stretch:
            _relativeY = 0;
            height = clientHeight;
            break;

        case iVerticalAlignment::Center:
            _relativeY = (clientHeight - height) / 2;
            break;

        case iVerticalAlignment::Bottom:
            _relativeY = clientHeight - height;
            break;

        case iVerticalAlignment::Absolute:
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

            wm.getTheme()->drawDialog(getActualRect(), getState(), isEnabled());

            for (const auto child : _children)
            {
                child->draw();
            }
        }
    }

    void iDialog::setX(int32 x)
    {
        _offsetX = x;
        setHorizontalAlignment(iHorizontalAlignment::Absolute);
    }

    void iDialog::setY(int32 y)
    {
        _offsetY = y;
        setVerticalAlignment(iVerticalAlignment::Absolute);
    }

    void iDialog::setPos(int32 x, int32 y)
    {
        _offsetX = x;
        _offsetY = y;
        setVerticalAlignment(iVerticalAlignment::Absolute);
        setHorizontalAlignment(iHorizontalAlignment::Absolute);
    }

} // namespace igor