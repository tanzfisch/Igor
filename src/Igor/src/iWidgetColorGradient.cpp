// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetColorGradient.h>

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iTextureFont.h>
#include <iTextureResourceFactory.h>
#include <iRenderer.h>
#include <iIntersection.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iWidgetColorGradient::iWidgetColorGradient()
    {
        _configuredWidth = 60;
        _configuredHeight = 20;
        _reactOnMouseWheel = false;

        setHorizontalAlignment(iHorizontalAlignment::Center);
        setVerticalAlignment(iVerticalAlignment::Center);

        _texture = iTextureResourceFactory::getInstance().loadFile("special\\checker.png");
        _gradient.setValue(0.0, iaColor4f(1, 1, 1, 1));
    }

    iWidgetColorGradient::~iWidgetColorGradient()
    {
        _texture = nullptr;
    }

    iWidget* iWidgetColorGradient::createInstance()
    {
        return new iWidgetColorGradient();
    }

    bool iWidgetColorGradient::handleMouseKeyDown(iKeyCode key)
    {
        iaVector2i mousePos(getLastMouseX(), getLastMouseY());

        if (_interactive)
        {
            iRectanglei gradientRect(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight());
            gradientRect._x += 5;
            gradientRect._width -= 10;
            gradientRect._height /= 2;

            const vector<pair<float, iaColor4f>> gradient = _gradient.getValues();

            iRectanglei buttonRect(0, 0, 0, 0);
            buttonRect._height = getActualHeight() - gradientRect._height - 1;
            buttonRect._width = 9;
            buttonRect._y = gradientRect._height + gradientRect._y + 1;

            iaColor4f color;
            int index = 0;

            for (auto entry : gradient)
            {
                buttonRect._x = (entry.first * gradientRect._width) + gradientRect._x - 4;

                if (iIntersection::isIntersecting(buttonRect, mousePos))
                {
                    _selectionChanged(index);
                    _change(this);
                }

                index++;
            }

            if (iIntersection::isIntersecting(gradientRect, mousePos))
            {
                float32 value = (static_cast<float32>(mousePos._x - gradientRect._x) / static_cast<float32>(gradientRect._width));
                iaColor4f color;
                _gradient.getValue(value, color);
                _colorCreated(value, color);
            }
        }

        return iWidget::handleMouseKeyDown(key);
    }

    void iWidgetColorGradient::registerOnSelectionChangedEvent(iColorGradientSelectionChangedDelegate delegate)
    {
        _selectionChanged.append(delegate);
    }
    
    void iWidgetColorGradient::unregisterOnSelectionChangedEvent(iColorGradientSelectionChangedDelegate delegate)
    {
        _selectionChanged.remove(delegate);
    }

    void iWidgetColorGradient::registerOnColorCreatedEvent(iColorGradientColorCreatedDelegate delegate)
    {
        _colorCreated.append(delegate);
    }
    
    void iWidgetColorGradient::unregisterOnColorCreatedEvent(iColorGradientColorCreatedDelegate delegate)
    {
        _colorCreated.remove(delegate);
    }

    void iWidgetColorGradient::setInteractive(bool interactive)
    {
        _interactive = interactive;
    }

    bool iWidgetColorGradient::isInteractive()
    {
        return _interactive;
    }

    void iWidgetColorGradient::calcMinSize()
    {
        setMinSize(0, 0);
    }

    void iWidgetColorGradient::setGradient(const iGradientColor4f& gradient)
    {
        _gradient = gradient;
    }

    const iGradientColor4f& iWidgetColorGradient::getGradient() const
    {
        return _gradient;
    }

    void iWidgetColorGradient::setUseAlpha(bool useAlpha)
    {
        _useAlpha = useAlpha;
    }

    bool iWidgetColorGradient::isUsingAlpha() const
    {
        return _useAlpha;
    }

    void iWidgetColorGradient::draw()
    {
        if (isVisible())
        {
            iRectanglei gradientRect(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight());

            if (_interactive)
            {
                gradientRect._x += 5;
                gradientRect._width -= 10;
                gradientRect._height *= 2.0f / 3.0f;
            }

            iWidgetManager::getInstance().getTheme()->drawTiledRectangle(gradientRect._x, gradientRect._y, gradientRect._width, gradientRect._height, _texture);
            iWidgetManager::getInstance().getTheme()->drawGradient(gradientRect._x, gradientRect._y, gradientRect._width, gradientRect._height, _gradient);
            iWidgetManager::getInstance().getTheme()->drawRectangle(gradientRect._x, gradientRect._y, gradientRect._width, gradientRect._height);

            if (_interactive)
            {
                const vector<pair<float, iaColor4f>> gradient = _gradient.getValues();

                iRectanglei buttonRect(0, 0, 0, 0);
                buttonRect._height = getActualHeight() - gradientRect._height - 1;
                buttonRect._width = 9;
                buttonRect._y = gradientRect._height + gradientRect._y + 1;

                iaColor4f color;

                for (auto entry : gradient)
                {
                    _gradient.getValue(entry.first, color);
                    color._a = 1.0;
                    buttonRect._x = (entry.first * gradientRect._width) + gradientRect._x - 4;
                    iWidgetManager::getInstance().getTheme()->drawButton(buttonRect._x, buttonRect._y, buttonRect._width, buttonRect._height, color, iWidgetAppearanceState::Standby, isActive());
                }
            }
        }
    }

}
