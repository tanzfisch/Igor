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

bool iWidget::isModal()
{
    return (_modal == this) ? true : false;
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

int32 iWidget::getWidth()
{
    return _width;
}

int32 iWidget::getHeight()
{
    return _height;
}

int32 iWidget::getX()
{
    return _posx;
}

int32 iWidget::getY()
{
    return _posy;
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
