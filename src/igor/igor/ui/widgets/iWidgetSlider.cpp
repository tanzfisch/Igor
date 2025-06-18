// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/ui/widgets/iWidgetSlider.h>

#include <igor/ui/iWidgetManager.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/resources/iResourceManager.h>
#include <igor/data/iIntersection.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{
    // just cull a value between min and max
    static float32 cullValue(float32 value, float32 min, float32 max)
    {
        return std::max(std::min(value, max), min);
    }

    iWidgetSlider::iWidgetSlider(const iWidgetPtr parent)
        : iWidget(iWidgetType::iWidgetSlider, iWidgetKind::Widget, parent)
    {
    }

    iWidgetSlider::~iWidgetSlider()
    {
        _texture = nullptr;
    }

    void iWidgetSlider::setSteppingWheel(float32 up, float32 down)
    {
        _wheelStepUp = up;
        _wheelStepDown = down;
    }

    void iWidgetSlider::setMinValue(float32 min)
    {
        _min = min;
    }

    void iWidgetSlider::setMaxValue(float32 max)
    {
        _max = max;
    }

    float32 iWidgetSlider::getMinValue()
    {
        return _min;
    }

    float32 iWidgetSlider::getMaxValue()
    {
        return _max;
    }

    void iWidgetSlider::updateButton()
    {
        if (_min >= _max ||
            _value < _min ||
            _value > _max)
        {
            return;
        }

        const float32 factor = _value / (_max - _min);
        const float32 offset = (getActualWidth() - 9) * factor;

        _sliderButton._rectangle.setHeight(getActualHeight());
        _sliderButton._rectangle.setWidth(9);
        _sliderButton._rectangle.setY(getActualPosY());
        _sliderButton._rectangle.setX(getActualPosX() + static_cast<int32>(offset));
    }

    void iWidgetSlider::setValue(float32 value)
    {
        const auto newValue = cullValue(value, _min, _max);
        if (_value != newValue)
        {
            _value = newValue;
            updateButton();
            _change(this);
        }
    }

    float32 iWidgetSlider::getValue()
    {
        return _value;
    }

    void iWidgetSlider::setTexture(const iaString &texturePath)
    {
        if (_texturePath != texturePath)
        {
            _texturePath = texturePath;
            _texture = iResourceManager::getInstance().loadResource<iTexture>(_texturePath);
        }
    }

    void iWidgetSlider::increaseNumber(float32 value)
    {
        setValue(_value + value);
    }

    void iWidgetSlider::decreaseNumber(float32 value)
    {
        setValue(_value - value);
    }

    bool iWidgetSlider::onMouseWheel(const iEventMouseWheel &event)
    {
        if (!isEnabled() ||
            !isMouseOver())
        {
            return false;
        }

        if (iWidget::onMouseWheel(event) &&
            !_ignoreChildEventConsumption)
        {
            return true;
        }

        if (event.getWheelDelta() < 0)
        {
            decreaseNumber(_wheelStepDown);
        }
        else
        {
            increaseNumber(_wheelStepUp);
        }

        return true;
    }

    iTexturePtr iWidgetSlider::getTexture() const
    {
        return _texture;
    }

    void iWidgetSlider::calcMinSize()
    {
        updateMinSize(0, 0);
    }

    void iWidgetSlider::onMouseMove(const iEventMouseMove &event)
    {
        if (!isEnabled())
        {
            return;
        }

        const auto &pos = event.getPosition();

        if (_sliderButton._mouseDown)
        {
            float32 scrollDelta = static_cast<float32>(iMouse::getInstance().getPosDelta()._x) / static_cast<float32>(getActualWidth());
            setValue(_value + static_cast<float32>(_max - _min) * scrollDelta);
        }

        auto rect = getActualRect();
        if (iIntersection::intersects(pos, rect) &&
            !event.isConsumed())
        {
            if (!_isMouseOver)
            {
                _widgetState = iWidgetState::Highlighted;
                _mouseOver(this);
            }

            _isMouseOver = true;

            if (!_sliderButton._mouseDown)
            {
                if (iIntersection::intersects(pos, _sliderButton._rectangle))
                {
                    _sliderButton._appearanceState = iWidgetState::Highlighted;
                    _sliderButton._mouseOver = true;
                }
                else
                {
                    _sliderButton._appearanceState = iWidgetState::Standby;
                    _sliderButton._mouseOver = false;
                }
            }
        }
        else
        {
            if (_isMouseOver)
            {
                _widgetState = iWidgetState::Standby;
                _mouseOff(this);

                // TODO need to be able to get key releases when mouse is outside of widget
                _sliderButton._appearanceState = iWidgetState::Standby;
                _sliderButton._mouseOver = false;
                _sliderButton._mouseDown = false;
            }

            _isMouseOver = false;
        }
    }

    bool iWidgetSlider::onMouseKeyUp(const iEventMouseKeyUp &event)
    {
        if (!isEnabled())
        {
            return false;
        }

        if (event.getKey() == iKeyCode::MouseLeft)
        {
            _sliderButton._appearanceState = iWidgetState::Standby;
            _sliderButton._mouseOver = false;
            _sliderButton._mouseDown = false;
            return true;
        }

        return iWidget::onMouseKeyUp(event);
    }

    bool iWidgetSlider::onMouseKeyDown(const iEventMouseKeyDown &event)
    {
        if (!isEnabled() ||
            !isMouseOver())
        {
            return false;
        }

        if (event.getKey() == iKeyCode::MouseLeft &&
            _sliderButton._mouseOver)
        {
            _sliderButton._appearanceState = iWidgetState::Pressed;
            _sliderButton._mouseDown = true;
            return true;
        }

        return iWidget::onMouseKeyDown(event);
    }

    void iWidgetSlider::handleMouseInput(int32 mouseX)
    {
        float32 oldValue = _value;
        float32 factor = static_cast<float32>(mouseX - (getActualPosX() + 5)) / static_cast<float32>(getActualWidth() - 10);

        if (factor < 0.0f)
        {
            factor = 0.0f;
        }

        if (factor > 1.0f)
        {
            factor = 1.0f;
        }

        setValue(_min + (static_cast<float32>(_max - _min) * factor));
    }

    void iWidgetSlider::draw()
    {
        if (!isVisible())
        {
            return;
        }

        iWidgetManager::getInstance().getTheme()->draw(this);
    }

    void iWidgetSlider::setBackgroundTexture(const iaString &texturePath)
    {
        if (_backgroundTexturePath != texturePath)
        {
            _backgroundTexturePath = texturePath;
            _backgroundTexture = iResourceManager::getInstance().loadResource<iTexture>(_backgroundTexturePath);
        }
    }

    iTexturePtr iWidgetSlider::getBackgroundTexture() const
    {
        return _backgroundTexture;
    }

} // namespace igor