// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetSlider.h>

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iTextureFont.h>
#include <iTextureResourceFactory.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
	iWidgetSlider::iWidgetSlider()
		: iWidget(iWidgetType::Button)
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
        //update();
    }

    void iWidgetSlider::setMaxValue(float32 max)
    {
        _max = max;
        //update();
    }

    float32 iWidgetSlider::getMinValue()
    {
        return _min;
    }

    float32 iWidgetSlider::getMaxValue()
    {
        return _max;
    }

    void iWidgetSlider::setValue(float32 value)
    {
		if (_value != value)
		{
			_value = value;
			cullBoundings();
			_change(this);
		}
    }

    float32 iWidgetSlider::getValue()
    {
        return _value;
    }

    void iWidgetSlider::setTexture(const iaString& texturePath)
    {
        if (_texturePath != texturePath)
        {
            _texturePath = texturePath;
            _texture = iTextureResourceFactory::getInstance().loadFile(_texturePath);

            //update();
        }
    }

    void iWidgetSlider::cullBoundings()
    {
        if (_value > _max)
        {
            _value = _max;
        }

        if (_value < _min)
        {
            _value = _min;
        }
    }

    void iWidgetSlider::increaseNumber(float32 value)
    {
		if (value != 0.0f)
		{
			_value += value;
			cullBoundings();

			_change(this);
		}
    }

    void iWidgetSlider::decreaseNumber(float32 value)
    {
		if (value != 0.0f)
		{
			_value -= value;
			cullBoundings();

			_change(this);
		}
    }

    bool iWidgetSlider::handleMouseWheel(int32 d)
    {
        if (!isActive())
        {
            return false;
        }

        iWidget::handleMouseWheel(d);

        if (isMouseOver())
        {
            if (d < 0)
            {
                decreaseNumber(_wheelStepDown);
            }
            else
            {
                increaseNumber(_wheelStepUp);
            }

            return true;
        }

        return false;
    }

    const iaString& iWidgetSlider::getTexture() const
    {
        return _texturePath;
    }

	void iWidgetSlider::calcMinSize()
	{
        setMinSize(0, 0);
	}

    bool iWidgetSlider::handleMouseKeyDown(iKeyCode key)
    {
        if (!isActive())
        {
            return false;
        }

        if (isMouseOver())
        {
            handleMouseInput(_mouseX);
        }

        return iWidget::handleMouseKeyDown(key);
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

        _value = _min + (static_cast<float32>(_max - _min) * factor);

        if (_value < _min)
        {
            _value = _min;
        }

        if (_value > _max)
        {
            _value = _max;
        }

        cullBoundings();

        if (oldValue != _value)
        {
            _change(this);
        }
    }

    void iWidgetSlider::handleMouseMove(int32 x, int32 y)
    {
        if (!isActive())
        {
            return;
        }

        _mouseX = x;
        _mouseY = y;

        iWidget::handleMouseMove(x, y);

        if (isMouseOver() &&
            iMouse::getInstance().getLeftButton())
        {
            handleMouseInput(_mouseX);
        }
    }

	void iWidgetSlider::draw()
	{
        con_assert(_min < _max, "invalid configuration");

		if (isVisible())
		{
            if (_backgroundTexture != nullptr)
            {
                iWidgetManager::getInstance().getTheme()->drawTiledRectangle(getActualPosX(), getActualPosY() + getActualHeight() / 4, getActualWidth(), getActualHeight() / 2, _backgroundTexture);
            }
            
            if (_texture != nullptr)
            {
                iWidgetManager::getInstance().getTheme()->drawPicture(getActualPosX(), getActualPosY() + getActualHeight() / 4, getActualWidth(), getActualHeight() / 2, _texture, getAppearanceState(), isActive());
            }

            if(_backgroundTexture == nullptr &&
                _texture == nullptr)
            {
                
                iWidgetManager::getInstance().getTheme()->drawFilledRectangle(getActualPosX(), getActualPosY() + getActualHeight() / 2 -2 , getActualWidth(), 4);
                iWidgetManager::getInstance().getTheme()->drawRectangle(getActualPosX(), getActualPosY() + getActualHeight() / 2 - 2, getActualWidth(), 4);
            }

            float32 factor = _value / (_max - _min);
            float32 offset = (getActualWidth() - 10) * factor;
            iWidgetManager::getInstance().getTheme()->drawFilledRectangle(getActualPosX() + offset, getActualPosY(), 10, getActualHeight());
            iWidgetManager::getInstance().getTheme()->drawRectangle(getActualPosX() + offset, getActualPosY(), 10, getActualHeight());
		}
	}

    void iWidgetSlider::setBackgroundTexture(const iaString& texturePath)
    {
        if (_backgroundTexturePath != texturePath)
        {
            _backgroundTexturePath = texturePath;
            _backgroundTexture = iTextureResourceFactory::getInstance().loadFile(_backgroundTexturePath);

            // update();
        }
    }

    const iaString& iWidgetSlider::getBackgroundTexture() const
    {
        return _backgroundTexturePath;
    }

}