#include <iWidgetUserDialog.h>

#include <iWidgetManager.h>
#include <iWidgetDialog.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iWidgetUserDialog::iWidgetUserDialog()
    {
        _dialog = static_cast<iWidgetDialog*>(iWidgetManager::getInstance().createWidget(iWidgetType::Dialog));
    }

    iWidgetUserDialog::~iWidgetUserDialog()
    {
        if (_dialog != nullptr)
        {
            iWidgetManager::getInstance().destroyWidget(_dialog);
        }
    }

    void iWidgetUserDialog::setX(int32 x)
    {
        getDialog()->setX(x);
    }

    void iWidgetUserDialog::setY(int32 y)
    {
        getDialog()->setY(y);
    }

	void iWidgetUserDialog::setWidth(int32 width)
	{
		getDialog()->setWidth(width);
	}

	void iWidgetUserDialog::setHeight(int32 height)
	{
		getDialog()->setHeight(height);
	}

    bool iWidgetUserDialog::isVisible()
    {
        return getDialog()->isVisible();
    }

    bool iWidgetUserDialog::isActive()
    {
        return getDialog()->isActive();
    }

    void iWidgetUserDialog::setVisible(bool visible)
    {
        getDialog()->setVisible(visible);
    }

    void iWidgetUserDialog::setActive(bool active)
    {
        getDialog()->setActive(active);
    }

    void iWidgetUserDialog::registerOnClickEvent(iClickDelegate clickDelegate)
    {
        _dialog->registerOnClickEvent(clickDelegate);
    }

    void iWidgetUserDialog::registerOnDoubleClickEvent(iDoubleClickDelegate doubleClickDelegate)
    {
        _dialog->registerOnDoubleClickEvent(doubleClickDelegate);
    }

    void iWidgetUserDialog::registerOnMouseOverEvent(iMouseOverDelegate mouseOverDelegate)
    {
        _dialog->registerOnMouseOverEvent(mouseOverDelegate);
    }

    void iWidgetUserDialog::registerOnMouseOffEvent(iMouseOffDelegate mouseOffDelegate)
    {
        _dialog->registerOnMouseOffEvent(mouseOffDelegate);
    }

    void iWidgetUserDialog::registerOnChangeEvent(iChangeDelegate changeDelegate)
    {
        _dialog->registerOnChangeEvent(changeDelegate);
    }

    void iWidgetUserDialog::registerOnFocusEvent(iFocusDelegate focusDelegate)
    {
        _dialog->registerOnFocusEvent(focusDelegate);
    }

    void iWidgetUserDialog::unregisterOnClickEvent(iClickDelegate clickDelegate)
    {
        _dialog->unregisterOnClickEvent(clickDelegate);
    }

    void iWidgetUserDialog::unregisterOnDoubleClickEvent(iDoubleClickDelegate doubleClickDelegate)
    {
        _dialog->unregisterOnDoubleClickEvent(doubleClickDelegate);
    }

    void iWidgetUserDialog::unregisterOnMouseOverEvent(iMouseOverDelegate mouseOverDelegate)
    {
        _dialog->unregisterOnMouseOverEvent(mouseOverDelegate);
    }

    void iWidgetUserDialog::unregisterOnMouseOffEvent(iMouseOffDelegate mouseOffDelegate)
    {
        _dialog->unregisterOnMouseOffEvent(mouseOffDelegate);
    }

    void iWidgetUserDialog::unregisterOnChangeEvent(iChangeDelegate changeDelegate)
    {
        _dialog->unregisterOnChangeEvent(changeDelegate);
    }

    void iWidgetUserDialog::unregisterOnFocusEvent(iFocusDelegate focusDelegate)
    {
        _dialog->unregisterOnFocusEvent(focusDelegate);
    }

    iWidgetDialog* iWidgetUserDialog::getDialog()
    {
        return _dialog;
    }
}