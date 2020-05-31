// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetColorGradient.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetBaseTheme.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/resources/texture/iTextureResourceFactory.h>
#include <igor/graphics/iRenderer.h>
#include <igor/data/iIntersection.h>

#include <iaux/system/iaConsole.h>
using namespace IgorAux;

namespace Igor
{

    iWidgetColorGradient::iWidgetColorGradient(const iWidgetPtr parent)
        : iWidget(parent)
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

    iWidgetType iWidgetColorGradient::getWidgetType() const
    {
        return iWidgetType::iWidgetColorGradient;
    }

    void iWidgetColorGradient::block(bool blockEvents)
    {
        iWidget::block(blockEvents);

        // update own events
        _colorCreated.block(isBlocked());
    }

    bool iWidgetColorGradient::handleMouseKeyDown(iKeyCode key)
    {
        iaVector2i mousePos = getLastMousePos();

        if (_interactive)
        {
            iRectanglei gradientRect(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight());
            gradientRect._x += 5;
            gradientRect._width -= 10;
            gradientRect._height /= 2;

            const std::vector<std::pair<float, iaColor4f>> gradient = _gradient.getValues();

            iRectanglei buttonRect(0, 0, 0, 0);
            buttonRect._height = getActualHeight() - gradientRect._height - 1;
            buttonRect._width = 9;
            buttonRect._y = gradientRect._height + gradientRect._y + 1;

            int index = 0;

            for (auto entry : gradient)
            {
                buttonRect._x = static_cast<int32>(entry.first * gradientRect._width) + gradientRect._x - 4;

                if (iIntersection::intersects(mousePos, buttonRect))
                {
                    _selectionChanged(index);
                    _change(this);
                }

                index++;
            }

            if (iIntersection::intersects(mousePos, gradientRect))
            {
                float32 value = (static_cast<float32>(mousePos._x - gradientRect._x) / static_cast<float32>(gradientRect._width));
                iaColor4f color;
                _gradient.getValue(value, color);
                _colorCreated(value, color);
            }
        }

        return iWidget::handleMouseKeyDown(key);
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

    void iWidgetColorGradient::setGradient(const iaGradientColor4f &gradient)
    {
        _gradient = gradient;
    }

    const iaGradientColor4f &iWidgetColorGradient::getGradient() const
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
        const int32 buttonHeight = 20;

        if (isVisible())
        {
            iRectanglei gradientRect(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight());

            if (_interactive)
            {
                gradientRect._x += 5;
                gradientRect._width -= 10;
                gradientRect._height -= buttonHeight;
            }

            iWidgetManager::getInstance().getTheme()->drawTiledRectangle(gradientRect, _texture);
            iWidgetManager::getInstance().getTheme()->drawGradient(gradientRect, _gradient);
            iWidgetManager::getInstance().getTheme()->drawRectangle(gradientRect);

            if (_interactive)
            {
                const std::vector<std::pair<float, iaColor4f>> gradient = _gradient.getValues();

                iRectanglei buttonRect(0, 0, 0, 0);
                buttonRect._height = buttonHeight;
                buttonRect._width = 9;
                buttonRect._y = gradientRect._height + gradientRect._y + 1;

                iaColor4f color;

                for (auto entry : gradient)
                {
                    _gradient.getValue(entry.first, color);
                    color._a = 1.0f;
                    buttonRect._x = static_cast<int32>(entry.first * gradientRect._width) + gradientRect._x - 4;
                    iWidgetManager::getInstance().getTheme()->drawButton(buttonRect, color, iWidgetState::Standby, isActive());
                }
            }
        }
    }

} // namespace Igor
