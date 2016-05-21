// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iWidgetPicture.h>

#include <iaConsole.h>
using namespace IgorAux;

#include <iWidgetManager.h>
#include <iWidgetBaseTheme.h>
#include <iTextureFont.h>
#include <iTextureResourceFactory.h>

namespace Igor
{

	iWidgetPicture::iWidgetPicture()
		: iWidget(iWidgetType::Label)
	{
		setHorrizontalAlignment(iHorrizontalAlignment::Center);
		setVerticalAlignment(iVerticalAlignment::Center);
	}

    iWidgetPicture::~iWidgetPicture()
    {
        _texture = nullptr;
    }

    void iWidgetPicture::setMaxSize(int32 width, int32 height)
    {
        _maxWidth = width;
        _maxHeight = height;
        update();
    }

    int32 iWidgetPicture::getMaxWidth()
    {
        return _maxWidth;
    }

    int32 iWidgetPicture::getMaxHeight()
    {
        return _maxHeight;
    }

	void iWidgetPicture::update()
	{
		int32 width = _configuredWidth;
		int32 height = _configuredHeight;

		if (isGrowingByContent())
		{
			if (_texture != nullptr)
			{
				if (_texture->getWidth() > width)
				{
					width = _texture->getWidth();
				}

				if (_texture->getHeight() > height)
				{
					height = _texture->getHeight();
				}
			}
		}

		if (width > _maxWidth)
		{
			width = _maxWidth;
		}

		if (height > _maxHeight)
		{
			height = _maxHeight;
		}

		iWidget::update(width, height);
	}

	void iWidgetPicture::draw(int32 parentPosX, int32 parentPosY)
	{
		updatePosition(parentPosX, parentPosY);

		if (isVisible() &&
            _texture != nullptr)
		{
			iWidgetManager::getInstance().getTheme()->drawPicture(getActualPosX(), getActualPosY(), getActualWidth(), getActualHeight(), _texture, _widgetAppearanceState, isActive());
		}
	}

	const iaString& iWidgetPicture::getTexture() const
	{
		return _texturePath;
	}
	
    void iWidgetPicture::setTexture(const iaString& texturePath)
	{
        if (_texturePath != texturePath)
        {
            _texturePath = texturePath;
            _texture = iTextureResourceFactory::getInstance().loadFile(_texturePath);

            update();
        }
	}

}
