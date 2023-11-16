// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetColorGradient.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/resources/iResourceManager.h>
#include <igor/data/iIntersection.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    static const int32 s_buttonHeight = 20; // TODO move to theme

    iWidgetColorGradient::iWidgetColorGradient(const iWidgetPtr parent)
        : iWidget(iWidgetType::iWidgetColorGradient, iWidgetKind::Widget, parent)
    {
        _configuredMinWidth = 60;
        _configuredMinHeight = 20;
        _reactOnMouseWheel = false;

        setHorizontalAlignment(iHorizontalAlignment::Center);
        setVerticalAlignment(iVerticalAlignment::Center);

        _texture = iResourceManager::getInstance().loadResource<iTexture>("igor_texture_checker");
        _gradient.setValue(0.0, iaColor4f(1, 1, 1, 1));
    }

    iWidgetColorGradient::~iWidgetColorGradient()
    {
        _texture = nullptr;
    }

    void iWidgetColorGradient::blockEvents()
    {
        iWidget::blockEvents();

        // update own events
        _colorCreated.block(true);
    }

    void iWidgetColorGradient::unblockEvents()
    {
        iWidget::unblockEvents();

        // update own events
        _colorCreated.block(false);
    }

    bool iWidgetColorGradient::onMouseKeyDown(iKeyCode key)
    {
        const iaVector2f &mousePos = getLastMousePos();

        if (_interactive)
        {
            iaRectanglef gradientRect(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight());
            gradientRect._x += 5;
            gradientRect._width -= 10;
            gradientRect._height /= 2;

            const auto &gradient = _gradient.getValues();

            iaRectanglef buttonRect(0, 0, 0, 0);
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
                color = _gradient.getValue(value);
                _colorCreated(value, color);
            }
        }

        return iWidget::onMouseKeyDown(key);
    }

    void iWidgetColorGradient::registerOnColorCreatedEvent(iColorGradientColorCreatedDelegate delegate)
    {
        _colorCreated.add(delegate);
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
        updateMinSize(0, 0);
    }

    void iWidgetColorGradient::setGradient(const iaKeyFrameGraphColor4f &gradient)
    {
        _gradient = gradient;
    }

    const iaKeyFrameGraphColor4f &iWidgetColorGradient::getGradient() const
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
        if (!isVisible())
        {
            return;
        }

        iaRectanglef gradientRect(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight());

        if (_interactive)
        {
            gradientRect._x += 5;
            gradientRect._width -= 10;
            gradientRect._height -= s_buttonHeight;
        }

        iWidgetManager::getInstance().getTheme()->drawTiledRectangle(gradientRect, _texture);
        iWidgetManager::getInstance().getTheme()->drawGradient(gradientRect, _gradient);
        iWidgetManager::getInstance().getTheme()->drawRectangle(gradientRect);

        if (_interactive)
        {
            const auto &gradient = _gradient.getValues();

            iaRectanglef buttonRect(0, 0, 0, 0);
            buttonRect._height = s_buttonHeight;
            buttonRect._width = 9;
            buttonRect._y = gradientRect._height + gradientRect._y + 1;

            iaColor4f color;

            for (auto entry : gradient)
            {
                color = _gradient.getValue(entry.first);
                color._a = 1.0f;
                buttonRect._x = static_cast<int32>(entry.first * gradientRect._width) + gradientRect._x - 4;
                iWidgetManager::getInstance().getTheme()->drawButton(buttonRect, color, iWidgetState::Standby, isEnabled());
            }
        }
    }

} // namespace igor
