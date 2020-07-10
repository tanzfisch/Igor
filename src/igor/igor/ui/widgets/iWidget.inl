// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

__IGOR_INLINE__ iWidgetID iWidget::getID() const
{
    return _id;
}

__IGOR_INLINE__ bool iWidget::isBlocked() const
{
    return _blockedEvents;
}

__IGOR_INLINE__ iWidgetID iWidget::getParentID() const
{
    if (_parent != nullptr)
    {
        return _parent->_id;
    }
    else
    {
        return iWidget::INVALID_WIDGET_ID;
    }
}

__IGOR_INLINE__ bool iWidget::hasKeyboardFocus() const
{
    return (_keyboardFocus == this) ? true : false;
}

__IGOR_INLINE__ iWidgetState iWidget::getState() const
{
    return _widgetState;
}

__IGOR_INLINE__ bool iWidget::hasParent() const
{
    return (_parent != nullptr) ? true : false;
}

__IGOR_INLINE__ bool iWidget::isVisible() const
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

__IGOR_INLINE__ int32 iWidget::getConfiguredWidth() const
{
    return _configuredWidth;
}

__IGOR_INLINE__ int32 iWidget::getConfiguredHeight() const
{
    return _configuredHeight;
}

__IGOR_INLINE__ int32 iWidget::getActualWidth() const
{
    return _actualWidth;
}

__IGOR_INLINE__ int32 iWidget::getActualHeight() const
{
    return _actualHeight;
}

__IGOR_INLINE__ int32 iWidget::getMinWidth() const
{
    return _minWidth;
}

__IGOR_INLINE__ int32 iWidget::getMinHeight() const
{
    return _minHeight;
}

__IGOR_INLINE__ iaVector2i iWidget::getActualPos() const
{
    return iaVector2i(_absoluteX, _absoluteY);
}

__IGOR_INLINE__ int32 iWidget::getActualPosX() const
{
    return _absoluteX;
}

__IGOR_INLINE__ int32 iWidget::getActualPosY() const
{
    return _absoluteY;
}

__IGOR_INLINE__ iRectanglei iWidget::getActualRect() const
{
    return iRectanglei(_absoluteX, _absoluteY, _actualWidth, _actualHeight);
}

__IGOR_INLINE__ int32 iWidget::getRelativePosX() const
{
    return _relativeX;
}

__IGOR_INLINE__ int32 iWidget::getRelativePosY() const
{
    return _relativeY;
}

__IGOR_INLINE__ bool iWidget::isActive() const
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
