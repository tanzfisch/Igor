// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
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
        if (_texture != nullptr)
        {
            _width = _texture->getWidth();
            _height = _texture->getHeight();

            if (_width > _maxWidth)
            {
                _width = _maxWidth;
            }

            if (_height > _maxHeight)
            {
                _height = _maxHeight;
            }
        }

		updateParent();
	}

	void iWidgetPicture::draw()
	{
		if (isVisible() &&
            _texture != nullptr)
		{
			iWidgetManager::getInstance().getTheme()->drawPicture(_posx, _posy, _width, _height, _texture, _widgetAppearanceState, isActive());
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
