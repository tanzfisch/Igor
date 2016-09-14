// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

uint64 iWidget::getID()
{
    return _id;
}

bool iWidget::hasKeyboardFocus()
{
    return (_keyboardFocus == this) ? true : false;
}

iWidgetAppearanceState iWidget::getAppearanceState()
{
    return _widgetAppearanceState;
}

iWidgetType iWidget::getType() const
{
    return _type;
}

bool iWidget::hasParent()
{
    return (_parent != nullptr) ? true : false;
}

bool iWidget::isVisible()
{
    if (_parent != nullptr)
    {
        if (!_parent->isVisible())
        {
            return false;
        }
    }

    return _visible;
}

int32 iWidget::getConfiguredWidth() const
{
	return _configuredWidth;
}

int32 iWidget::getConfiguredHeight() const
{
	return _configuredHeight;
}

int32 iWidget::getActualWidth() const
{
    return _actualWidth;
}

int32 iWidget::getActualHeight() const
{
    return _actualHeight;
}

int32 iWidget::getContentWidth() const
{
    return _contentWidth;
}

int32 iWidget::getContentHeight() const
{
    return _contentHeight;
}

int32 iWidget::getActualPosX() const
{
	return _absoluteX;
}

int32 iWidget::getActualPosY() const
{
	return _absoluteY;
}

int32 iWidget::getRelativePosX() const
{
	return _relativeX;
}

int32 iWidget::getRelativePosY() const
{
	return _relativeY;
}

bool iWidget::isActive()
{
    if (_parent != nullptr)
    {
        if (!_parent->isActive())
        {
            return false;
        }
    }

    return _active;
}
